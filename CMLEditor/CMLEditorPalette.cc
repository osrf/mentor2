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


#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtvariantproperty.h"

#include "BreadCrumbWidget.hh"
#include "CMLEvents.hh"
#include "CMLEditorPalette.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditorPalette::CMLEditorPalette(QWidget *_parent)
    : QWidget(_parent)
{
  this->setObjectName("CMLEditorPalette");

  this->propMutex = new boost::mutex();

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
  /*QTreeWidgetItem *shapesItem =
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
  shapesItem->setExpanded(true);*/

  // components
  QTreeWidgetItem *allModelItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0),
        QStringList(QString("Components")));
  this->componentTreeWidget->addTopLevelItem(allModelItem);

  QTreeWidgetItem *allModelChildItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0));
  allModelItem->addChild(allModelChildItem);

  // all model buttons
  QWidget *allModelWidget = new QWidget(this);
  QGridLayout *allModelLayout = new QGridLayout;

  // electrical button
  QPushButton *electricalButton = new QPushButton(tr("Electrical"), this);
  electricalButton->setCheckable(false);
  electricalButton->setChecked(false);
  connect(electricalButton, SIGNAL(clicked()), this,
      SLOT(OnElecticalComponent()));

  // mechanical button
  QPushButton *mechanicalButton = new QPushButton(tr("Mechanical"), this);
  mechanicalButton->setCheckable(false);
  mechanicalButton->setChecked(false);
  connect(mechanicalButton, SIGNAL(clicked()), this,
      SLOT(OnMechanicalComponent()));

  allModelLayout->addWidget(electricalButton, 0, 0);
  allModelLayout->addWidget(mechanicalButton, 1, 0);
  allModelWidget->setLayout(allModelLayout);

  this->componentTreeWidget->setItemWidget(
      allModelChildItem, 0, allModelWidget);
  allModelItem->setExpanded(true);



  // Wiring and Connecting item
  this->wiringConnectingItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0),
        QStringList(QString("Wiring && Connecting")));
  this->componentTreeWidget->addTopLevelItem(this->wiringConnectingItem);

  QTreeWidgetItem *wiringConnectingChildItem =
    new QTreeWidgetItem(static_cast<QTreeWidgetItem*>(0));
  this->wiringConnectingItem->addChild(wiringConnectingChildItem);

  // electrical component buttons
  QWidget *wiringConnectingWidget = new QWidget(this);
  QGridLayout *wiringConnectingLayout = new QGridLayout;

  // wiring button
  QPushButton *wiringButton = new QPushButton(tr("Wires"), this);
  wiringButton->setCheckable(false);
  wiringButton->setChecked(false);
  connect(wiringButton, SIGNAL(clicked()), this,
      SLOT(OnElectricalConnection()));

  wiringConnectingLayout->addWidget(wiringButton, 0, 0);
  wiringConnectingWidget->setLayout(wiringConnectingLayout);

  this->componentTreeWidget->setItemWidget(
      wiringConnectingChildItem, 0, wiringConnectingWidget);
  this->wiringConnectingItem->setExpanded(true);


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







  BreadCrumbWidget *breadCrumbWidget = new BreadCrumbWidget;

  // property browser for the simple components
/*  this->variantManager = new QtVariantPropertyManager();
  this->propTreeBrowser = new QtTreePropertyBrowser();
  this->propTreeBrowser->setObjectName("simplePropTreeBrowser");
  this->variantFactory = new QtVariantEditorFactory();
  this->propTreeBrowser->setFactoryForManager(this->variantManager,
                                              this->variantFactory);
  connect(this->variantManager,
          SIGNAL(propertyChanged(QtProperty*)),
          this, SLOT(OnPropertyChanged(QtProperty *)));
  connect(this->propTreeBrowser,
          SIGNAL(currentItemChanged(QtBrowserItem*)),
          this, SLOT(OnCurrentPropertyChanged(QtBrowserItem *)));*/



  this->componentSubLevelWidget = new QWidget;
  QWidget *propTreeBrowserStub = new QWidget;



  QSplitter *splitter = new QSplitter(Qt::Vertical);
  splitter->addWidget(breadCrumbWidget);
  splitter->addWidget(this->componentSubLevelWidget);
  splitter->addWidget(propTreeBrowserStub);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 2);
  splitter->setStretchFactor(2, 2);
  splitter->setCollapsible(0, false);
  splitter->setCollapsible(1, false);
  splitter->setCollapsible(2, false);

  this->componentInfoFrame = new QFrame;
  QVBoxLayout *frameLayout = new QVBoxLayout;
  frameLayout->addWidget(splitter);
  frameLayout->setContentsMargins(0, 0, 0, 0);
  this->componentInfoFrame->setLayout(frameLayout);

  this->pageStackWidget = new QStackedWidget(this);
  this->pageStackWidget->addWidget(this->componentTreeWidget);
  this->pageStackWidget->addWidget(this->componentInfoFrame);
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

/////////////////////////////////////////////////
void CMLEditorPalette::OnCurrentPropertyChanged(QtBrowserItem *_item)
{
  if (_item)
    this->selectedProperty = _item->property();
  else
    this->selectedProperty = NULL;
}

/////////////////////////////////////////////////
void CMLEditorPalette::OnPropertyChanged(QtProperty *_item)
{
  boost::mutex::scoped_try_lock lock(*this->propMutex);
  if (!lock)
    return;

  if (this->selectedProperty != _item /*|| this->fillingPropertyTree*/)
    return;

  /*QTreeWidgetItem *currentItem = this->componentTreeWidget->currentItem();

  if (!currentItem)
    return;

  if (this->wiringConnectingItem->indexOfChild(currentItem) != -1)
    this->LightPropertyChanged(_item);
  else if (currentItem == this->sceneItem)
    this->ScenePropertyChanged(_item);
  else if (currentItem == this->physicsItem)
    this->PhysicsPropertyChanged(_item);*/
}

/////////////////////////////////////////////////
void CMLEditorPalette::OnElecticalComponent()
{
  this->pageStackWidget->setCurrentWidget(this->componentInfoFrame);
}

/////////////////////////////////////////////////
void CMLEditorPalette::OnMechanicalComponent()
{
  this->pageStackWidget->setCurrentWidget(this->componentInfoFrame);
}
