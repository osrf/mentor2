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

#include <gazebo/gazebo.hh>
#include "SimpleMOOC.pb.h"


typedef const boost::shared_ptr<const SimpleMOOC_msgs::msgs::LoginRequest> ConstLoginRequestPtr;


namespace gazebo
{

  class SimpleMOOCPlugin : public SystemPlugin
  {
    /// \brief ctor
    public: SimpleMOOCPlugin();

    /// \brief dtor
    public: virtual ~SimpleMOOCPlugin();

    /// \brief Plugin Load
    public: virtual void Load(int /*_argc*/, char ** /*_argv*/);

    /// \brief  called everytime a message is received.
    public: void OnLoginRequest(ConstLoginRequestPtr &_msg );
    
    /// \brief Plugin initialization
    private: virtual void Init();
    
    /// \brief Gazebo pub/sub node
    private: gazebo::transport::NodePtr node;
    
    /// \brief Gazebo subscriber
    private: gazebo::transport::SubscriberPtr sub;

    /// \brief Gazebo publisher
    private: gazebo::transport::PublisherPtr pub;

    /// \brief Gazebo events
    private: std::vector<event::ConnectionPtr> connections;
  };

}


