/*
 * Copyright 2014 Open Source Robotics Foundation
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

#include <gazebo/gui/qt.h>
#include <gazebo/gazebo.hh>

#include "SimpleMOOC.pb.h"
#include "MOOCLoginDialog.hh"

typedef const boost::shared_ptr<const SimpleMOOC_msgs::msgs::LoginResponse> ConstLoginResponsePtr;

namespace gazebo
{
  class MOOCUIWidget : public QWidget
  {
    Q_OBJECT

    /// \brief ctor
    public: MOOCUIWidget(QWidget *_parent = 0);
    /// \brief dtor
    public: virtual ~MOOCUIWidget();

    /// \brief QT callback (MOOC/Login menu) 
    public slots: void LoginMOOC();

    /// \brief pub/sub node to communicate with gzserver
    private: gazebo::transport::NodePtr node;

    /// \brief Gazebo topics publisher
    private: gazebo::transport::PublisherPtr pub;

    // \brief Gazebo topics subscriber
    private: gazebo::transport::SubscriberPtr sub;

    /// \brief called everytime a login response  message is received.
    private: void OnLoginResponse(ConstLoginResponsePtr &_msg);   
 
    /// \brief login dialog
    gui::MOOCLoginDialog dialog;
    // private: bool isLoggedIn;
  };
}

