/*
 * Copyright (C) 2012-2014 Open Source Robotics Foundation
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

#include "SimpleModel.pb.h"
#include "CMLPortInspector.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLPortInspector::CMLPortInspector(QWidget *_parent)
  : QDialog(_parent)
{
  this->setObjectName("portInspectorDialog");

  this->setWindowTitle(tr("Select a Port"));

  this->setMinimumWidth(150);

/*  QGroupBox *electricalGroupBox = new QGroupBox(tr("Electrical"));
  electricalGroupBox->setLayout(electricalLayout);

  QGroupBox *mechanicalGroupBox = new QGroupBox(tr("Mechanical"));
  mechanicalGroupBox->setLayout(mechanicalLayout);*/

  this->portsLayout = new QVBoxLayout;
  this->portsGroupBox = new QGroupBox(tr("Ports"));
  this->portsGroupBox->setLayout(this->portsLayout);

  this->portsButtonGroup = new QButtonGroup(this);
  this->portsButtonGroup->setExclusive(true);

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(OnCancel()));
  QPushButton *OKButton = new QPushButton(tr("&OK"));
  OKButton->setDefault(true);
  connect(OKButton, SIGNAL(clicked()), this, SLOT(OnOK()));
  buttonsLayout->addWidget(cancelButton);
  buttonsLayout->addWidget(OKButton);
  buttonsLayout->setAlignment(Qt::AlignRight);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(this->portsGroupBox);
  mainLayout->addLayout(buttonsLayout);

  this->setLayout(mainLayout);
}

/////////////////////////////////////////////////
CMLPortInspector::~CMLPortInspector()
{
}

/////////////////////////////////////////////////
void CMLPortInspector::Load(const Simple_msgs::msgs::SimpleModel *_msg)
{
  for (int i = 0; i < _msg->port_size(); ++i)
  {
    QHBoxLayout *radioLayout = new QHBoxLayout;
    QRadioButton *button = new QRadioButton(tr(_msg->port(i).c_str()));
    button->setStyleSheet("QRadioButton { color: #d0d0d0 }");

    this->portsButtonGroup->addButton(button);
    this->portsLayout->addWidget(button);
  }
  // By default, check the first port
  QList<QAbstractButton *> buttons = this->portsButtonGroup->buttons();
  if (buttons.size() > 0)
  {
    buttons[0]->setChecked(true);
  }
}

/////////////////////////////////////////////////
void CMLPortInspector::OnCancel()
{
  this->reject();
}

/////////////////////////////////////////////////
void CMLPortInspector::OnOK()
{
  emit Applied();
  this->accept();
}
