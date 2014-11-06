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

#ifndef _SWITCH_PLUGIN_HH_
#define _SWITCH_PLUGIN_HH_

#include <gazebo/common/PID.hh>

#include "SimpleModelPlugin.hh"

typedef const boost::shared_ptr<const Simple_msgs::msgs::Variant>
    ConstVariantPtr;

namespace gazebo
{
  class SwitchPlugin : public SimpleModelPlugin
  {
    /// \brief Constructor.
    public: SwitchPlugin();

    /// \brief Destructor.
    public: ~SwitchPlugin();

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    public: virtual void LoadImpl(sdf::ElementPtr _sdf);

    /// \brief Initialize the plugin.
    public: virtual void Init();

    /// Documentation Inherited
    protected: virtual void UpdateImpl(double _timeSinceLastUpdate);

    /// \brief Callback when a connector0 message is received.
    private: void OnConnector0(ConstVariantPtr &_msg);

    /// \brief Callback when a connector1 message is received.
    private: void OnConnector1(ConstVariantPtr &_msg);

    /// \brief Mutex to protect connector 0 messages.
    private: boost::recursive_mutex *connector0Mutex;

    /// \brief Mutex to protect connector 1 messages.
    private: boost::recursive_mutex *connector1Mutex;

    /// \brief Joint low stop
    private: double switchLow;

    /// \brief Joint high stop
    private: double switchHigh;

    /// \brief A revolute joint for the swtich
    private: physics::JointPtr switchJoint;

    /// \brief Position to set the swtich to.
    private: double switchCmd;

    /// \brief PID controller for making a bi-stable switch
    private: common::PID switchPID;

    /// \brief True if the switch is closed.
    private: bool closed;

    /// \brief Subscriber to the connector 1 topic.
    private: transport::SubscriberPtr connector0Sub;

    /// \brief Subscriber to the connector 1 topic.
    private: transport::SubscriberPtr connector1Sub;

    private: Simple_msgs::msgs::Variant connector0Msg;

    private: Simple_msgs::msgs::Variant connector1Msg;

    /// \brief Flag to keep track of new connector0 messages.
    private: bool connector0Dirty;

    /// \brief Flag to keep track of new connector1 messages.
    private: bool connector1Dirty;

    /// \brief Time when the switch state was changed.
    private: common::Time fnrSwitchTime;
  };
}

#endif
