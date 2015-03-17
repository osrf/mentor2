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
/*  if (_sdf->HasElement("gear_ratio"))
  {
    sdf::ElementPtr elem = _sdf->GetElement("gear_ratio");
    this->gearRatio = elem->Get<double>();
  }*/
}

/////////////////////////////////////////////////
void GearboxPlugin::Init()
{
  SimpleModelPlugin::Init();
}

/////////////////////////////////////////////////
physics::JointPtr GearboxPlugin::SpawnJoint(const std::string &_link1,
    const std::string &_link2, const std::string &_jointType,
    const math::Vector3 &_jointAxis)
{
  msgs::Model msg;
  std::string modelName = this->parent->GetName();
  msg.set_name(modelName);
  msgs::Set(msg.mutable_pose(), this->parent->GetWorldPose());


  msgs::Link *link =  msg.add_link();
  link->set_name(_link1);

  msgs::Link *link2 =  msg.add_link();
  link2->set_name(_link2);

  msgs::Joint *jointMsg = msg.add_joint();
  jointMsg->set_name("test_joint");
  jointMsg->set_type(msgs::ConvertJointType(_jointType));
  msgs::Set(jointMsg->mutable_pose(), math::Pose::Zero);

  jointMsg->set_parent(_link1);
  jointMsg->set_child(_link2);


  {
    msgs::Axis *axis = jointMsg->mutable_axis1();
    msgs::Set(axis->mutable_xyz(), _jointAxis);
    axis->set_use_parent_model_frame(false);
  }

  physics::WorldPtr world = this->parent->GetWorld();
  world->InsertModelString(
    "<sdf version='" + std::string(SDF_VERSION) + "'>"
    + msgs::ModelToSDF(msg)->ToString("")
    + "</sdf>");
}

/////////////////////////////////////////////////
void GearboxPlugin::UpdateImpl(double _timeSinceLastUpdate)
{

  // GearboxPlugin: Dynamically creates a gearbox joint between two links
  // that are connected to the parent model
  // e.g. Gearbox joint will be created between link1 and link2:
  // link1 -- joint1 -- PARENT_MODEL -- joint2 -- link2


  // find two joints connected to ththe parent model
  if (this->joints.empty())
  {
    if (this->parent->GetJointCount() < 2u)
      return;

    std::vector<physics::JointPtr> parentJoints = this->parent->GetJoints();
    this->joints.push_back(parentJoints[0]->GetName());
    this->joints.push_back(parentJoints[1]->GetName());
  }

  // set up a gearbox joint between the two links that are connected to this
  // parent model
  if (!this->gearboxJoint)
  {
    this->gearRatio = this->GetProperty<double>("gear_ratio");

    physics::JointPtr joint1 = this->parent->GetJoint(this->joints[0]);
    physics::JointPtr joint2 = this->parent->GetJoint(this->joints[1]);

    if (joint1 && joint2)
    {
      physics::LinkPtr link1 = joint1->GetJointLink(0);
      if (link1 && link1->GetName() == this->parent->GetName())
        link1 = joint1->GetJointLink(1);
      physics::LinkPtr link2 = joint2->GetJointLink(0);
      if (link2 && link2->GetName() == this->parent->GetName())
        link2 = joint2->GetJointLink(1);

      if (link1 && link2)
      {
        this->SpawnJoint(link1->GetName(), link2->GetName(), "gearbox",
            math::Vector3::UnitX);
/*        link1->SetAutoDisable(false);
        link2->SetAutoDisable(false);
        physics::WorldPtr world = this->parent->GetWorld();
        physics::PhysicsEnginePtr physics = world->GetPhysicsEngine();
        this->gearboxJoint = physics->CreateJoint("gearbox", this->parent);
        gearboxJoint->Attach(link1, link2);
        gearboxJoint->Load(link1, link2, math::Pose::Zero);
        physics::GearboxJoint *gbJoint;
//            dynamic_cast<physics::GearboxJoint *>(this->gearboxJoint.get());

        if (gbJoint)
          gbJoint->SetGearboxRatio(this->gearRatio);
        gearboxJoint->Init();*/
      }
    }
  }
  else
  {
    // check if the two joints still exist, if not remove the gearbox joint
    if (this->joints.size() == 2u)
    {
      physics::JointPtr joint1 = this->parent->GetJoint(this->joints[0]);
      physics::JointPtr joint2 = this->parent->GetJoint(this->joints[1]);

      if (!joint1 || !joint2)
      {
        this->joints.clear();
        this->gearboxJoint->Detach();
        this->gearboxJoint.reset();
      }
    }
  }
}
