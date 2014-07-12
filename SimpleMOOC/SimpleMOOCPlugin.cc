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

#include "SimpleMOOCPlugin.hh"


using namespace gazebo;
using namespace std;

SimpleMOOCPlugin::SimpleMOOCPlugin()
  :node(new gazebo::transport::Node())
{
  
}

SimpleMOOCPlugin::~SimpleMOOCPlugin()
{
  //if (this->userCam)
  //  this->userCam->EnableSaveFrame(false);
  node.reset();
  sub.reset();
}

void SimpleMOOCPlugin::Init()
{
  std::cerr << "Simple MOOC server plugin Init()" <<  std::endl;
  // setup our node for communication
  node->Init();
  pub = node->Advertise<SimpleMOOC_msgs::msgs::LoginResponse>("~/MOOCResponse");
  sub = node->Subscribe("~/MOOC", &SimpleMOOCPlugin::OnLoginRequest, this);  
}

void SimpleMOOCPlugin::Load(int /*_argc*/, char ** /*_argv*/)
{
  std::cerr << "Simple MOOC server plugin Load()" <<  std::endl;
}

void SimpleMOOCPlugin::OnLoginRequest(ConstLoginRequestPtr &_msg )
{
  cout << "SimpleMOOC login request: [";
  cout << _msg->url() << ", ";
  cout << _msg->username() << ", ";
  cout << _msg->password() << "]" << endl;
 
  try {
    restApi.Login(_msg->url().c_str(), _msg->username().c_str(), _msg->password().c_str() );
  }
  catch(MOOCException &x) {
    SimpleMOOC_msgs::msgs::LoginResponse msg;
    msg.set_success(false);
    std::string errorMsg ("There was a problem trying to login the Mentor2 Learning companion: ");
    errorMsg += x.what();
    msg.set_msg(errorMsg);
    // alert the user via the gui plugin
    pub->Publish(msg);
  }
}

// plugin registration
GZ_REGISTER_SYSTEM_PLUGIN(SimpleMOOCPlugin)

