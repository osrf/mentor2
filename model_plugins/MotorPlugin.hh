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

#ifndef _MOTOR_PLUGIN_HH_
#define _MOTOR_PLUGIN_HH_

#include "SimpleModelPlugin.hh"

typedef const boost::shared_ptr<const Simple_msgs::msgs::Variant>
    ConstVariantPtr;

namespace gazebo
{
  class MotorPlugin : public SimpleModelPlugin
  {
    /// \brief Constructor.
    public: MotorPlugin();

    /// \brief Destructor.
    public: ~MotorPlugin();

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    public: virtual void LoadImpl(sdf::ElementPtr _sdf);

    /// \brief Initialize the plugin.
    public: virtual void Init();

    /// Documentation Inherited
    protected: virtual void UpdateImpl(double _timeSinceLastUpdate);

    /// \brief Callback when voltage is received.
    /// \param[in] _msg Message containing the input voltage value.
    private: void OnVoltage(ConstVariantPtr &_msg);

    /// \brief
    private: double lastSimTimei;

    private: double backEmf;

    /// \brief
    private: double motorResistance;

    /// \brief Torque constant in  N*m/A
    private: double torqueConstant;

    /// \brief Subcriber to the input voltage topic.
    private: transport::SubscriberPtr voltageSub;

    /// \brief Input voltage.
    private: double voltage;
  };
}

#endif
