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

#include "SimpleConnectionsPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(SimpleConnectionsPlugin)


/////////////////////////////////////////////////
SimpleConnectionsPlugin::SimpleConnectionsPlugin()
{
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
}

/////////////////////////////////////////////////
void SimpleConnectionsPlugin::Update()
{
  if (!this->connectionsInit)
  {
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
        this->PublishConnections(source, sourcePort, target, targetPort);
        connectionElem = connectionElem->GetNextElement("connection");
      }
    }
    event::Events::DisconnectWorldUpdateBegin(this->updateConnection);
    this->updateConnection.reset();
    this->connectionsInit = true;
  }

}
