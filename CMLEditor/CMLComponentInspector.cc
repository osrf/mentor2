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

#include <gazebo/math/Helpers.hh>

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

  this->setMinimumWidth(220);

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
  this->simpleModelMsg = *_msg;
  QLabel *nameLabel = new QLabel(tr("Name"));
  QLabel *nameValueLabel = new QLabel(tr(_msg->name().c_str()));
  this->propertyLayout->addWidget(nameLabel, 0, 0);
  this->propertyLayout->addWidget(nameValueLabel, 0, 1);

  for (int i = 0; i < _msg->key_size(); ++i)
  {
    QLabel *keyLabel = new QLabel(tr(_msg->key(i).c_str()));
    QWidget *valueWidget = this->GetValueWidget(&_msg->value(i));
    this->valueWidgets[_msg->key(i)] = valueWidget;
    this->propertyLayout->addWidget(keyLabel, i+1, 0);
    this->propertyLayout->addWidget(valueWidget, i+1, 1);
  }
}
/////////////////////////////////////////////////
Simple_msgs::msgs::SimpleModel CMLComponentInspector::GetSimpleModelMsg()
{
  return this->simpleModelMsg;
}

/////////////////////////////////////////////////
void CMLComponentInspector::UpdateFromMsg(
    const Simple_msgs::msgs::SimpleModel *_msg)
{
  this->simpleModelMsg = *_msg;
  for (int i = 0; i < _msg->key_size(); ++i)
  {
    std::string key = _msg->key(i);
    if (this->valueWidgets.find(key) != this->valueWidgets.end())
    {
      QWidget *valueWidget = this->valueWidgets[key];
      const Simple_msgs::msgs::Variant variantMsg =
          this->simpleModelMsg.value(i);

      switch (variantMsg.type())
      {
        case  Simple_msgs::msgs::Variant::UINT32:
        {
          QSpinBox *widget = qobject_cast<QSpinBox *>(valueWidget);
          widget->setValue(variantMsg.v_uint32());
          break;
        }
        case  Simple_msgs::msgs::Variant::INT32:
        {
          QSpinBox *widget = qobject_cast<QSpinBox *>(valueWidget);
          widget->setValue(variantMsg.v_int32());
          break;
        }
        case  Simple_msgs::msgs::Variant::DOUBLE:
        {
          QDoubleSpinBox *widget = qobject_cast<QDoubleSpinBox *>(valueWidget);
          widget->setValue(variantMsg.v_double());
          break;
        }
        case  Simple_msgs::msgs::Variant::STRING:
        {
          QLineEdit *widget = qobject_cast<QLineEdit *>(valueWidget);
          widget->setText(tr(variantMsg.v_string().c_str()));
          break;
        }
        case  Simple_msgs::msgs::Variant::BOOL:
        {
          QCheckBox *widget = qobject_cast<QCheckBox *>(valueWidget);
          widget->setCheckState(
              variantMsg.v_bool() ? Qt::Checked : Qt::Unchecked);
          break;
        }
      }
    }
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
    {
      widget = new QSpinBox();
      QSpinBox *spinBoxWidget = qobject_cast<QSpinBox *>(widget);
      spinBoxWidget->setRange(GZ_UINT32_MIN, GZ_UINT32_MAX);
      spinBoxWidget->setValue(_msg->v_uint32());
      break;
    }
    case  Simple_msgs::msgs::Variant::INT32:
    {
      widget = new QSpinBox();
      QSpinBox *spinBoxWidget = qobject_cast<QSpinBox *>(widget);
      spinBoxWidget->setRange(-GZ_INT32_MAX, GZ_INT32_MAX);
      spinBoxWidget->setValue(_msg->v_int32());
      break;
    }
    case  Simple_msgs::msgs::Variant::DOUBLE:
    {
      widget = new QDoubleSpinBox();
      QDoubleSpinBox *spinBoxWidget = qobject_cast<QDoubleSpinBox *>(widget);
      spinBoxWidget->setRange(-GZ_DBL_MAX, GZ_DBL_MAX);
      spinBoxWidget->setDecimals(5);
      spinBoxWidget->setValue(_msg->v_double());
      break;
    }
    case  Simple_msgs::msgs::Variant::STRING:
    {
      widget = new QLineEdit();
      qobject_cast<QLineEdit *>(widget)->setText(tr(_msg->v_string().c_str()));
      break;
    }
    case  Simple_msgs::msgs::Variant::BOOL:
    {
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
  }
  return widget;
}

/////////////////////////////////////////////////
void CMLComponentInspector::Update()
{
  for (int i = 0; i < this->simpleModelMsg.key_size(); ++i)
  {
    std::string key = this->simpleModelMsg.key(i);
    if (this->valueWidgets.find(key) != this->valueWidgets.end())
    {
      QWidget *valueWidget = this->valueWidgets[key];
      Simple_msgs::msgs::Variant *variantMsg =
          this->simpleModelMsg.mutable_value(i);
      std::string typeName =
          std::string(valueWidget->metaObject()->className());
      if (typeName == "QDoubleSpinBox")
      {
        QDoubleSpinBox *widget = qobject_cast<QDoubleSpinBox *>(valueWidget);
        variantMsg->set_v_double(widget->value());
      }
      else if (typeName == "QSpinBox")
      {
        QSpinBox *widget = qobject_cast<QSpinBox *>(valueWidget);
        variantMsg->set_v_double(widget->value());
      }
      else if (typeName == "QCheckBox")
      {
        QCheckBox *widget = qobject_cast<QCheckBox *>(valueWidget);
        variantMsg->set_v_bool(widget->checkState() == Qt::Checked);
      }
      else if (typeName == "QLineEdit")
      {
        QLineEdit *widget = qobject_cast<QLineEdit *>(valueWidget);
        variantMsg->set_v_string(widget->text().toStdString());
      }
    }
  }
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
  this->Update();
  emit Applied();
}

/////////////////////////////////////////////////
void CMLComponentInspector::OnOK()
{
  this->Update();
  emit Applied();
  this->accept();
}
