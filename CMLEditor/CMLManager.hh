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

#include "SimpleModel.pb.h"

namespace gazebo
{
  namespace gui
  {
    /// \class CMLEditor CMLEditor.hh gui/gui.hh
    /// \brief Interface to the CML editor.
    class CMLManager : public SingletonT<CMLManager>
    {
      /// \brief Constructor
      private: CMLManager();

      /// \brief Destructor
      private: virtual ~CMLManager();

      /// \brief Initialize the manager
      public: void Init();

      /// \brief Initialize the manager
      public: SimpleModel_msgs::msgs::SimpleModel
          GetModelInfo(const std::string &_name);

      /// \brief Callback when a response message is received.
      private: void OnResponse(ConstResponsePtr &_msg);

      /// \brief Transport node.
      private: transport::NodePtr node;

      /// \brief Subscriber for response messages.
      private: transport::SubscriberPtr responseSub;

      /// \brief Publisher for request messages.
      private: transport::PublisherPtr requestPub;

      /// \brief A request message to retrieve all simple models.
      private: msgs::Request *requestMsg;

      /// \brief A map of model name to a message with its properties.
      private: std::map<std::string, SimpleModel_msgs::msgs::SimpleModel>
          modelInfo;

      /// \brief Mutex to protect the model info map.
      private: boost::recursive_mutex *modelInfoMutex;

      /// \brief A flag to indicate if the manager is initialized or not.
      private: bool initialized;

      /// \brief This is a singleton class.
      private: friend class SingletonT<CMLManager>;
    };
  }
}
#endif
