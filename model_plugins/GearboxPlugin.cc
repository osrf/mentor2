/*
 * Copyright (C) 2015 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gazebo/physics/physics.hh>

#include <gazebo/msgs/msgs.hh>
#include <gazebo/physics/GearboxJoint.hh>

#include "GearboxPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(GearboxPlugin)

/////////////////////////////////////////////////
GearboxPlugin::GearboxPlugin()
{
  this->schematicType = "gearbox";
  this->gearRatio = 1.0;
  this->efficiency = 1.0;
  this->parentLinkName = "";
  this->childLinkName = "";
}

/////////////////////////////////////////////////
GearboxPlugin::~GearboxPlugin()
{
  if (this->gearboxJoint)
  {
    this->gearboxJoint->Detach();
    this->gearboxJoint.reset();
  }
}

/////////////////////////////////////////////////
void GearboxPlugin::LoadImpl(sdf::ElementPtr _sdf)
{
  this->gearRatio = this->GetProperty<double>("gear_ratio");
  this->parentLinkName = this->GetProperty<std::string>("parent");
  this->childLinkName = this->GetProperty<std::string>("child");
  this->efficiency = this->GetProperty<double>("efficiency");
}

/////////////////////////////////////////////////
void GearboxPlugin::Init()
{
  SimpleModelPlugin::Init();
}

/////////////////////////////////////////////////
void GearboxPlugin::Reset()
{
  if (this->gearboxJoint)
  {
    this->gearboxJoint->Detach();
    this->gearboxJoint.reset();
    std::cerr << " gearbox reset " << std::endl;
  }

  if (this->parentLinkJoint)
  {
  }

  if (this->childLinkJoint)
  {
  }
}

/////////////////////////////////////////////////
void GearboxPlugin::UpdateImpl(double _timeSinceLastUpdate)
{
  physics::BasePtr b = this->parent;
  while ( b->GetParent() &&
       b->GetParent()->HasType(physics::Base::MODEL))
  {
     b =  b->GetParent();
  }
  if (b)
  {
//    std::cerr << " got b " << std::endl;
    physics::ModelPtr rootModel =
        boost::dynamic_pointer_cast<physics::Model>(b);
    if (rootModel)
    {
//      std::cerr << " got rootModel " << std::endl;
      physics::ModelPtr axle = rootModel->NestedModel("axle_wheels");
      if (axle)
      {
//        std::cerr << " got axle " << std::endl;
        physics::LinkPtr axleLink = axle->GetLink("link");
//        std::cerr << axleLink->GetWorldLinearVel() << std::endl;
      }
    }
  }




  if (this->gearboxJoint)
  {
    if (this->parentLinkJoint && this->childLinkJoint)
    {
      physics::JointWrench parentWrench =
          this->parentLinkJoint->GetForceTorque(0u);
      physics::JointWrench childWrench =
          this->childLinkJoint->GetForceTorque(0u);

      /// rotate into appropriate link frame
      math::Vector3 rotatedAxis =
          this->childLinkJoint->GetChild()->GetWorldPose().rot.GetInverse() *
          this->childLinkJoint->GetGlobalAxis(0);

      // project torque vector onto joint axis
      double torque = rotatedAxis.Dot(childWrench.body2Torque);

      // double torque = childWrench.body2Torque.GetLength();

/*      std::cerr << "child body1Torque " <<  childWrench.body1Torque
          << std::endl;
      std::cerr << "child body2Torque " << childWrench.body2Torque
          << std::endl;
      std::cerr << "parent body1Torque "  << parentWrench.body1Torque
          << std::endl;
      std::cerr << "parent body2Torque "  << parentWrench.body2Torque
          << std::endl;*/

//      std::cerr << "projected torque: " << childWrench.body2Torque <<
//            " vs " << torque << " vs " <<
//            childWrench.body2Torque.GetLength() << std::endl;
//      std::cerr << "rotated axis: " << rotatedAxis << std::endl;
//      std::cerr << "projected torque: " << torque << std::endl;

      // TODO override torque for now.
      torque = parentWrench.body2Torque.GetLength();
//      std::cerr << "torque " << torque << std::endl;

      this->childLinkJoint->SetParam("friction", 0,
          (1-this->efficiency)*fabs(torque));

    }
    return;
  }

  // GearboxPlugin: Dynamically creates a gearbox joint between two links


  std::string rootModelName;
  physics::BasePtr entity = this->parent;
