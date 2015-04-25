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
#ifndef _CML_MANAGER_HH_
#define _CML_MANAGER_HH_

#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/gui/qt.h>

#include "SimpleModel.pb.h"

typedef const boost::shared_ptr<const Simple_msgs::msgs::SimpleModel>
    ConstSimpleModelPtr;

namespace gazebo
{
  namespace gui
  {
    class CMLComponentInspector;
    class CMLPropertyManager;

    /// \class CMLManager CMLManager.hh gui/gui.hh
    /// \brief CML simple model manager.
    class CMLManager : public SingletonT<CMLManager>
    {
      /// \brief Constructor
      private: CMLManager();

      /// \brief Destructor
      private: virtual ~CMLManager();

      /// \brief Initialize the manager
      public: void Init();

      /// \brief Get data of a component
      /// \param[in] _name Node of the component
      public: Simple_msgs::msgs::SimpleModel GetModelInfo(
          const std::string &_name);

      public: void UpdateModelInfo(const std::string &_name,
          Simple_msgs::msgs::SimpleModel _info, bool _publish = true);

      /// \brief Add a simple model to the manager
      /// \param[in] _msg Simple model message describing the model.
      public: void AddSimpleModel(Simple_msgs::msgs::SimpleModel _msg);

      /// \brief Remove a simple model from the manager
      /// \param[in] _name Name of the simple model.
      public: void RemoveSimpleModel(const std::string &_name);

      /// \brief Show an inspector for configuring component properties.
      /// \param[in] _Name of component.
      /// \return True if the component exists.
      public: bool ShowInspector(const std::string &_name);

      /// \brief Callback when a response message is received.
      /// \param[in] _mgs Pointer to the response message.
      private: void OnResponse(ConstResponsePtr &_msg);

      /// \brief Callback when a simple model message is received.
      /// \param[in] _msg Pointer to the simple model message.
      private: void OnSimpleModel(ConstSimpleModelPtr &_msg);

      /// \brief Callback when a request message is received.
      /// \param[in] _msg The request message.
      private: void OnRequest(ConstRequestPtr &_msg);

      /// \brief Qt callback when component properties have changed property.
      private slots: void OnComponentProperyChanged();

      /// \brief Transport node.
      private: transport::NodePtr node;

      /// \brief Subscriber for simple model messages.
      private: transport::SubscriberPtr simpleModelSub;

      /// \brief Publisher for simple model messages.
      private: transport::PublisherPtr simpleModelPub;

      /// \brief Subscriber for response messages.
      private: transport::SubscriberPtr responseSub;

      /// \brief Publisher for request messages.
      private: transport::PublisherPtr requestPub;

      /// \brief Subscriber for request messages.
      private: transport::SubscriberPtr requestSub;

      /// \brief A request message to retrieve all simple models.
      private: msgs::Request *requestMsg;

      /// \brief A map of model name to a message with its properties.
      private: std::map<std::string, Simple_msgs::msgs::SimpleModel>
          modelInfo;

      /// \brief Mutex to protect the model info map.
      private: boost::recursive_mutex *modelInfoMutex;

      /// \brief A flag to indicate if the manager is initialized or not.
      private: bool initialized;

      /// \brief Manager for simple model properties.
      private: CMLPropertyManager *propertyManager;

      /// \brief This is a singleton class.
      private: friend class SingletonT<CMLManager>;
    };
  }
}
#endif
