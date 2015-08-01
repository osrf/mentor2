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

typedef const boost::shared_ptr<const Simple_msgs::msgs::SimpleModel>
    ConstSimpleModelPtr;

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
    /// \param[in] _key String key
    /// \return The value of the property
    public: template<typename T>
            T GetProperty(const std::string &_key)
            {
              T result = T();

              if (!this->propertyMutex)
                return result;

              boost::recursive_mutex::scoped_lock lock(*this->propertyMutex);
              if (this->properties.find(_key) != this->properties.end())
              {
                try
                {
                  Simple_msgs::msgs::Variant valueMsg = this->properties[_key];
                  result = boost::any_cast<T>(this->ConvertVariant(valueMsg));
                }
                catch(...)
                {
                  return result;
                }
              }
              return result;
            }

    /// \brief Set a value of a property by key
    /// \param[in] _key String key
    /// \return The value of the property
    public: template<typename T>
            void SetProperty(const std::string &_key, const T &_value,
                bool _publish = true)
            {
              if (!this->propertyMutex)
                return;

              boost::recursive_mutex::scoped_lock lock(*this->propertyMutex);
              if (this->properties.find(_key) != this->properties.end())
              {
                Simple_msgs::msgs::Variant valueMsg = this->properties[_key];
                if (typeid(unsigned int) == typeid(_value))
                {
                  unsigned int value =
                      boost::lexical_cast<unsigned int>(_value);
                  valueMsg.set_v_uint32(value);
                  valueMsg.set_type(Simple_msgs::msgs::Variant::UINT32);
                }
                else if (typeid(int) == typeid(_value))
                {
                  int value = boost::lexical_cast<int>(value);
                  valueMsg.set_v_int32(value);
                  valueMsg.set_type(Simple_msgs::msgs::Variant::INT32);
                }
                else if (typeid(bool) == typeid(_value))
                {
                  bool value = boost::lexical_cast<bool>(_value);
                  valueMsg.set_v_bool(value);
                  valueMsg.set_type(Simple_msgs::msgs::Variant::BOOL);
                }
                else if (typeid(double) == typeid(_value))
                {
                  double value = boost::lexical_cast<double>(_value);
                  valueMsg.set_v_double(value);
                  valueMsg.set_type(Simple_msgs::msgs::Variant::DOUBLE);
                }
                else if (typeid(std::string) == typeid(_value))
                {
                  std::string value = boost::lexical_cast<std::string>(_value);
                  valueMsg.set_v_string(value);
                  valueMsg.set_type(Simple_msgs::msgs::Variant::STRING);
                }
                /*else if (typeid(Simple_msgs::msgs::Variant) == typeid(_value))
                {
                  valueMsg = _value;
                }*/
                this->properties[_key] = valueMsg;
                if (_publish && this->simpleModelPub)
                {
                  Simple_msgs::msgs::SimpleModel simpleModelMsg;
                  this->FillMsg(simpleModelMsg);
                  this->simpleModelPub->Publish(simpleModelMsg);
                }
              }

              if (this->sdfElements.find(_key) != this->sdfElements.end())
              {
                sdf::ElementPtr elem = this->sdfElements[_key];
                elem->Set<T>(_value);
              }
            }

    /// \brief Set the value of a property by key
    /// \param[in] _key String key
    /// \param[in] The value of the property
    public: void SetPropertyVariant(const std::string &_key,
        const Simple_msgs::msgs::Variant &_value);
    //public: void SetProperty(const std::string &_key, const boost::any &_value);

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    protected: virtual void Load(sdf::ElementPtr _sdf);

    /// \brief Load implementation to be overriden by subclasses.
    /// \param[in] _sdf The SDF of this plugin.
    protected: virtual void LoadImpl(sdf::ElementPtr _sdf);

    /// \brief Update the simple model plugin
    protected: void Update();

    /// \brief Convert a variant message to a boost any object.
    /// \param[in] _variant simple variant msg.
    /// \return boost any object.
    protected: boost::any ConvertVariant(Simple_msgs::msgs::Variant _variant);

    /// \brief Implementation for updating the simple model plugin.
    /// \param[in] _timeSinceLastUpdate Time in seconds since last world update.
    protected: virtual void UpdateImpl(double _timeSinceLastUpdate);

    /// \brief Fill a message with the ports and properties of this model.
    /// \param[out] _msg A message to be filled with ports and properties of
    /// this model.
    protected: void FillMsg(Simple_msgs::msgs::SimpleModel &_msg);

    /// \brief Callback on a simple model event.
    /// \param[in] _msg Message describing the simple model.
    protected: void OnSimpleModel(ConstSimpleModelPtr &_msg);

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

    /// \brief A thread that blocks and wait until there is at least a
    /// simple connection subscriber
    private: void InitThread();

    /// \brief Process incoming messages.
    private: void ProcessMsgs();

    /// \brief Type of model
    protected: std::string schematicType;

    /// \brief Subscriber to request messages.
    protected: transport::SubscriberPtr requestSub;

    /// \brief Publisher for request response messages.
    protected: transport::PublisherPtr responsePub;

    /// \brief Publisher for simple model messages.
    protected: transport::PublisherPtr simpleModelPub;

    /// \brief Subscriber to simple model messages.
    protected: transport::SubscriberPtr simpleModelSub;

    /// \brief Transportation node.
    protected: transport::NodePtr node;

    /// \brief Request message buffer.
    protected: std::list<msgs::Request> requestMsgs;

    /// \brief A list of ports associated with this model.
    //protected: std::vector<std::string> ports;
    protected: std::map<std::string, boost::any> ports;

    /// \brief A list of properties associated with this model.
    protected: std::map<std::string, Simple_msgs::msgs::Variant> properties;

        /// \brief A list of properties associated with this model.
    protected: std::map<std::string, sdf::ElementPtr> sdfElements;

    /// \brief Pointer to the parent model.
    protected: physics::ModelPtr parent;

    /// \brief Subscribe to model connection updates
    private: transport::SubscriberPtr simpleConnectionSub;

    /// \brief Mutex to protect incoming message buffers.
    private: boost::recursive_mutex *receiveMutex;

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

    /// \brief Mutex for protecting messages that simple models.
    private: boost::recursive_mutex *simpleModelMutex;

    /// \brief Mutex to protect properties.
    private: boost::recursive_mutex *propertyMutex;

    /// \def SimpleConnectionMsgs_L
    /// \brief List of simple connection messages.
    typedef std::list<boost::shared_ptr<
        Simple_msgs::msgs::SimpleConnection const> > SimpleConnectionMsgs_L;

    /// \brief List of simple connection message to process.
    private: SimpleConnectionMsgs_L simpleConnectionMsgs;

    /// \def SimpleModelMsgs_L
    /// \brief List of simple model messages.
    typedef std::list<boost::shared_ptr<
        Simple_msgs::msgs::SimpleModel const> > SimpleModelMsgs_L;

    /// \brief List of simple model message to process.
    private: SimpleModelMsgs_L simpleModelMsgs;

    /// \brief a map of port data publishers to their name .
    protected: std::map<std::string, transport::PublisherPtr> portPubs;

    /// \brief a map of port name to its associated topic name.
    protected: std::map<std::string, std::string> portTopics;

    //protected: std::map<std::string, transport::SubscriberPtr> portSubs;
  };
}

#endif