/*  while (entity->GetParent() &&
      entity->GetParent()->HasType(physics::Base::MODEL))
  {
    entity = entity->GetParent();
  }*/

  physics::WorldPtr world = this->parent->GetWorld();
  while (!this->parentLink || !this->childLink)
  {
    // try find parent and child links
    std::string parentScopedName =
        entity->GetScopedName() + "::" + this->parentLinkName;
    std::string childScopedName =
        entity->GetScopedName() + "::" + this->childLinkName;
    this->parentLink = boost::dynamic_pointer_cast<physics::Link>(
      world->GetByName(parentScopedName));
    this->childLink = boost::dynamic_pointer_cast<physics::Link>(
      world->GetByName(childScopedName));
    if (entity->GetParent() &&
        entity->GetParent()->HasType(physics::Base::MODEL))
      entity = entity->GetParent();
    else
      break;
  }

  if (!this->parentLink || !this->childLink)
  {
    // try harder
    size_t pos = this->parentLinkName.find("::");
    if (pos != std::string::npos &&
        this->parentLinkName.substr(0, pos) != entity->GetName())
    {
      this->parentLinkName = entity->GetName() +
          this->parentLinkName.substr(pos);
    }
    pos = this->childLinkName.find("::");
    if (pos != std::string::npos &&
        this->childLinkName.substr(0, pos) != entity->GetName())
    {
      this->childLinkName = entity->GetName() +
          this->childLinkName.substr(pos);
    }
    this->parentLink = boost::dynamic_pointer_cast<physics::Link>(
      world->GetByName(this->parentLinkName));
    this->childLink = boost::dynamic_pointer_cast<physics::Link>(
      world->GetByName(this->childLinkName));
  }

  if (!this->parentLink || !this->childLink)
  {
    gzerr << "Gearbox plugin: unable to find parent or child link" << std::endl;
    return;
  }

  // auto determine gearbox axes
  math::Quaternion axisTransform =
      this->parent->GetLink()->GetWorldPose().rot.GetInverse();

  // parent link axis
  math::Vector3 parentLinkAxis = math::Vector3::UnitY;
  physics::ModelPtr parentLinkModel = this->parentLink->GetModel();
  bool foundParentLinkAxis = false;
  while (parentLinkModel)
  {
    physics::Joint_V parentLinkJoints = parentLinkModel->GetJoints();
    for (unsigned int i = 0; i < parentLinkJoints.size(); ++i)
    {
      physics::JointPtr joint = parentLinkJoints[i];
      if (joint->GetParent() == parentLink ||
          joint->GetChild() == parentLink)
      {
        if (joint->GetMsgType() == msgs::Joint::REVOLUTE &&
            (joint->GetLowerLimit(0) != math::Angle(0) ||
            joint->GetUpperLimit(0) != math::Angle(0)))
        {
          parentLinkAxis = axisTransform * joint->GetGlobalAxis(0);
          foundParentLinkAxis = true;
          this->parentLinkJoint = joint;
          this->parentLinkJoint->SetProvideFeedback(true);
          break;

        }
      }
    }
    if (foundParentLinkAxis)
      break;

    physics::BasePtr p = parentLinkModel->GetParent();
    if (p->HasType(physics::Base::MODEL))
      parentLinkModel = boost::static_pointer_cast<physics::Model>(p);
    else
      break;
  }

  // child link axis
  math::Vector3 childLinkAxis = math::Vector3::UnitY;
  physics::ModelPtr childLinkModel = this->childLink->GetModel();
  bool foundChildLinkAxis = false;
  while (childLinkModel)
  {
    physics::Joint_V childLinkJoints = childLinkModel->GetJoints();
    for (unsigned int i = 0; i < childLinkJoints.size(); ++i)
    {
      physics::JointPtr joint = childLinkJoints[i];
      if (joint->GetParent() == childLink ||
          joint->GetChild() == childLink)
      {
        if (joint->GetMsgType() == msgs::Joint::REVOLUTE &&
            (joint->GetLowerLimit(0) != math::Angle(0) ||
            joint->GetUpperLimit(0) != math::Angle(0)))
        {
          childLinkAxis = axisTransform * joint->GetGlobalAxis(0);
          foundChildLinkAxis = true;
          this->childLinkJoint = joint;
          this->childLinkJoint->SetProvideFeedback(true);
          break;
        }
      }
    }
    if (foundChildLinkAxis)
      break;

    physics::BasePtr p = childLinkModel->GetParent();
    if (p->HasType(physics::Base::MODEL))
      childLinkModel = boost::static_pointer_cast<physics::Model>(p);
    else
      break;
  }


  this->parentLink->SetAutoDisable(false);
  this->childLink->SetAutoDisable(false);
  physics::PhysicsEnginePtr physics = world->GetPhysicsEngine();
  this->gearboxJoint = physics->CreateJoint("gearbox", this->parent);
  this->gearboxJoint->SetModel(this->parent);

  sdf::ElementPtr jointSDF;
  jointSDF.reset(new sdf::Element);
  sdf::initFile("joint.sdf", jointSDF);
  jointSDF->GetElement("parent")->Set(this->parentLinkName);
  jointSDF->GetElement("child")->Set(this->childLinkName);
  jointSDF->GetElement("gearbox_ratio")->Set(this->gearRatio);
  jointSDF->GetElement("gearbox_reference_body")->Set(
      this->parent->GetLink()->GetName());


  sdf::ElementPtr axis1Elem = jointSDF->GetElement("axis");
  axis1Elem->GetElement("xyz")->Set(parentLinkAxis);
  sdf::ElementPtr axis2Elem = jointSDF->GetElement("axis2");
  axis2Elem->GetElement("xyz")->Set(childLinkAxis);

  std::cerr << "gear ratio " << this->gearRatio << std::endl;
  std::cerr << "efficiency " << this->efficiency << std::endl;

  this->gearboxJoint->Load(jointSDF);
  this->gearboxJoint->Init();

/*        physics::GearboxJoint *gbJoint =
      dynamic_cast<physics::GearboxJoint *>(this->gearboxJoint.get());

  if (gbJoint)
    gbJoint->SetGearboxRatio(this->gearRatio);
  gearboxJoint->Init();*/


}
