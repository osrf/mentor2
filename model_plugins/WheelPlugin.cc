/*
 * Copyright (C) 2014 Open Source Robotics Foundation
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

#include "WheelPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(WheelPlugin)

/////////////////////////////////////////////////
WheelPlugin::WheelPlugin()
{
}

/////////////////////////////////////////////////
WheelPlugin::~WheelPlugin()
{
}

/////////////////////////////////////////////////
void WheelPlugin::LoadImpl(sdf::ElementPtr _sdf)
{
  if (_sdf->HasElement("shaft_link"))
  {
    sdf::ElementPtr elem = _sdf->GetElement("shaft_link");
    this->shaftLinkName = elem->Get<std::string>();
  }
  if (_sdf->HasElement("body_link"))
  {
    sdf::ElementPtr elem = _sdf->GetElement("body_link");
    this->bodyLinkName = elem->Get<std::string>();
  }
}

/////////////////////////////////////////////////
void WheelPlugin::Init()
{
  SimpleModelPlugin::Init();

  // sub not used any more, remove me later
  //this->torqueSub = this->node->Subscribe("~/motor/torque",
  //    &WheelPlugin::OnTorque, this);
  //this->joint = this->parent->GetJoint("world_joint");

  this->shaftLink = boost::dynamic_pointer_cast<physics::Link>(
      this->parent->GetWorld()->GetEntity(this->shaftLinkName));

  this->bodyLink = boost::dynamic_pointer_cast<physics::Link>(
      this->parent->GetWorld()->GetEntity(this->bodyLinkName));
}

/////////////////////////////////////////////////
void WheelPlugin::OnTorque(ConstVariantPtr &_msg)
{
  // HACK: This is setting max force, while we are manually controlling the
  // max velocity

  // not used any more, remove me later.
  //this->joint->SetForce(0, _msg->v_double());

/*  if (_msg->v_double() > 0)
    this->joint->SetVelocity(0, 0.7);
  else
    this->joint->SetVelocity(0, 0.0);*/
}

/////////////////////////////////////////////////
void WheelPlugin::UpdateImpl(double _timeSinceLastUpdate)
{
  if (!this->shaftLink || !this->bodyLink)
    return;

  if (this->shaftJoint)
    return;

  this->shaftLink->SetAutoDisable(false);
  this->shaftJoint = this->parent->GetWorld()->GetPhysicsEngine()->CreateJoint(
      "revolute", this->parent);

  this->shaftJoint->Attach(this->shaftLink, this->bodyLink);
  this->shaftJoint->Load(this->shaftLink, this->bodyLink, math::Pose::Zero);
  {
    std::stringstream jointNameStream;
    jointNameStream << this->bodyLinkName + "_" <<
        this->shaftLinkName << "_joint";
    this->shaftJoint->SetName(jointNameStream.str());
  }
  this->shaftJoint->SetAxis(0, math::Vector3(0, 0 ,1));
  this->shaftJoint->SetHighStop(0, -1000000);
  this->shaftJoint->SetLowStop(0, 1000000);

  this->shaftJoint->Init();
}
