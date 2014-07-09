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
#include <gazebo/gui/MainWindow.hh>
#include <gazebo/gui/RenderWidget.hh>

#include "CMLEditorWidget.hh"
#include "CMLEditorPalette.hh"
#include "CMLEditor.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditor::CMLEditor(MainWindow *_mainWindow)
  : Editor(_mainWindow)
{
  // Create the CML editor tab
  this->CMLPalette = new CMLEditorPalette(_mainWindow);
  this->Init("CMLEditorTab", "CML Editor", this->CMLPalette);

//  connect(g_editBuildingAct, SIGNAL(toggled(bool)), this, SLOT(OnEdit(bool)));
//  this->menuBar = NULL;

  this->CMLEditorWidget = new gazebo::gui::CMLEditorWidget(
      this->mainWindow->GetRenderWidget());
  this->CMLEditorWidget->setSizePolicy(QSizePolicy::Expanding,
      QSizePolicy::Expanding);
//  this->CMLEditorWidget->hide();

  this->mainWindow->GetRenderWidget()->InsertWidget(0,
      this->CMLEditorWidget);

  this->OnEdit(true);
}

/////////////////////////////////////////////////
CMLEditor::~CMLEditor()
{
}

/////////////////////////////////////////////////
void CMLEditor::OnEdit(bool _checked)
{
  if (_checked)
  {
    this->mainWindow->Pause();
    this->mainWindow->ShowLeftColumnWidget("CMLEditorTab");
  }
  else
  {
    this->mainWindow->ShowLeftColumnWidget();
    this->mainWindow->Play();
  }
}
