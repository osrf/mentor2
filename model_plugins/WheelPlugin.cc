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
}

/////////////////////////////////////////////////
void WheelPlugin::Init()
{
  SimpleModelPlugin::Init();
  this->torqueSub = this->node->Subscribe("~/motor/torque",
      &WheelPlugin::OnTorque, this);
  this->joint = this->parent->GetJoint("world_joint");
}

/////////////////////////////////////////////////
void WheelPlugin::OnTorque(ConstVariantPtr &_msg)
{
  // HACK: This is setting max force, while we are manually controlling the
  // max velocity
  this->joint->SetMaxForce(0, _msg->v_double());
//  this->joint->SetForce(0, _msg->v_double());

  if (_msg->v_double() > 0)
    this->joint->SetVelocity(0, 0.7);
  else
    this->joint->SetVelocity(0, 0.0);
}
