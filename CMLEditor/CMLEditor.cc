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
#include <gazebo/gui/TimePanel.hh>
#include <gazebo/gui/MainWindow.hh>
#include <gazebo/gui/RenderWidget.hh>
#include <gazebo/gui/Actions.hh>
#include <gazebo/gui/model/ModelEditor.hh>

#include "CMLEvents.hh"
#include "CMLEditor.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditor::CMLEditor(MainWindow *_mainWindow)
  : mainWindow(_mainWindow)
{
  // Create the CML editor tab
//  this->CMLPalette = new CMLEditorPalette(_mainWindow);

  if (!_mainWindow)
  {
    gzerr << "Main window is NULL!" << std::endl;
    return;
  }

  std::cerr << " CML loading " << std::endl;

  ModelEditor *modelEditor =
      dynamic_cast<ModelEditor *>(_mainWindow->GetEditor("model"));

  if (!modelEditor)
  {
    gzerr << "Model Editor not found. Unable to start CMLEditor plugin"
        << std::endl;
    return;
  }
  // wiring button
  QPushButton *wiringButton = new QPushButton(tr("Wires"));
  wiringButton->setCheckable(false);
  wiringButton->setChecked(false);
  connect(wiringButton, SIGNAL(clicked()), this,
      SLOT(OnElectricalConnection()));

  modelEditor->AddItemToPalette(wiringButton, "Wiring");
  std::cerr << " got model editor! " << std::endl;


}

/////////////////////////////////////////////////
CMLEditor::~CMLEditor()
{
}

/////////////////////////////////////////////////
void CMLEditor::OnElectricalConnection()
{
  emit CMLEvents::createConnection("electrical");
}
