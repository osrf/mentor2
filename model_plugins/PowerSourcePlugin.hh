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

#ifndef _POWER_SOURCE_PLUGIN_HH_
#define _POWER_SOURCE_PLUGIN_HH_

#include "SimpleModelPlugin.hh"

namespace gazebo
{
  class PowerSourcePlugin : public SimpleModelPlugin
  {
    /// \brief Constructor.
    public: PowerSourcePlugin();

    /// \brief Destructor.
    public: ~PowerSourcePlugin();

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    public: virtual void LoadImpl(sdf::ElementPtr _sdf);

    /// \brief Initialize the plugin.
    public: virtual void Init();

    /// \brief update
    protected: void UpdateImpl(double _timeSinceLastUpdate);

    /// \brief Charge (Ampere-hour)
    private: double capacity;

    /// \brief nominal voltage (Volts)
    private: double voltage;

    /// \brief current draw
    private: double current;

    /// \brief keep track of time between discharges
    private: double timeAccum;

    /// \brief true if the battery is discharging
    private: bool discharging;
  };
}

#endif
