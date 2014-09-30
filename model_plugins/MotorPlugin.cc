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

#include "MotorPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(MotorPlugin)

/////////////////////////////////////////////////
MotorPlugin::MotorPlugin()
{
  this->voltage = 0;
  this->schematicType = "motor";
  this->backEmf = 0.0064;
  this->motorResistance = 14;
  this->torqueConstant = 0.0046;
}

/////////////////////////////////////////////////
MotorPlugin::~MotorPlugin()
{
}

/////////////////////////////////////////////////
void MotorPlugin::LoadImpl(sdf::ElementPtr _sdf)
{
}

/////////////////////////////////////////////////
void MotorPlugin::Init()
{
  SimpleModelPlugin::Init();
}

/////////////////////////////////////////////////
void MotorPlugin::UpdateImpl(double _timeSinceLastUpdate)
{
  if (!this->voltageSub)
  {
    if (this->portTopics.empty() ||
        this->portTopics.find("positive") == this->portTopics.end())
      return;

    this->voltageSub = this->node->Subscribe(this->portTopics["positive"],
        &MotorPlugin::OnVoltage, this);

    return;
  }

  // std::cout << "MOTOR update!" << std::endl;

  // get this value from the connectors
  double voltage = this->voltage; // in Volts

  double shaftRotationSpeed = 0.1; // in radians per seconds
  double emfVolt = this->backEmf * shaftRotationSpeed;
  double internalVoltage = voltage - emfVolt;
  double internalCurrent = internalVoltage / this->motorResistance;
  double torque = internalCurrent * this->torqueConstant;

}

/////////////////////////////////////////////////
void MotorPlugin::OnVoltage(ConstVariantPtr &_msg)
{
  this->voltage = _msg->v_double();
  //std::cerr << "receiving voltage " << this->voltage << std::endl;
}
