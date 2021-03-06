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

#include "Variant.pb.h"
#include "PowerSourcePlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(PowerSourcePlugin)


/////////////////////////////////////////////////
PowerSourcePlugin::PowerSourcePlugin()
{
  this->schematicType = "power";
  this->voltage = 0;
  this->timeAccum = 0;
}


/////////////////////////////////////////////////
PowerSourcePlugin::~PowerSourcePlugin()
{
}


/////////////////////////////////////////////////
void PowerSourcePlugin::LoadImpl(sdf::ElementPtr _sdf)
{
  this->discharging = true;
  this->capacity = this->GetProperty<double>("capacity");
  this->voltage = this->GetProperty<double>("voltage");
  this->current = this->GetProperty<double>("current");
  this->Reset();
}


/////////////////////////////////////////////////
void PowerSourcePlugin::Init()
{
  SimpleModelPlugin::Init();
}

/////////////////////////////////////////////////
void PowerSourcePlugin::Reset()
{
  this->remainingCapacity = this->capacity;
  this->currentVoltage = this->voltage;
  this->timeAccum = 0;
}

/////////////////////////////////////////////////
void PowerSourcePlugin::UpdateImpl(double _timeSinceLastUpdate)
{
  double timeSlice = 1.0; // discharge every second
  this->timeAccum += _timeSinceLastUpdate;

  if(this->timeAccum > 1.0)
  {
    this->timeAccum -= 1.0;
  }
  else
  {
    // too early for update
    return;
  }

  if (this->remainingCapacity > 0.0)
  {
    if(this->discharging)
    {
      double ampHours = this->current * timeSlice / 3600.0;
      this->remainingCapacity -= ampHours;
      double minVoltage = this->voltage*0.75;
      double voltageRange = this->voltage - minVoltage;
      this->currentVoltage = minVoltage +
          (this->remainingCapacity / this->capacity) * voltageRange;
      if (this->remainingCapacity < 0.0)
      {
        this->remainingCapacity = 0;
        this->currentVoltage = 0;
      }

      static int loop = 10;
      loop --;
      if (loop <0)
      {
        loop = 10;
//        std::cout << "battery capacity " << this->remainingCapacity
//            << std::endl;
//        std::cout << "battery voltage " << this->currentVoltage
//            << std::endl;
      }
    }
  }

  if (!this->portPubs.empty())
  {    Simple_msgs::msgs::Variant msg;
    msg.set_type(Simple_msgs::msgs::Variant::DOUBLE);
    msg.set_v_double(this->currentVoltage);

    if (this->portPubs.find("positive") != this->portPubs.end() &&
        this->portPubs.find("negative") != this->portPubs.end())
    {
      this->portPubs["positive"]->Publish(msg);
    }
  }
}
