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
  :node(new gazebo::transport::Node()),
  stopMsgProcessing(false),
  requestQThread(NULL)
{
  
}

SimpleMOOCPlugin::~SimpleMOOCPlugin()
{
  //if (this->userCam)
  //  this->userCam->EnableSaveFrame(false);
  node.reset();
  subEvent.reset();
  subRequest.reset();
  // tell the requestQ to stop precessing
  stopMsgProcessing = true;
  if(this->requestQThread->joinable()) {
    this->requestQThread->join();
    delete this->requestQThread;
  }
  cout << "SimpleMOOCPlugin::~SimpleMOOCPlugin()" << endl;
}

void SimpleMOOCPlugin::Init()
{
  std::cerr << "SimpleMOOCPlugin::Init() setting up pubs/sub node" <<  std::endl;
  // setup our node for communication
  node->Init();
  subRequest = node->Subscribe("~/event/rest_login", &SimpleMOOCPlugin::OnRestLoginRequest, this);
  subEvent = node->Subscribe("~/event/rest_post", &SimpleMOOCPlugin::OnEventRestPost, this);
  requestQThread = new boost::thread( boost::bind(&SimpleMOOCPlugin::RunRequestQ, this));
}

void SimpleMOOCPlugin::Load(int /*_argc*/, char ** /*_argv*/)
{
  std::cerr << "Simple MOOC server plugin Load()" <<  std::endl;
  
}

void SimpleMOOCPlugin::OnEventRestPost(ConstRestPostPtr &_msg)
{
  cout << "SimpleMOOCPlugin::OnRestPost";
  cout << "[" << _msg->route() << ", " << _msg->json() << "]"  << endl; 
  cout << endl;

  try
  {
    restApi.PostLearningEvent(_msg->route().c_str(), _msg->json().c_str());
  }
  catch(MOOCException &x)
  {
    Event_msgs::msgs::RestError msg;
    std::string errorMsg ("There was a problem trying to send data to the MOOC: ");
    errorMsg += x.what();
    msg.set_type("Error");
    msg.set_msg(errorMsg);
    // alert the user via the gui plugin
    cerr << "ERROR in request... publising to MOOCUI: " << errorMsg << endl;
    this->pub->Publish(msg);
  }

}

void SimpleMOOCPlugin::OnRestLoginRequest(ConstRestLoginPtr &_msg )
{
  cout << "SimpleMOOC queuing Request: [";
  cout << _msg->url() << ", ";
  cout << _msg->username() << ", ";
  cout << _msg->password() << "]" << endl;
  {
    boost::mutex::scoped_lock lock(this->requestQMutex);
    msgLoginQ.push_back(_msg);
  }

}


void SimpleMOOCPlugin::ProcessLoginRequest(ConstRestLoginPtr _msg)
{
 // this is executed asynchronously
 try
  {
    std::string resp;
    resp = restApi.Login(_msg->url().c_str(), "/login", _msg->username().c_str(), _msg->password().c_str());
  }
  catch(MOOCException &x)
  {
    Event_msgs::msgs::RestError msg;
    std::string errorMsg ("There was a problem trying to login the web server: ");
    errorMsg += x.what();
    msg.set_type("Error");
    msg.set_msg(errorMsg);
    // alert the user via the gui plugin
    cerr << "ERROR in request... publising to MOOCUI: " << errorMsg << endl;
    this->pub->Publish(msg);
  } 
}

void SimpleMOOCPlugin::ProcessMOOCEvent(ConstRestPostPtr _msg)
{
  // this is executed asynchronously
  try
  {
    restApi.PostLearningEvent(_msg->route().c_str(), _msg->json().c_str());
  }
  catch(MOOCException &x)
  {
    Event_msgs::msgs::RestError msg;
    std::string errorMsg ("There was a problem trying to post data to the web server: ");
    errorMsg += x.what();
    msg.set_type("Error");
    msg.set_msg(errorMsg);
    // alert the user via the gui plugin
    cerr << "ERROR POSTING to MOOC: " << errorMsg << endl;
    this->pub->Publish(msg);
  } 
}

void SimpleMOOCPlugin::RunRequestQ()
{
  
  // be ready to send errors back to the UI
  cout << "SimpleMOOCPlugin::RunRequestQ THREAD started" << endl;
  pub = node->Advertise<Event_msgs::msgs::RestError>("~/event/rest_error");

  // process any login or post data that ha been received
  while (!stopMsgProcessing) {
    gazebo::common::Time::MSleep(50);
    try{
      boost::shared_ptr<const Event_msgs::msgs::RestLogin> login;
      boost::shared_ptr<const Event_msgs::msgs::RestPost> post;
      // Grab the mutex and remove first message in each queue
      {
        boost::mutex::scoped_lock lock(this->requestQMutex);
        if(!msgLoginQ.empty())
        {
          login = msgLoginQ.front();
          msgLoginQ.pop_front();
        }
        if(!msgEventQ.empty())
        {
          post = msgEventQ.front();
          msgEventQ.pop_front();
        }
      }

      if(login)
      {
        this->ProcessLoginRequest(login);
      }
      if(post)
      {
        this->ProcessMOOCEvent(post);
      }
    }
    catch(...) {
      cerr << "Unhandled exception while processing request message" << endl;
    }
    
  }
  cout << "SimpleMOOCPlugin::RunRequestQ THREAD started" << endl;
}


// plugin registration
GZ_REGISTER_SYSTEM_PLUGIN(SimpleMOOCPlugin)

