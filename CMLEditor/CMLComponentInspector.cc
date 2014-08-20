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

#include <iostream>
#include "SimpleModel.pb.h"
#include "CMLComponentInspector.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLComponentInspector::CMLComponentInspector(QWidget *_parent)
  : QDialog(_parent)
{
  this->setObjectName("componentInspectorDialog");

  this->setWindowTitle(tr("Component Inspector"));

/*  QGroupBox *electricalGroupBox = new QGroupBox(tr("Electrical"));
  electricalGroupBox->setLayout(electricalLayout);

  QGroupBox *mechanicalGroupBox = new QGroupBox(tr("Mechanical"));
  mechanicalGroupBox->setLayout(mechanicalLayout);*/


  this->propertyLayout = new QGridLayout;

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(OnCancel()));
  QPushButton *applyButton = new QPushButton(tr("&Apply"));
  connect(applyButton, SIGNAL(clicked()), this, SLOT(OnApply()));
  QPushButton *OKButton = new QPushButton(tr("&OK"));
  OKButton->setDefault(true);
  connect(OKButton, SIGNAL(clicked()), this, SLOT(OnOK()));
  buttonsLayout->addWidget(cancelButton);
  buttonsLayout->addWidget(applyButton);
  buttonsLayout->addWidget(OKButton);
  buttonsLayout->setAlignment(Qt::AlignRight);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(this->propertyLayout);
  mainLayout->addLayout(buttonsLayout);

  this->setLayout(mainLayout);
}

/////////////////////////////////////////////////
CMLComponentInspector::~CMLComponentInspector()
{
}

/////////////////////////////////////////////////
void CMLComponentInspector::Load(const Simple_msgs::msgs::SimpleModel *_msg)
{
  for (int i = 0; i < _msg->key_size(); ++i)
  {
    QLabel *keyLabel = new QLabel(tr(_msg->key(i).c_str()));
    QWidget *valueWidget = this->GetValueWidget(&_msg->value(i));
    this->propertyLayout->addWidget(keyLabel, i, 0);
    this->propertyLayout->addWidget(valueWidget, i, 1);
  }
}

/////////////////////////////////////////////////
QWidget *CMLComponentInspector::GetValueWidget(
    const Simple_msgs::msgs::Variant *_msg)
{
  QWidget *widget = NULL;
  switch (_msg->type())
  {
    case  Simple_msgs::msgs::Variant::UINT32:
      widget = new QSpinBox();
      qobject_cast<QSpinBox *>(widget)->setValue(_msg->v_uint32());
      break;
    case  Simple_msgs::msgs::Variant::INT32:
      widget = new QSpinBox();
      qobject_cast<QSpinBox *>(widget)->setValue(_msg->v_int32());
      break;
    case  Simple_msgs::msgs::Variant::DOUBLE:
      widget = new QDoubleSpinBox();
      qobject_cast<QDoubleSpinBox *>(widget)->setValue(_msg->v_double());
      break;
    case  Simple_msgs::msgs::Variant::STRING:
      widget = new QLineEdit();
      qobject_cast<QLineEdit *>(widget)->setText(tr(_msg->v_string().c_str()));
      break;
    case  Simple_msgs::msgs::Variant::BOOL:
      widget = new QCheckBox();
      QCheckBox *checkBoxWidget = qobject_cast<QCheckBox *>(widget);
      checkBoxWidget->setTristate(false);
      connect(checkBoxWidget, SIGNAL(stateChanged(int)), this,
           SLOT(BoolPropChanged(int)));
      if (_msg->v_bool())
      {
        checkBoxWidget->setCheckState(Qt::Checked);
        checkBoxWidget->setText(tr("True"));
      }
      else
      {
        checkBoxWidget->setCheckState(Qt::Unchecked);
        checkBoxWidget->setText(tr("False"));
      }
      break;
  }
  return widget;
}

/////////////////////////////////////////////////
void CMLComponentInspector::BoolPropChanged(int _state)
{
  QCheckBox *checkBoxWidget = qobject_cast<QCheckBox *>(QObject::sender());
  if (_state == Qt::Checked)
    checkBoxWidget->setText(tr("True"));
  else
    checkBoxWidget->setText(tr("False"));
}

/////////////////////////////////////////////////
void CMLComponentInspector::OnCancel()
{
  this->close();
}

/////////////////////////////////////////////////
void CMLComponentInspector::OnApply()
{
  emit Applied();
}

/////////////////////////////////////////////////
void CMLComponentInspector::OnOK()
{
  emit Applied();
  this->accept();
}
