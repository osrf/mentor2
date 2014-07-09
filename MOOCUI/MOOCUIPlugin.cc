/*
 * Copyright 2014 Open Source Robotics Foundation
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

#include "gazebo/gui/MainWindow.hh"
#include "gazebo/gui/GuiIface.hh"
#include "gazebo/gui/GuiEvents.hh"

#include "MOOCUIWidget.hh"
#include "MOOCUIPlugin.hh"

using namespace gazebo;

MOOCUIPlugin::~MOOCUIPlugin()
{
  //if (this->userCam)
  //  this->userCam->EnableSaveFrame(false);
}

void MOOCUIPlugin::Load(int /*_argc*/, char ** /*_argv*/)
{
}

void MOOCUIPlugin::Init()
{
  // Connect to the sensor update event.
  this->connections.push_back(
      gui::Events::ConnectMainWindowReady(
      boost::bind(&MOOCUIPlugin::OnMainWindowReady, this)));

  std::cerr << " Hello World " <<  std::endl;

}

void MOOCUIPlugin::OnMainWindowReady()
{
  // Get a pointer to the active user camera
  //rendering::UserCameraPtr userCam;
  gui::MainWindow *mainWindow = gui::get_main_window();
  QMenu *menu = new QMenu(QString("&MOOC"));

  QAction* loginAct = new QAction(QString("&Login"), menu );
  loginAct->setStatusTip(QString("Login to Mentor 2 Learning Companion"));
  
  MOOCUIWidget *widget = new MOOCUIWidget(mainWindow);
  
  QObject::connect(loginAct, SIGNAL(triggered()), widget, SLOT(LoginMOOC()));

  
  menu->addAction(loginAct);
  mainWindow->AddMenu(menu);
}

// Register this plugin with the simulator
GZ_REGISTER_SYSTEM_PLUGIN(MOOCUIPlugin)



