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

#ifndef _MOOCUI_LOGIN_DIALOG_HH_
#define _MOOCUI_LOGIN_DIALOG_HH_


#include "MOOCLoginDialog.hh"
#include "MOOCUIWidget.hh"
#include <iostream>

#include <curl/curl.h>

using namespace gazebo;
using namespace gui;
using namespace std;


/////////////////////////////////////////////////
MOOCLoginDialog::MOOCLoginDialog::MOOCLoginDialog(QWidget *_parent, const char* defaultUrl)
    : QDialog(_parent), url(defaultUrl)
{

  setWindowTitle(tr("Mentor 2 Login"));
  setModal(true);

  labelUrl = new QLabel(this);
  labelUrl->setText(tr("Learning companion URL"));
  editUrl = new QLineEdit(this);
  editUrl->setText(tr(url.c_str()));
  editUrl->setFixedWidth(400);
  labelUrl->setBuddy(editUrl);

  labelUsername = new QLabel(this);
  labelUsername->setText(tr("Username"));
  editUsername = new QLineEdit(this);
  editUsername->setFocus();
  labelUsername->setBuddy(editUsername);

  labelPassword = new QLabel(this);
  labelPassword->setText(tr("Password"));
  editPassword = new QLineEdit(this);
  editPassword->setEchoMode(QLineEdit::Password);
  labelPassword->setBuddy(editPassword);

  labelInfo = new QLabel(this);

  buttons = new QDialogButtonBox( this );
  buttons->addButton(QDialogButtonBox::Ok );
  buttons->button(QDialogButtonBox::Ok)->setText("Login");
  buttons->button(QDialogButtonBox::Ok)->setDefault(true);
  buttons->addButton(QDialogButtonBox::Cancel );
  // buttons->button(QDialogButtonBox::Cancel)->setText("Abort");

  // place components
  QGridLayout *formGridLayout = new QGridLayout(this);

  formGridLayout->addWidget(labelUrl, 0, 0, 1, 2);
  formGridLayout->addWidget(editUrl,  1, 0, 1, 2);
  formGridLayout->addWidget(labelUsername, 2, 0);
  formGridLayout->addWidget(editUsername,  2, 1);
  formGridLayout->addWidget(labelPassword, 3, 0);
  formGridLayout->addWidget(editPassword,  3, 1);
  formGridLayout->addWidget(labelInfo, 4, 0, 1, 2);
  formGridLayout->addWidget(buttons,   5, 0, 1, 2);

  setLayout(formGridLayout);

  connect( buttons->button( QDialogButtonBox::Cancel ),
             SIGNAL(clicked()),
             this,
             SLOT(close())
             ); 

  connect( buttons->button( QDialogButtonBox::Ok ),
             SIGNAL(clicked()),
             this,
             SLOT(slotAcceptLogin()) );

}

void MOOCLoginDialog::slotAcceptLogin()
{
  QString user = editUsername->text();
  QString pass = editPassword->text();
  QString u    = editUrl->text();

  username = user.toStdString();
  password = pass.toStdString();
  url = u.toStdString();

  labelInfo->setText("Connecting...");  
  qApp->processEvents();

  std::string msg;  

//  cout << "MOOCLoginDialog::slotAcceptLogin" << endl;
//  cout << "  URL: " << url << endl;
//  cout << "  USER: " << username << endl;
//  cout << "  password: " << password <<  endl; 
    emit acceptLogin(u, user, pass);
    // close the dialog with success exit code
    accept();

}


#endif


