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

#include <iostream>
#include <gazebo/gui/qt.h>

#include "BreadCrumbWidget.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
BreadCrumbWidget::BreadCrumbWidget(QWidget *_parent)
    : QWidget(_parent)
{
  this->setObjectName("breadCrumbWidget");


  QVBoxLayout *mainLayout = new QVBoxLayout;

  this->breadCrumbLayout = new QHBoxLayout;
  this->breadCrumbLayout->setAlignment(Qt::AlignLeft);

  QPushButton *all = new QPushButton(tr("All"));
  all->setFlat(true);
  connect(all, SIGNAL(clicked()), this, SLOT(OnClicked()));

  this->breadCrumbLayout->addWidget(all);

  this->currentIndex = 0;

  mainLayout->addLayout(this->breadCrumbLayout);
  this->setLayout(mainLayout);
}

/////////////////////////////////////////////////
BreadCrumbWidget::~BreadCrumbWidget()
{
}


/////////////////////////////////////////////////
void BreadCrumbWidget::Push(const std::string &_value)
{
  this->PushSeparator();

  QPushButton *value = new QPushButton(tr(_value.c_str()));
  value->setFlat(true);

  this->breadCrumbLayout->addWidget(value);

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
    QLayoutItem *item = this->breadCrumbLayout->takeAt(index);
    delete item->widget();
    delete item;
  }
}

/////////////////////////////////////////////////
void BreadCrumbWidget::SetCurrentIndex(unsigned int _index)
{
  if ((_index * 2) >= this->breadCrumbLayout->count())
  {
    std::cerr << "Error setting current BreadCrumb index: index is out or range"
       << std::endl;
    return;
  }

  this->Truncate(_index+1);
  emit IndexChanged(_index);
}

/////////////////////////////////////////////////
void BreadCrumbWidget::OnClicked()
{
  QPushButton * sender = qobject_cast<QPushButton *>(QObject::sender());
  if (sender)
  {
     int index = this->breadCrumbLayout->indexOf(sender)/2;
    this->SetCurrentIndex(index);
  }
}

/////////////////////////////////////////////////
void BreadCrumbWidget::Truncate(unsigned int _start, unsigned int _end)
{
  unsigned int start = _start * 2;

  unsigned int end = _end;
  if  (end == 0)
    end = this->breadCrumbLayout->count()-1;
  else
    end = end * 2;

  if (end >= this->breadCrumbLayout->count())
  {
    std::cerr << "Error truncating BreadCrumb: end is out or range"
        << std::endl;
    return;
  }
  else if (end < start)
  {
    std::cerr << "Error truncating BreadCrumb: start is larger than end"
        << std::endl;
    return;
  }
  else


  if (start > 0)
    start -= 1;

  unsigned int count = end - start + 1;

  while (count > 0)
  {
    QLayoutItem *item = this->breadCrumbLayout->takeAt(start);
    delete item->widget();
    delete item;
    count--;
  }

  if (this->currentIndex >= _end || this->currentIndex >= _start)
    this->currentIndex = _start - 1;
}
