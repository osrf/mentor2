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

#ifndef _SIMPLE_CONNECTIONS_PLUGIN_HH_
#define _SIMPLE_CONNECTIONS_PLUGIN_HH_

#include <gazebo/common/Events.hh>
#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/PhysicsTypes.hh>
#include <gazebo/transport/transport.hh>

#include "SimpleModel.pb.h"
#include "SimpleConnection.pb.h"


namespace gazebo
{
  class SimpleConnectionsPlugin : public ModelPlugin
  {
    /// \brief Constructor.
    public: SimpleConnectionsPlugin();

    /// \brief Destructor.
    public: ~SimpleConnectionsPlugin();

    /// \brief Load the model plugin.
    /// param[in] _model Parent model this plugin is attached to.
    /// param[in] _sdf The SDF of this plugin.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

    /// \brief Initialize the model plugin.
    public: virtual void Init();

    /// \brief Publish simple connection msgs.
    /// \param[in] _parent Parent of the connection.
    /// \param[in] _parentPort Parent port of the connection.
    /// \param[in] _child Child of the connection.
    /// \param[in] _childPort Child port of the connection.
    private: void PublishConnections(const std::string &_parent,
        const std::string &_parentPort, const std::string &_child,
        const std::string &_childPort);

    /// \brief Update the simple connections plugin
    private: void Update();

    /// \brief A connection the update event
    private: event::ConnectionPtr updateConnection;

    /// \brief Publisher for simple connection messages.
    private: transport::PublisherPtr connectionPub;

    /// \brief Transportation node.
    private: transport::NodePtr node;

    /// \brief sdf for this plugin element.
    private: sdf::ElementPtr sdf;

    /// \brief True if connections have been initialized.
    private: bool connectionsInit;

    /// \brief Pointer to parent model.
    private: physics::ModelPtr parent;
  };
}

#endif
