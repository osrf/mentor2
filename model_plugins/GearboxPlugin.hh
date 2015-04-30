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

#ifndef _GEARBOX_PLUGIN_HH_
#define _GEARBOX_PLUGIN_HH_

#include <string>
#include "SimpleModelPlugin.hh"

typedef const boost::shared_ptr<const Simple_msgs::msgs::Variant>
    ConstVariantPtr;

namespace gazebo
{
  class GearboxPlugin : public SimpleModelPlugin
  {
    /// \brief Constructor.
    public: GearboxPlugin();

    /// \brief Destructor.
    public: virtual ~GearboxPlugin();

    /// \brief Load the model plugin.
    /// param[in] _sdf The SDF of this plugin.
    public: virtual void LoadImpl(sdf::ElementPtr _sdf);

    /// \brief Initialize the plugin.
    public: virtual void Init();

    /// Documentation Inherited
    protected: virtual void UpdateImpl(double _timeSinceLastUpdate);

    private: physics::JointPtr SpawnJoint(const std::string &_link1,
    const std::string &_link2, const std::string &_jointType,
    const math::Vector3 &_jointAxis);

    /// \brief Connected joint names.
    private: std::vector<std::string> joints;

    /// \brief The gearbox joint that will be created by this plugin.
    private: physics::JointPtr gearboxJoint;

    /// \brief Gear ratio.
    private: double gearRatio;

    /// \brief Gearbox efficiency.
    private: double efficiency;

    /// \brief Parent link name.
    private: std::string parentLinkName;

    /// \brief Child link name.
    private: std::string childLinkName;

    /// \brief Gearbox axis
    private: math::Vector3 gearboxAxis;
  };
}

#endif
