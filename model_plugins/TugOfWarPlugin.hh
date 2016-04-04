/*
 * Copyright (C) 2016 Open Source Robotics Foundation
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

#ifndef _TUGOFWAR_PLUGIN_HH_
#define _TUGOFWAR_PLUGIN_HH_

#include <string>

#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo
{
  class TugOfWarPlugin : public ModelPlugin
  {
    /// \brief Constructor.
    public: TugOfWarPlugin();

    /// \brief Destructor.
    public: virtual ~TugOfWarPlugin();

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

    /// \brief Initialize the plugin.
    public: virtual void Init();

    /// \brief Reset the plugin.
    public: virtual void Reset();

    /// \brief Update the plugin
    protected: void Update();

    /// \brief Link to be connected for tug of war
    private: std::string towLinkName;

   /// \brief The fixed joint that will be created by this plugin.
    private: physics::JointPtr towJoint;

   /// \brief The link that the fixed joint will be attached to.
    private: physics::LinkPtr towLink;

   /// \brief The model that the fixed joint will be attached to.
    private: physics::ModelPtr towModel;

    /// \brief Pointer to the model.
    public: physics::ModelPtr model;

    /// \brief A connection the update event
    private: event::ConnectionPtr updateConnection;
  };
}

#endif
