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


#include "SimpleModelPlugin.hh"


using namespace gazebo;

/////////////////////////////////////////////////
SimpleModelPlugin::SimpleModelPlugin()
{
  this->receiveMutex = new boost::recursive_mutex();
}

/////////////////////////////////////////////////
SimpleModelPlugin::~SimpleModelPlugin()
{
  delete this->receiveMutex;
}

/////////////////////////////////////////////////
void SimpleModelPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
}

/////////////////////////////////////////////////
void SimpleModelPlugin::Load(sdf::ElementPtr _sdf)
{
  if (_sdf->HasElement("port"))
  {
    sdf::ElementPtr childElem = _sdf->GetElement("port");
    while (childElem)
    {
      std::string name = childElem->Get<std::string>("name");
      this->ports.push_back(name);
      childElem = childElem->GetNextElement("port");
    }
  }
  if (_sdf->HasElement("properties"))
  {
    sdf::ElementPtr ports = _sdf->GetElement("properties");
    sdf::ElementPtr childElem = ports->GetFirstElement();
    while (childElem)
    {
      std::string key = childElem->GetName();
      std::string value = childElem->GetValue()->GetAsString();
      this->properties[key] = value;
      childElem = childElem->GetNextElement("");
    }
  }

  // DEBUG
  /*for (unsigned int i = 0 ; i < this->ports.size(); ++i)
    std::cerr << " got port " << this->ports[i] << std::endl;

  std::map<std::string, std::string>::iterator it;
  for (it = properties.begin() ; it != properties.end(); ++it)
    std::cerr << " got property " << it->first << ": " <<
        it->second << std::endl;
  */
}

/////////////////////////////////////////////////
void SimpleModelPlugin::Init()
{
  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();

  this->requestSub = this->node->Subscribe("~/simple/request",
      &SimpleModelPlugin::OnRequest, this, true);

  this->responsePub =
      this->node->Advertise<msgs::Response>("~/simple/response");
}

//////////////////////////////////////////////////
void SimpleModelPlugin::OnRequest(ConstRequestPtr &_msg)
{
  boost::recursive_mutex::scoped_lock lock(*this->receiveMutex);
  this->requestMsgs.push_back(*_msg);
}

//////////////////////////////////////////////////
void SimpleModelPlugin::ProcessRequestMsgs()
{
  boost::recursive_mutex::scoped_lock lock(*this->receiveMutex);
  msgs::Response response;

  std::list<msgs::Request>::iterator iter;
  for (iter = this->requestMsgs.begin();
       iter != this->requestMsgs.end(); ++iter)
  {
    bool send = true;
    response.set_id((*iter).id());
    response.set_request((*iter).request());
    response.set_response("success");

    if ((*iter).request() == "entity_info")
    {

    }

    if (send)
    {
      this->responsePub->Publish(response);
    }
  }

  this->requestMsgs.clear();
}
