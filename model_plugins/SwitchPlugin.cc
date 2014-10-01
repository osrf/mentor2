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
#include "SwitchPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(SwitchPlugin)

/////////////////////////////////////////////////
SwitchPlugin::SwitchPlugin()
{
  this->schematicType = "switch";
  this->closed = false;
  this->connector0Dirty = false;
  this->connector1Dirty = false;
}

/////////////////////////////////////////////////
SwitchPlugin::~SwitchPlugin()
{
  delete this->connector0Mutex;
  delete this->connector1Mutex;
}

/////////////////////////////////////////////////
void SwitchPlugin::LoadImpl(sdf::ElementPtr _sdf)
{
  std::string closedStr = this->properties["closed"];
  this->closed = closedStr == "true" ? true : false;

  if (_sdf->HasElement("switch_joint"))
  {
    std::string jointName = _sdf->Get<std::string>("switch_joint");
    this->switchJoint = this->parent->GetJoint(jointName);
    if (!this->switchJoint)
    {
      gzerr << "Joint: '" + jointName + "' for switch: '" +
          this->parent->GetName() + "' not found" << std::endl;
      return;
    }

    this->switchHigh  = this->switchJoint->GetHighStop(0).Radian();
    this->switchLow   = this->switchJoint->GetLowStop(0).Radian();

    double jointCenter = (this->switchHigh + this->switchLow) / 2.0;
    double jointDeadbandPercent = 0.02;
    this->switchHigh = jointCenter +
      (1 - jointDeadbandPercent) * (this->switchHigh - jointCenter);
    this->switchLow = jointCenter +
      (1 - jointDeadbandPercent) * (this->switchLow - jointCenter);
  }

  double switchForce = 0.2;
  this->switchPID.Init(30, 0, 0, 0, 0, switchForce, -switchForce);


  if (this->closed)
    this->switchCmd = this->switchLow;
  else
    this->switchCmd = this->switchHigh;
}

/////////////////////////////////////////////////
void SwitchPlugin::Init()
{
  SimpleModelPlugin::Init();

  this->connector0Mutex = new boost::recursive_mutex();
  this->connector1Mutex = new boost::recursive_mutex();
}

/////////////////////////////////////////////////
void SwitchPlugin::UpdateImpl(double _timeSinceLastUpdate)
{
  // check in ports
  if (!this->connector0Sub)
  {

//    if (this->portTopics.empty())
//      return;

    std::map<std::string, std::string>::iterator it =
        this->portTopics.find("connector0");

    if (it != this->portTopics.end())
    {
      this->connector0Sub =
          this->node->Subscribe(this->portTopics["connector0"],
          &SwitchPlugin::OnConnector0, this);
    }
  }
  if (!this->connector1Sub)
  {
//    if (this->portTopics.empty())
//      return;

    std::map<std::string, std::string>::iterator it =
        this->portTopics.find("connector1");

    if (it != this->portTopics.end())
    {
      this->connector1Sub =
          this->node->Subscribe(this->portTopics["connector1"],
          &SwitchPlugin::OnConnector1, this);
    }
  }

  // switch control logic
  double switchHysteresis = 0.2;
  double switchCmdEps = 0.01;
  double switchState = this->switchJoint->GetAngle(0).Radian();
  double switchPercent = math::clamp((switchState - this->switchLow) /
      (this->switchHigh-this->switchLow), 0.0, 1.0);

  if (this->switchCmd < (this->switchLow + switchCmdEps) &&
      switchPercent > (0.5 + switchHysteresis))
  {
    this->closed = false;
    this->switchCmd = this->switchHigh;
    std::cerr << "switch set to false" << std::endl;
  }
  else if (this->switchCmd > (this->switchHigh - switchCmdEps) &&
      switchPercent < (0.5 - switchHysteresis))
  {
    this->closed = true;
    this->switchCmd = this->switchLow;
    std::cerr << "switch set to true" << std::endl;
  }

  // PID (position) switch
  double switchError = switchState - this->switchCmd;
  double switchPIDCmd = this->switchPID.Update(switchError,
      _timeSinceLastUpdate);
  this->switchJoint->SetForce(0, switchPIDCmd);


  // publish
  if (!this->portPubs.empty())
  {
    // forward the messages
    if (this->portPubs.find("connector0") != this->portPubs.end())
    {
      boost::recursive_mutex::scoped_lock lock(*this->connector0Mutex);
      if (this->connector1Dirty)
      {
        if (!this->closed &&
            this->connector1Msg.type() == Simple_msgs::msgs::Variant::DOUBLE)
          this->connector1Msg.set_v_double(0);

        this->portPubs["connector0"]->Publish(this->connector1Msg);
        this->connector1Dirty = false;
      }
    }
    if (this->portPubs.find("connector1") != this->portPubs.end())
    {
      boost::recursive_mutex::scoped_lock lock(*this->connector1Mutex);
      if (this->connector0Dirty)
      {
        if (!this->closed &&
            this->connector0Msg.type() == Simple_msgs::msgs::Variant::DOUBLE)
          this->connector0Msg.set_v_double(0);
        this->portPubs["connector1"]->Publish(this->connector0Msg);
        this->connector0Dirty = false;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void SwitchPlugin::OnConnector0(ConstVariantPtr &_msg)
{
  boost::recursive_mutex::scoped_lock lock(*this->connector0Mutex);
  this->connector0Dirty = true;
  this->connector0Msg = *_msg;
}

////////////////////////////////////////////////////////////////////////////////
void SwitchPlugin::OnConnector1(ConstVariantPtr &_msg)
{
  boost::recursive_mutex::scoped_lock lock(*this->connector1Mutex);
  this->connector1Dirty = true;
  this->connector1Msg = *_msg;
}
