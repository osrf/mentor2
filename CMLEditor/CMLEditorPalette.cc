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

#include "CMLEvents.hh"
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

  // -----
  // simple shapes item
  QTreeWidgetItem *shapesItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0),
        QStringList(QString("Simple Shapes")));
  this->componentTreeWidget->addTopLevelItem(shapesItem);

  QTreeWidgetItem *shapesChildItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0));
  shapesItem->addChild(shapesChildItem);

  // Shapes buttons
  QWidget *shapesWidget = new QWidget(this);
  QGridLayout *shapesLayout = new QGridLayout;

  // box button
  QPushButton *boxButton = new QPushButton(tr("Box"), this);
  boxButton->setCheckable(false);
  boxButton->setChecked(false);
  //connect(componentButton, SIGNAL(clicked()), this, SLOT(OnCylinder()));

  shapesLayout->addWidget(boxButton, 0, 0);
  shapesWidget->setLayout(shapesLayout);

  this->componentTreeWidget->setItemWidget(
      shapesChildItem, 0, shapesWidget);
  shapesItem->setExpanded(true);

  // -----

  // electical item
  QTreeWidgetItem *electricalItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0),
        QStringList(QString("Electrical")));
  this->componentTreeWidget->addTopLevelItem(electricalItem);

  QTreeWidgetItem *electricalChildItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0));
  electricalItem->addChild(electricalChildItem);

  // Shapes buttons
  QWidget *electricalWidget = new QWidget(this);
  QGridLayout *electricalLayout = new QGridLayout;

  // wiring button
  QPushButton *wiringButton = new QPushButton(tr("Wiring && Connecting"), this);
  wiringButton->setCheckable(false);
  wiringButton->setChecked(false);
  connect(wiringButton, SIGNAL(clicked()), this,
      SLOT(OnElectricalConnection()));

  electricalLayout->addWidget(wiringButton, 0, 0);
  electricalWidget->setLayout(electricalLayout);

  this->componentTreeWidget->setItemWidget(
      electricalChildItem, 0, electricalWidget);
  electricalItem->setExpanded(true);


  /*// save buttons
  QPushButton *discardButton = new QPushButton(tr("Discard"));
  connect(discardButton, SIGNAL(clicked()), this, SLOT(OnDiscard()));

  this->saveButton = new QPushButton(tr("Save As"));
  connect(this->saveButton, SIGNAL(clicked()), this, SLOT(OnSave()));

  QPushButton *doneButton = new QPushButton(tr("Done"));
  connect(doneButton, SIGNAL(clicked()), this, SLOT(OnDone()));

  QHBoxLayout *navigationLayout = new QHBoxLayout;
  navigationLayout->addWidget(discardButton);
  navigationLayout->addWidget(this->saveButton);
  navigationLayout->addWidget(doneButton);
  navigationLayout->setAlignment(Qt::AlignCenter);*/


/*  QFrame *frame = new QFrame;
  QVBoxLayout *frameLayout = new QVBoxLayout;
  frameLayout->addWidget(this->componentTreeWidget, 0);
  frameLayout->setContentsMargins(0, 0, 0, 0);
  frame->setLayout(frameLayout);*/




  this->pageStackWidget = new QStackedWidget(this);
  this->pageStackWidget->addWidget(this->componentTreeWidget);

  mainLayout->addWidget(this->pageStackWidget);


  /*
  this->backButton = new QPushButton(tr("Back"));
  connect(this->backButton, SIGNAL(clicked()), this, SLOT(OnBack()));
  this->backButton->setEnabled(false);

  //QPushButton *homeButton = new QPushButton(tr("Home"));
  //connect(homeButton, SIGNAL(clicked()), this, SLOT(OnHome()));

  QHBoxLayout *navigationLayout = new QHBoxLayout;
  navigationLayout->addWidget(this->backButton);
  //navigationLayout->addWidget(homeButton);
  navigationLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(navigationLayout);*/

  this->setLayout(mainLayout);
  this->layout()->setContentsMargins(0, 0, 0, 0);

/*  this->saved = false;
  this->saveLocation = QDir::homePath().toStdString();
  this->modelName = "default";*/
}

/////////////////////////////////////////////////
CMLEditorPalette::~CMLEditorPalette()
{
}

/////////////////////////////////////////////////
void CMLEditorPalette::OnBack()
{
  int index = this->pageStackWidget->currentIndex();
  if (index > 0)
  {
    if (--index == 0)
      this->backButton->setEnabled(false);
    this->pageStackWidget->setCurrentIndex(index);
  }
}

/////////////////////////////////////////////////
void CMLEditorPalette::OnItemSelection(QTreeWidgetItem *_item, int /*_column*/)
{
  if (_item && _item->childCount() > 0)
    _item->setExpanded(!_item->isExpanded());
}

/////////////////////////////////////////////////
void CMLEditorPalette::OnElectricalConnection()
{
  emit CMLEvents::createConnection("electrical");
}
