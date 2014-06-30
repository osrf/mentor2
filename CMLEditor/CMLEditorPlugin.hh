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

#include <gazebo/gui/qt.hh>
#include <gazebo/gui/GuiIface.hh>
#include "CMLEditorPlugin.hh"

using namespace gazebo;

/////////////////////////////////////////////////
CMLEditorPlugin::~CMLEditorPlugin()
{
}

/////////////////////////////////////////////////
void CMLEditorPlugin::Load(int /*_argc*/, char ** /*_argv*/)
{
}

/////////////////////////////////////////////////
void CMLEditorPlugin::Init()
{

}

// Register this plugin with the simulator
GZ_REGISTER_SYSTEM_PLUGIN(CMLEditorPlugin)
