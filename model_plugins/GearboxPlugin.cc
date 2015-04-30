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
}

/////////////////////////////////////////////////
void GearboxPlugin::Init()
{
  SimpleModelPlugin::Init();
}

/////////////////////////////////////////////////
void GearboxPlugin::UpdateImpl(double _timeSinceLastUpdate)
{

  // GearboxPlugin: Dynamically creates a gearbox joint between two links

  this->gearRatio = this->GetProperty<double>("gear_ratio");
  this->parentLinkName = this->GetProperty<std::string>("parent");
  this->childLinkName = this->GetProperty<std::string>("child");
  this->efficiency = this->GetProperty<double>("efficiency");

  if (!this->gearboxJoint)
  {
    physics::WorldPtr world = this->parent->GetWorld();
    physics::LinkPtr parentLink = boost::dynamic_pointer_cast<physics::Link>(
      world->GetByName(this->parentLinkName));
    physics::LinkPtr childLink = boost::dynamic_pointer_cast<physics::Link>(
      world->GetByName(this->childLinkName));

    if (!parentLink || !childLink)
      return;

    // auto determine gearbox axes
    math::Quaternion axisTransform =
        this->parent->GetLink()->GetWorldPose().rot.GetInverse();

    // parent link axis
    math::Vector3 parentLinkAxis = math::Vector3::UnitY;
    physics::ModelPtr parentLinkModel = parentLink->GetModel();
    bool foundParentLinkAxis = false;
    while (parentLinkModel)
    {
      physics::Joint_V parentLinkJoints = parentLinkModel->GetJoints();
      std::cerr << " parentLinkModel " << parentLinkModel->GetName() << " " <<
          parentLinkJoints.size() <<  std::endl;
      for (unsigned int i = 0; i < parentLinkJoints.size(); ++i)
      {
        physics::JointPtr joint = parentLinkJoints[i];
        std::cerr << " parent model joint "  << joint->GetName() << std::endl;
        if (joint->GetParent() == parentLink ||
            joint->GetChild() == parentLink)
        {
          if (joint->GetMsgType() == msgs::Joint::REVOLUTE &&
              (joint->GetLowerLimit(0) != math::Angle(0) ||
              joint->GetUpperLimit(0) != math::Angle(0)))
          {
            parentLinkAxis = axisTransform * joint->GetGlobalAxis(0);
            std::cerr << "parent " << parentLinkAxis <<  " VS " <<
                joint->GetGlobalAxis(0) << std::endl;
            foundParentLinkAxis = true;
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
    physics::ModelPtr childLinkModel = childLink->GetModel();
    bool foundChildLinkAxis = false;
    while (childLinkModel)
    {
      physics::Joint_V childLinkJoints = childLinkModel->GetJoints();
      std::cerr << " childLinkModel " << childLinkModel->GetName() << " " <<
          childLinkJoints.size() <<  std::endl;
      for (unsigned int i = 0; i < childLinkJoints.size(); ++i)
      {
        physics::JointPtr joint = childLinkJoints[i];
        std::cerr << " child model joint "  << joint->GetName() << std::endl;
        if (joint->GetParent() == childLink ||
            joint->GetChild() == childLink)
        {
          if (joint->GetMsgType() == msgs::Joint::REVOLUTE &&
              (joint->GetLowerLimit(0) != math::Angle(0) ||
              joint->GetUpperLimit(0) != math::Angle(0)))
          {
            childLinkAxis = axisTransform * joint->GetGlobalAxis(0);
            std::cerr << "child " << childLinkAxis << std::endl;
            foundChildLinkAxis = true;
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


    parentLink->SetAutoDisable(false);
    childLink->SetAutoDisable(false);
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
    jointSDF->GetElement("axis")->GetElement("xyz")->Set(
        parentLinkAxis);
    jointSDF->GetElement("axis2")->GetElement("xyz")->Set(
        childLinkAxis);

    std::cerr << "gear ratio " << this->gearRatio << std::endl;
    std::cerr << "efficiency " << this->efficiency << std::endl;

    this->gearboxJoint->Load(jointSDF);

/*    physics::GearboxJoint *gbJoint =
        dynamic_cast<physics::GearboxJoint *>(this->gearboxJoint.get());

    if (gbJoint)
      gbJoint->SetGearboxRatio(this->gearRatio);*/

    this->gearboxJoint->Init();

/*        physics::GearboxJoint *gbJoint =
        dynamic_cast<physics::GearboxJoint *>(this->gearboxJoint.get());

    if (gbJoint)
      gbJoint->SetGearboxRatio(this->gearRatio);
    gearboxJoint->Init();*/

  }

  return;
}
