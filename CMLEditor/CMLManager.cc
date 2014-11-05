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

#include <gazebo/transport/transport.hh>
#include <gazebo/gui/gui.hh>

#include "CMLPropertyManager.hh"
#include "CMLComponentInspector.hh"

#include "CMLManager.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLManager::CMLManager()
{
  this->initialized = false;
  this->propertyManager = new CMLPropertyManager;
  this->requestMsg = NULL;
}

/////////////////////////////////////////////////
CMLManager::~CMLManager()
{
 this->requestPub.reset();
 this->responseSub.reset();
 this->node.reset();

 delete this->requestMsg;
 this->requestMsg = NULL;

 delete this->modelInfoMutex;
 this->modelInfoMutex = NULL;

 delete this->propertyManager;
 this->propertyManager = NULL;
}

/////////////////////////////////////////////////
void CMLManager::Init()
{
  if (this->initialized)
    return;

  this->modelInfoMutex = new boost::recursive_mutex();

  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();

  this->requestPub =
      this->node->Advertise<msgs::Request>("~/simple/request");

  this->responseSub = this->node->Subscribe("~/simple/response",
      &CMLManager::OnResponse, this);

  this->simpleModelSub = this->node->Subscribe("~/simple/model/info",
      &CMLManager::OnSimpleModel, this);

  this->simpleModelPub =
      this->node->Advertise<Simple_msgs::msgs::SimpleModel>(
      "~/simple/model/modify");

  this->requestMsg = msgs::CreateRequest("entity_info", "");
  this->requestPub->Publish(*this->requestMsg);

  this->requestSub = this->node->Subscribe("~/request",
    &CMLManager::OnRequest, this);

  this->initialized = true;
}

/////////////////////////////////////////////////
Simple_msgs::msgs::SimpleModel CMLManager::GetModelInfo(
    const std::string &_name)
{
  boost::recursive_mutex::scoped_lock lock(*this->modelInfoMutex);
  if (this->modelInfo.find(_name) != this->modelInfo.end())
    return this->modelInfo[_name];
  else
  {
    // return empty msg for now if nothing is found.
    Simple_msgs::msgs::SimpleModel empty;
    return empty;
  }
}

/////////////////////////////////////////////////
void CMLManager::UpdateModelInfo(const std::string &_name,
    Simple_msgs::msgs::SimpleModel _info, bool _publish)
{
  boost::recursive_mutex::scoped_lock lock(*this->modelInfoMutex);
  if (this->modelInfo.find(_name) != this->modelInfo.end())
    this->modelInfo[_name] = _info;

  if (_publish)
  {
    this->simpleModelPub->Publish(_info);
  }
}

/////////////////////////////////////////////////
void CMLManager::OnRequest(ConstRequestPtr &_msg)
{
  if (_msg->request() == "entity_delete")
  {
    if (this->modelInfo.find(_msg->data()) != this->modelInfo.end())
        this->modelInfo.erase(_msg->data());
  }
}

/////////////////////////////////////////////////
void CMLManager::OnResponse(ConstResponsePtr &_msg)
{
  std::cerr << " on response " << std::endl;

  if (!this->requestMsg || _msg->id() != this->requestMsg->id())
    return;

  Simple_msgs::msgs::SimpleModel simpleModelMsg;
  simpleModelMsg.ParseFromString(_msg->serialized_data());

  boost::recursive_mutex::scoped_lock lock(*this->modelInfoMutex);
  this->modelInfo[simpleModelMsg.name()] = simpleModelMsg;
}

/////////////////////////////////////////////////
void CMLManager::OnSimpleModel(ConstSimpleModelPtr &_msg)
{
  std::cerr << " on simple model " << _msg->name()
    << " " << _msg->schematic_type() << std::endl;

  // TODO ~/simple/model/info doesn't get messages so this doesn't get called.
  boost::recursive_mutex::scoped_lock lock(*this->modelInfoMutex);
  this->modelInfo[_msg->name()] = *_msg.get();
}

/////////////////////////////////////////////////
bool CMLManager::ShowInspector(const std::string &_name)
{
  boost::recursive_mutex::scoped_lock lock(*this->modelInfoMutex);
  if (this->modelInfo.find(_name) == this->modelInfo.end())
    return false;

  CMLComponentInspector *inspector = NULL;
  if (this->propertyManager->componentInspectors.find(_name)
      != this->propertyManager->componentInspectors.end())
  {
    inspector = this->propertyManager->componentInspectors[_name];
    inspector->UpdateFromMsg(&this->modelInfo[_name]);
    inspector->activateWindow();
    inspector->setFocus();
  }
  else
  {
    inspector = new CMLComponentInspector(gui::get_main_window());
    inspector->Load(&this->modelInfo[_name]);
    this->propertyManager->componentInspectors[_name] = inspector;
    QObject::connect(inspector, SIGNAL(Applied()),
        this->propertyManager, SLOT(OnComponentProperyChanged()));
  }
  inspector->show();
  return true;
}
