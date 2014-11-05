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

#ifndef _WHEEL_PLUGIN_HH_
#define _WHEEL_PLUGIN_HH_

#include "SimpleModelPlugin.hh"

typedef const boost::shared_ptr<const Simple_msgs::msgs::Variant>
    ConstVariantPtr;

namespace gazebo
{
  class WheelPlugin : public SimpleModelPlugin
  {
    /// \brief Constructor.
    public: WheelPlugin();

    /// \brief Destructor.
    public: ~WheelPlugin();

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    public: virtual void LoadImpl(sdf::ElementPtr _sdf);

    /// \brief Initialize the plugin.
    public: virtual void Init();

    // Documentation Inherited.
    protected: void UpdateImpl(double _timeSinceLastUpdate);

    /// \brief Callback when voltage is received.
    /// \param[in] _msg Message containing the input voltage value.
    private: void OnTorque(ConstVariantPtr &_msg);

    /// \brief Subcriber to the input torque topic.
    private: transport::SubscriberPtr torqueSub;

    /// \brief Joint to rotate.
    // private: gazebo::physics::JointPtr joint;

    /// \brief Joint to rotate.
    private: gazebo::physics::JointPtr shaftJoint;

    /// \brief External shaft link connected to this wheel.
    private: physics::LinkPtr shaftLink;

    /// \brief Link to be rotated by the shaft.
    private: physics::LinkPtr bodyLink;

    /// \brief Name of the external shaft link.
    private: std::string shaftLinkName;

    /// \brief Name of the link to be rotated.
    private: std::string bodyLinkName;
  };
}

#endif
