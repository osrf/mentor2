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

#include "CMLEditorWidget.hh"
#include "CMLEditorPalette.hh"
#include "CMLEditor.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditor::CMLEditor(MainWindow *_mainWindow)
  : mainWindow(_mainWindow)
//  : Editor(_mainWindow)
{
  // Create the CML editor tab
//  this->CMLPalette = new CMLEditorPalette(_mainWindow);
//  this->Init("CMLEditorTab", "CML Editor", this->CMLPalette);

  RenderWidget *renderWidget = this->mainWindow->GetRenderWidget();
  this->CMLEditorWidget = new gazebo::gui::CMLEditorWidget(renderWidget);
  this->CMLEditorWidget->setSizePolicy(QSizePolicy::Expanding,
      QSizePolicy::Expanding);
  this->CMLEditorWidget->Init();

  renderWidget->InsertWidget(0, this->CMLEditorWidget);
  this->CMLEditorWidget->hide();

  if (g_editModelAct)
    connect(g_editModelAct, SIGNAL(toggled(bool)), this, SLOT(OnEdit(bool)));
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
    RenderWidget *renderWidget = this->mainWindow->GetRenderWidget();
    this->CMLEditorWidget->show();

    TimePanel *timePanel = renderWidget->GetTimePanel();
    timePanel->ShowSimTime(!_checked);
    timePanel->ShowRealTimeFactor(!_checked);
    timePanel->ShowStepWidget(!_checked);
    timePanel->ShowIterations(!_checked);

//    this->jointAct->setVisible(_checked);
//    this->jointTypeAct->setVisible(_checked);
//    this->jointSeparatorAct->setVisible(_checked);
  }

 /* if (_checked)
  {
//    this->mainWindow->AddLeftColumnTab("CMLEditor", "default",
//        this->CMLPalette);
    this->mainWindow->ShowLeftColumnWidget("CMLEditorTab");
  }
  else
  {
    this->mainWindow->ShowLeftColumnWidget();
//    this->mainWindow->RemoveLeftColumnTab("CMLEditor", "default");
  }*/
}
