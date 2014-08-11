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

#ifndef _SIMPLE_MODEL_PLUGIN_HH_
#define _SIMPLE_MODEL_PLUGIN_HH_

#include <gazebo/common/Plugin.hh>
#include <gazebo/transport/transport.hh>

#include "SimpleModel.pb.h"

namespace gazebo
{
  class SimpleModelPlugin : public ModelPlugin
  {
    /// \brief Constructor.
    public: SimpleModelPlugin();

    /// \brief Destructor.
    public: ~SimpleModelPlugin();

    /// \brief Load the model plugin.
    /// param[in] _model Parent model this plugin is attached to.
    /// param[in] _sdf The SDF of this plugin.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

    /// \brief Initialize the model plugin.
    public: virtual void Init();

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    protected: virtual void Load(sdf::ElementPtr _sdf);

    /// \brief Load implementation to be overriden by subclasses.
    /// \param[in] _sdf The SDF of this plugin.
    protected: virtual void LoadImpl(sdf::ElementPtr _sdf);

    /// \brief Fill a message with the ports and properties of this model.
    /// \param[out] _msg A message to be filled with ports and properties of
    /// this model.
    protected: void FillMsg(SimpleModel_msgs::msgs::SimpleModel &_msg);

    /// \brief Called when a request message is received.
    /// \param[in] _msg The request message.
    private: void OnRequest(ConstRequestPtr &_msg);

    /// \brief Process all received request messages.
    /// Must only be called from the World::ProcessMessages function.
    private: void ProcessRequestMsgs();

    /// \brief Subscriber to request messages.
    protected: transport::SubscriberPtr requestSub;

    /// \brief Publisher for request response messages.
    protected: transport::PublisherPtr responsePub;

    /// \brief Publisher for simple model messages.
    protected: transport::PublisherPtr simpleModelPub;

    /// \brief Transportation node.
    protected: transport::NodePtr node;

    /// \brief Request message buffer.
    protected: std::list<msgs::Request> requestMsgs;

    /// \brief A list of ports associated with this model.
    protected: std::vector<std::string> ports;

    /// \brief A list of properties associated with this model.
    protected: std::map<std::string, std::string> properties;

    /// \brief Mutex to protect incoming message buffers.
    private: boost::recursive_mutex *receiveMutex;

    /// \brief Pointer to the parent model.
    private: physics::ModelPtr parent;
  };
}

#endif
