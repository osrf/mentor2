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

    /// \brief Callback when voltage is received on connector0.
    /// \param[in] _msg Message containing the input voltage value.
    private: void OnConnector0Voltage(ConstVariantPtr &_msg);

    /// \brief Callback when voltage is received on connector 1.
    /// \param[in] _msg Message containing the input voltage value.
    private: void OnConnector1Voltage(ConstVariantPtr &_msg);

    /// \brief Back-EMF (Electromotive Force).
    private: double backEmf;

    /// \brief Motor resistance in ohms.
    private: double motorResistance;

    /// \brief Torque constant in  N*m/A
    private: double torqueConstant;

    /// \brief Subcriber to the input voltage topic on connector0.
    private: transport::SubscriberPtr voltageConnector0Sub;

    /// \brief Subcriber to the input voltage topic on connector1.
    private: transport::SubscriberPtr voltageConnector1Sub;

    /// \brief Publisher of torque values on the ~/motor/torque topic.
    private: transport::PublisherPtr torquePub;

    /// \brief Mutex to protect voltage variable.
    private: boost::recursive_mutex *voltageMutex;

    /// \brief Joint to rotate.
    private: gazebo::physics::JointPtr joint;

    /// \brief Link of the motor shaft.
    private: gazebo::physics::LinkPtr shaftLink;

    /// \brief Name of the shaft joint.
    private: std::string shaftJointName;

    /// \brief Input voltage.
    private: double voltage;
  };
}

#endif
