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

#include "CMLItemListWidget.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLItemListWidget::CMLItemListWidget(QWidget *_parent)
    : QWidget(_parent)
{
  this->setObjectName("cmlComponentInfo");

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
void CMLItemListWidget::Populate(const std::vector<std::string> &_items)
{
  QWidget *widget = this->stackedWidget->currentWidget();
  if (!widget)
    return;

  for (unsigned int i = 0; i < _items.size(); ++i)
  {
    QPushButton *itemButton = new QPushButton(tr(_items[i].c_str()), this);
    itemButton->setCheckable(false);
    itemButton->setChecked(false);
    this->itemsSignalMapper->setMapping(itemButton, itemButton->text());
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
void CMLItemListWidget::OnItemSelected(QString _item)
{
}
