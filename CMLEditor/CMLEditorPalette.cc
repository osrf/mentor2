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

#include "CMLEditorPalette.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditorPalette::CMLEditorPalette(QWidget *_parent)
    : QWidget(_parent)
{
  this->setObjectName("CMLEditorPalette");

  QVBoxLayout *mainLayout = new QVBoxLayout;

  this->componentTreeWidget = new QTreeWidget();
  this->componentTreeWidget->setColumnCount(1);
  this->componentTreeWidget->setIndentation(10);
  this->componentTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  this->componentTreeWidget->header()->hide();
  this->componentTreeWidget->setFocusPolicy(Qt::NoFocus);

  this->componentTreeWidget->setSelectionMode(QAbstractItemView::NoSelection);
  connect(this->componentTreeWidget,
      SIGNAL(itemClicked(QTreeWidgetItem *, int)),
      this, SLOT(OnItemSelection(QTreeWidgetItem *, int)));

  // components item
  QTreeWidgetItem *componentsItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0),
        QStringList(QString("Components")));
  this->componentTreeWidget->addTopLevelItem(componentsItem);

  /*QTreeWidgetItem *componentsChildItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0));
  componentsItem->addChild(componentsChildItem);*/

  // Shapes buttons
  QWidget *modelWidget = new QWidget(this);
  QGridLayout *componentsLayout = new QGridLayout;

  // cylinder button
  QPushButton *componentButton = new QPushButton(tr("ComponentA"), this);
  componentButton->setCheckable(true);
  componentButton->setChecked(false);
  //connect(componentButton, SIGNAL(clicked()), this, SLOT(OnCylinder()));

  componentsLayout->addWidget(componentButton, 0, 0);
  modelWidget->setLayout(componentsLayout);

  this->componentTreeWidget->setItemWidget(componentsItem, 0, modelWidget);
  componentsItem->setExpanded(true);

  /*// save buttons
  QPushButton *discardButton = new QPushButton(tr("Discard"));
  connect(discardButton, SIGNAL(clicked()), this, SLOT(OnDiscard()));

  this->saveButton = new QPushButton(tr("Save As"));
  connect(this->saveButton, SIGNAL(clicked()), this, SLOT(OnSave()));

  QPushButton *doneButton = new QPushButton(tr("Done"));
  connect(doneButton, SIGNAL(clicked()), this, SLOT(OnDone()));

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(discardButton);
  buttonsLayout->addWidget(this->saveButton);
  buttonsLayout->addWidget(doneButton);
  buttonsLayout->setAlignment(Qt::AlignCenter);*/

  mainLayout->addWidget(this->componentTreeWidget);

  this->setLayout(mainLayout);
  this->layout()->setContentsMargins(0, 0, 0, 0);

/*  this->saved = false;
  this->saveLocation = QDir::homePath().toStdString();
  this->modelName = "default";*
}

/////////////////////////////////////////////////
CMLEditorPalette::~CMLEditorPalette()
{
}
