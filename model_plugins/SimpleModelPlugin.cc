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

#include <boost/lexical_cast.hpp>
#include <gazebo/physics/physics.hh>

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
  this->parent = _model;
  GZ_ASSERT(this->parent, "Parent model is NULL");

  this->Load(_sdf);
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

  this->LoadImpl(_sdf);

  // DEBUG
  for (unsigned int i = 0 ; i < this->ports.size(); ++i)
    std::cerr << " got port " << this->ports[i] << std::endl;

  std::map<std::string, std::string>::iterator it;
  for (it = properties.begin() ; it != properties.end(); ++it)
    std::cerr << " got property " << it->first << ": " <<
        it->second << std::endl;

}

/////////////////////////////////////////////////
void SimpleModelPlugin::LoadImpl(sdf::ElementPtr _sdf)
{
  // This function should be mostly empty
}

/////////////////////////////////////////////////
void SimpleModelPlugin::Init()
{
  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();

  // currently just handles entity_info requests.
  this->requestSub = this->node->Subscribe("~/simple/request",
      &SimpleModelPlugin::OnRequest, this, true);

  this->responsePub =
      this->node->Advertise<msgs::Response>("~/simple/response");

  this->simpleModelPub =
      this->node->Advertise<Simple_msgs::msgs::SimpleModel>(
      "~/simple/model/info");

  // let subscribers know that a simple model has been spawned.
  Simple_msgs::msgs::SimpleModel simpleModelMsg;
  this->FillMsg(simpleModelMsg);
  this->simpleModelPub->Publish(simpleModelMsg);
}

//////////////////////////////////////////////////
void SimpleModelPlugin::OnRequest(ConstRequestPtr &_msg)
{
  {
    boost::recursive_mutex::scoped_lock lock(*this->receiveMutex);
    this->requestMsgs.push_back(*_msg);
  }

  // process here as ConnectWorldUpdateBegin doesn't work when world is paused
  this->ProcessRequestMsgs();
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
    response.set_id((*iter).id());
    response.set_request((*iter).request());
    response.set_response("success");

    if ((*iter).request() == "entity_info")
    {
      Simple_msgs::msgs::SimpleModel simpleModelMsg;
      this->FillMsg(simpleModelMsg);
      std::string *serializedData = response.mutable_serialized_data();
      simpleModelMsg.SerializeToString(serializedData);
      response.set_type(simpleModelMsg.GetTypeName());
      this->responsePub->Publish(response);
    }
  }

  this->requestMsgs.clear();
}

//////////////////////////////////////////////////
void SimpleModelPlugin::FillMsg(Simple_msgs::msgs::SimpleModel &_msg)
{
  _msg.set_name(this->parent->GetScopedName());
  for (unsigned int i =0; i < this->ports.size(); ++i)
  {
    _msg.add_port(this->ports[i]);
  }

  std::map<std::string, std::string>::iterator it;
  for (it = this->properties.begin() ; it != this->properties.end(); ++it)
  {
    _msg.add_key(it->first);
    Simple_msgs::msgs::Variant *property = _msg.add_value();

    std::string value = it->second;
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    if (value == "true")
    {
      property->set_type(Simple_msgs::msgs::Variant::BOOL);
      property->set_v_bool(true);
    }
    else if (value == "false")
    {
      property->set_type(Simple_msgs::msgs::Variant::BOOL);
      property->set_v_bool(false);
    }
    else
    {
      try
      {
        double value = boost::lexical_cast<double>(it->second);
        property->set_type(Simple_msgs::msgs::Variant::DOUBLE);
        property->set_v_double(value);
      }
      catch (const boost::bad_lexical_cast &)
      {
        property->set_type(Simple_msgs::msgs::Variant::STRING);
        property->set_v_string(it->second);
      }
    }
    //_msg.add_value(it->second);
  }

}
