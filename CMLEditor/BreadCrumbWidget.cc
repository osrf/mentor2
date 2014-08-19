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

#include "BreadCrumbWidget.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
BreadCrumbWidget::BreadCrumbWidget(QWidget *_parent)
    : QWidget(_parent)
{
  this->setObjectName("breadCrumbWidget");

  this->breadCrumbLayout = new QHBoxLayout;
  this->breadCrumbLayout->setAlignment(Qt::AlignLeft);

  QLabel *all = new QLabel(tr("All"));
  all->setTextFormat(Qt::RichText);
  all->setTextInteractionFlags(Qt::TextBrowserInteraction);
  all->setOpenExternalLinks(false);

  this->breadCrumbLayout->addWidget(all);

  this->currentIndex = 0;

  this->setLayout(this->breadCrumbLayout);
}

/////////////////////////////////////////////////
BreadCrumbWidget::~BreadCrumbWidget()
{
}


/////////////////////////////////////////////////
void BreadCrumbWidget::Push(const std::string &_value)
{
  this->PushSeparator();

  QLabel *value = new QLabel(tr(_value.c_str()));

  this->breadCrumbLayout->addWidget(value);
  value->setTextFormat(Qt::RichText);
  value->setTextInteractionFlags(Qt::TextBrowserInteraction);
  value->setOpenExternalLinks(false);

  this->currentIndex++;
}

/////////////////////////////////////////////////
std::string BreadCrumbWidget::Pop()
{
  int index = this->breadCrumbLayout->count() - 1;
  if (index < 2)
    return "";

  delete this->breadCrumbLayout->takeAt(index);

  this->PopSeparator();

  this->currentIndex--;
}

/////////////////////////////////////////////////
void BreadCrumbWidget::PushSeparator()
{
  QLabel *separator = new QLabel(tr(" > "));
  this->breadCrumbLayout->addWidget(separator);
}

/////////////////////////////////////////////////
void BreadCrumbWidget::PopSeparator()
{
  int index = this->breadCrumbLayout->count() - 1;
  if (index < 1)
    return;

  QLayoutItem *child =  this->breadCrumbLayout->itemAt(index);
  QLabel *label = dynamic_cast<QLabel *>(child->widget());
  if (label && label->text().contains(tr(">")))
  {
    delete this->breadCrumbLayout->takeAt(index);
  }
}
