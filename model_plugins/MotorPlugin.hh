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

#ifndef _MOTOR_PLUGIN_HH_
#define _MOTOR_PLUGIN_HH_

#include "SimpleModelPlugin.hh"

namespace gazebo
{
  class MotorPlugin : public SimpleModelPlugin
  {
    /// \brief Constructor.
    public: MotorPlugin();

    /// \brief Destructor.
    public: ~MotorPlugin();

    /// \brief Load the model plugin.
    /// param[in] _model Parent model the plugin is attached to.
    /// param[in] _sdf The sdf parameters for this plugin.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

    /// \brief Initialize the plugin.
    public: virtual void Init();

  };
}

#endif
