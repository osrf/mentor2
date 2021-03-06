/*
 * Copyright (C) 2015 Open Source Robotics Foundation
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

#include <sdf/sdf.hh>
#include <gazebo/physics/PhysicsTypes.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/World.hh>

#include "SimpleConnectionsPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(SimpleConnectionsPlugin)


/////////////////////////////////////////////////
SimpleConnectionsPlugin::SimpleConnectionsPlugin()
{
  this->pubReady = false;
}

/////////////////////////////////////////////////
SimpleConnectionsPlugin::~SimpleConnectionsPlugin()
{
}

/////////////////////////////////////////////////
void SimpleConnectionsPlugin::Load(physics::ModelPtr _model,
    sdf::ElementPtr _sdf)
{
  this->parent = _model;
  GZ_ASSERT(this->parent, "Parent model is NULL");

  this->sdf = _sdf;

  if (this->sdf->HasElement("connection"))
  {
    std::string modelName = this->parent->GetScopedName();
    sdf::ElementPtr connectionElem = this->sdf->GetElement("connection");
    while (connectionElem)
    {
      std::string source = connectionElem->Get<std::string>("source");
      std::string sourcePort =
          connectionElem->Get<std::string>("source_port");

      std::string target = connectionElem->Get<std::string>("target");
      std::string targetPort =
          connectionElem->Get<std::string>("target_port");

      source = modelName + "::" + source;
      target = modelName + "::" + target;
      SimpleConnection data;
      data.source = source;
      data.sourcePort = sourcePort;
      data.target = target;
      data.targetPort = targetPort;
      this->connectionData.push_back(data);
      //this->PublishConnections(source, sourcePort, target, targetPort);
      connectionElem = connectionElem->GetNextElement("connection");
    }
  }

  this->connectionsInit = false;

  // Listen to the update event. This event is broadcast every
  // simulation iteration.
  this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      boost::bind(&SimpleConnectionsPlugin::Update, this));
}

/////////////////////////////////////////////////
void SimpleConnectionsPlugin::Init()
{
  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();

  this->connectionPub =
      this->node->Advertise<Simple_msgs::msgs::SimpleConnection>(
      "~/simple/connection");

  this->initThread = new boost::thread(
      boost::bind(&SimpleConnectionsPlugin::InitThread, this));
}

/////////////////////////////////////////////////
void SimpleConnectionsPlugin::InitThread()
{
  this->connectionPub->WaitForConnection();
  this->pubReady = true;
}

/////////////////////////////////////////////////
void SimpleConnectionsPlugin::PublishConnections(const std::string &_parent,
    const std::string &_parentPort, const std::string &_child,
    const std::string &_childPort)
{
  Simple_msgs::msgs::SimpleConnection msg;
  msg.set_parent(_parent);
  msg.set_child(_child);
  msg.set_parent_port(_parentPort);
  msg.set_child_port(_childPort);
  this->connectionPub->Publish(msg);
  std::cerr <<  " pub conn " <<_parent << "::" << _parentPort << " ||to || " <<
      _child << "::" << _childPort << std::endl;
}

/////////////////////////////////////////////////
void SimpleConnectionsPlugin::Update()
{
  if (!this->pubReady)
    return;

  if (!this->connectionsInit)
  {
    auto it = this->connectionData.begin();
    while (it != this->connectionData.end())
    {
      SimpleConnection data = *it;
      if (this->parent->GetWorld()->GetModel(data.source) &&
          this->parent->GetWorld()->GetModel(data.target))
      {
        this->PublishConnections(data.source, data.sourcePort,
            data.target, data.targetPort);
        it = this->connectionData.erase(it);
      }
      else
          ++it;
    }

    if (this->connectionData.empty())
    {
      event::Events::DisconnectWorldUpdateBegin(this->updateConnection);
      this->updateConnection.reset();
      this->connectionsInit = true;
    }
  }

}
