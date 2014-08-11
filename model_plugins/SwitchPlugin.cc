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

#include "SwitchPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(SwitchPlugin)

/////////////////////////////////////////////////
SwitchPlugin::SwitchPlugin()
{
}

/////////////////////////////////////////////////
SwitchPlugin::~SwitchPlugin()
{
}

/////////////////////////////////////////////////
void SwitchPlugin::LoadImpl(sdf::ElementPtr _sdf)
{
}

/////////////////////////////////////////////////
void SwitchPlugin::Init()
{
  SimpleModelPlugin::Init();
}
