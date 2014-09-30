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

#include <boost/any.hpp>

#include <gazebo/common/Plugin.hh>
#include <gazebo/transport/transport.hh>

#include "Variant.pb.h"
#include "SimpleModel.pb.h"
#include "SimpleConnection.pb.h"

typedef const boost::shared_ptr<const Simple_msgs::msgs::SimpleConnection>
    ConstSimpleConnectionPtr;

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

    /// \brief Get a value of a property by key
    /// \param _key String key
    /// \return The value of the property
    public: boost::any GetProperty(const std::string &_key);

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    protected: virtual void Load(sdf::ElementPtr _sdf);

    /// \brief Load implementation to be overriden by subclasses.
    /// \param[in] _sdf The SDF of this plugin.
    protected: virtual void LoadImpl(sdf::ElementPtr _sdf);

    /// \brief Update the simple model plugin
    protected: void Update();

    /// \brief Implementation for updating the simple model plugin.
    /// \param[in] _timeSinceLastUpdate Time in seconds since last world update.
    protected: virtual void UpdateImpl(double _timeSinceLastUpdate);

    /// \brief Fill a message with the ports and properties of this model.
    /// \param[out] _msg A message to be filled with ports and properties of
    /// this model.
    protected: void FillMsg(Simple_msgs::msgs::SimpleModel &_msg);

    /// \brief Callback on a simple connection event.
    /// \param[in] _msg Message describing the simple connection.
    protected: void OnSimpleConnection(ConstSimpleConnectionPtr &_msg);

    /// \brief Callback on incoming port data.
    /// \param[in] _msg Message describing the port data.
    //protected: void OnPortData(ConstVariantPtr &_msg);

    /// \brief Called when a request message is received.
    /// \param[in] _msg The request message.
    private: void OnRequest(ConstRequestPtr &_msg);

    /// \brief Process all received request messages.
    /// Must only be called from the World::ProcessMessages function.
    private: void ProcessRequestMsgs();

    /// \brief Publish simple model info on a separate thread. Blocks until
    /// there is at least a subscriber.
    private: void InitThread();

    /// \brief Process simple connection messages.
    private: void ProcessSimpleConnectionMsgs();

    /// \brief Type of model
    protected: std::string schematicType;

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
    //protected: std::vector<std::string> ports;
    protected: std::map<std::string, boost::any> ports;

    /// \brief A list of properties associated with this model.
    protected: std::map<std::string, std::string> properties;

    /// \brief Subscribe to model connection updates
    private: transport::SubscriberPtr simpleConnectionSub;

    /// \brief Mutex to protect incoming message buffers.
    private: boost::recursive_mutex *receiveMutex;

    /// \brief Pointer to the parent model.
    private: physics::ModelPtr parent;

    /// \brief A thread that publishes a simple model info on init.
    private: boost::thread *initThread;

    /// \brief A connection the update event
    private: event::ConnectionPtr updateConnection;

    /// \brief SimTime of last update.
    private: double timeOfLastUpdate;

    /// \brief Mutex to protect reading and writing to ports.
    private: boost::recursive_mutex *portMutex;

    /// \brief Mutex for managing connections between simple components.
    private: boost::recursive_mutex *simpleConnectionMutex;

    /// \def SimpleConnectionMsgs_L
    /// \brief List of simple connection messages.
    typedef std::list<boost::shared_ptr<
        Simple_msgs::msgs::SimpleConnection const> > SimpleConnectionMsgs_L;

    /// \brief List of simple connection message to process.
    private: SimpleConnectionMsgs_L simpleConnectionMsgs;

    protected: std::map<std::string, transport::PublisherPtr> portPubs;

    protected: std::map<std::string, std::string> portTopics;

    //protected: std::map<std::string, transport::SubscriberPtr> portSubs;
  };
}

#endif
