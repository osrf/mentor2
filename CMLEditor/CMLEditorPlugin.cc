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

#include <gazebo/gui/qt.h>
#include <gazebo/gui/GuiIface.hh>
#include <gazebo/gui/MainWindow.hh>
#include <gazebo/gui/GuiEvents.hh>
#include <gazebo/gui/Actions.hh>

#include "CMLEditor.hh"
#include "CMLManager.hh"
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
  // keep tracks of simple models in the world
  gui::CMLManager::Instance()->Init();


  this->connections.push_back(
      gui::Events::ConnectMainWindowReady(
      boost::bind(&CMLEditorPlugin::OnMainWindowReady, this)));
}

/////////////////////////////////////////////////
void CMLEditorPlugin::OnMainWindowReady()
{
  // gazebo main window
  gui::MainWindow *mainWindow = gui::get_main_window();

  // CML editor
  gui::CMLEditor *editor = new gui::CMLEditor(mainWindow);
}

// Register this plugin with the simulator
GZ_REGISTER_SYSTEM_PLUGIN(CMLEditorPlugin)
