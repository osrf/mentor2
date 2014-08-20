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


#include <gazebo/common/common.hh>
#include <gazebo/gui/gui.hh>

#include "CMLItemListWidget.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLItemListWidget::CMLItemListWidget(QWidget *_parent)
    : QWidget(_parent)
{
  this->setObjectName("CMLItemListWidget");

  QVBoxLayout *mainLayout = new QVBoxLayout;

  this->nameLabel = new QLabel;
  mainLayout->addWidget(this->nameLabel);

  this->itemsSignalMapper = new QSignalMapper(this);
  connect(this->itemsSignalMapper, SIGNAL(mapped(QString)),
      this, SLOT(OnItemSelected(QString)));

  this->stackedWidget = new QStackedWidget(this);
  mainLayout->addWidget(this->stackedWidget);

  this->setLayout(mainLayout);
}

/////////////////////////////////////////////////
CMLItemListWidget::~CMLItemListWidget()
{
}

/////////////////////////////////////////////////
void CMLItemListWidget::SetCurrentItemList(const std::string &_name)
{
  this->nameLabel->setText(tr(_name.c_str()));

  if (this->itemsMap.find(_name) == this->itemsMap.end())
  {
    QWidget *itemWidget = new QWidget;
    QVBoxLayout *itemListlayout = new QVBoxLayout;
    itemWidget->setLayout(itemListlayout);
    this->itemsMap[_name] = itemWidget;
    this->stackedWidget->addWidget(itemWidget);
    this->stackedWidget->setCurrentWidget(itemWidget);
  }
  else
  {
    this->stackedWidget->setCurrentWidget(this->itemsMap[_name]);
  }
}

/////////////////////////////////////////////////
void CMLItemListWidget::Populate(const std::map<std::string, std::string>
    &_items)
{
  QWidget *widget = this->stackedWidget->currentWidget();
  if (!widget)
    return;

  std::map<std::string, std::string>::const_iterator it;
  for (it = _items.begin(); it != _items.end(); ++it)
  {
    QPushButton *itemButton = new QPushButton(tr(it->first.c_str()), this);
    itemButton->setCheckable(false);
    itemButton->setChecked(false);
    this->itemsSignalMapper->setMapping(itemButton, tr(it->second.c_str()));
    connect(itemButton, SIGNAL(clicked()),
        this->itemsSignalMapper, SLOT(map()));

    widget->layout()->addWidget(itemButton);
  }
}

/////////////////////////////////////////////////
unsigned int CMLItemListWidget::GetItemCount()
{
  QWidget *widget = this->stackedWidget->currentWidget();

  if (widget)
    return static_cast<unsigned int>(widget->layout()->count());

  return 0;
}

/////////////////////////////////////////////////
void CMLItemListWidget::OnItemSelected(QString _url)
{
  std::string path = std::string("model://") + _url.toStdString();
  std::string filename = common::ModelDatabase::Instance()->GetModelFile(path);
  gui::Events::createEntity("model", filename);
}
