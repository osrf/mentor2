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
  requestQThread(boost::thread( boost::bind(&SimpleMOOCPlugin::RunRequestQ, this))) 
{
  
}

SimpleMOOCPlugin::~SimpleMOOCPlugin()
{
  //if (this->userCam)
  //  this->userCam->EnableSaveFrame(false);
  node.reset();
  sub.reset();
  // tell the requestQ to stop precessing
  stopMsgProcessing = true;
  if(this->requestQThread.joinable()) {
    this->requestQThread.join();
  }
  cout << "SimpleMOOCPlugin::~SimpleMOOCPlugin()" << endl;
}

void SimpleMOOCPlugin::Init()
{
  std::cerr << "Simple MOOC server plugin Init()" <<  std::endl;
  // setup our node for communication
  node->Init();
  pub = node->Advertise<SimpleMOOC_msgs::msgs::RestResponse>("~/MOOCResponse");
  sub = node->Subscribe("~/MOOCRequest", &SimpleMOOCPlugin::OnRestRequest, this);  


}

void SimpleMOOCPlugin::Load(int /*_argc*/, char ** /*_argv*/)
{
  std::cerr << "Simple MOOC server plugin Load()" <<  std::endl;
}

void SimpleMOOCPlugin::OnRestRequest(ConstRestRequestPtr &_msg )
{
  cout << "SimpleMOOC queuing Request: [";
  cout << _msg->url() << ", ";
  cout << _msg->username() << ", ";
  cout << _msg->password() << "]" << endl;
  {
    boost::mutex::scoped_lock lock(this->requestQMutex);
    msgRequestQ.push_back(_msg);
  }

}


void SimpleMOOCPlugin::RunRequestQ()
{
  cout << "SimpleMOOCPlugin::RunRequestQ THREAD started" << endl;
  while (!stopMsgProcessing) {
    gazebo::common::Time::MSleep(50);
    try{
      boost::shared_ptr<const SimpleMOOC_msgs::msgs::RestRequest> req;
      // Grab the mutex and remove first message in the queue
      {
        boost::mutex::scoped_lock lock(this->requestQMutex);
        if(!msgRequestQ.empty())
        {
          req = msgRequestQ.front();
          msgRequestQ.pop_front();
        }
      }

      if(req)
      {
        // if the password field is set, then this is a login attempt
        try
        {
          std::string resp;
          if( req->password().length() > 0)
          {
            cout << "PROCESSING... " << endl;
            resp = restApi.Login(req->url().c_str(), req->username().c_str(), req->password().c_str());
          }
          else
          {
            resp = restApi.Request(req->url().c_str());
          }
          // send the response back to the client
          SimpleMOOC_msgs::msgs::RestResponse msg;
          msg.set_success(true);
          msg.set_resp(resp.c_str());
          cout << "SUCCESS ... publishing to MOOCUI" << endl;
          this->pub->Publish(msg);          
        }
        catch(MOOCException &x)
        {
          SimpleMOOC_msgs::msgs::RestResponse msg;
          msg.set_success(false);
          std::string errorMsg ("There was a problem trying to communicate with the Mentor2 Learning companion: ");
          errorMsg += x.what();
          msg.set_msg(errorMsg);
          // alert the user via the gui plugin
          cerr << "ERROR in request... publising to MOOCUI" << endl;
          this->pub->Publish(msg);
        }
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

