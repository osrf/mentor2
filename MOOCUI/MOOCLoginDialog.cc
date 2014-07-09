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

#include "MOOCLoginDialog.hh"
#include "MOOCUIWidget.hh"
#include <iostream>

#include <curl/curl.h>

using namespace gazebo;
using namespace gui;
using namespace std;


LearningCompanion:: LearningCompanion(const char* url, const char* user, const char* pass)
  :url(url), user(user), pass(pass)
{

}


bool LearningCompanion::Login(std::string &msg)
{
/*
    bool isLoggedIn = false;
    // verify login credentials
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) {
     cerr << "Login to " << url << " failed: " << curl_easy_strerror(res) << endl;
    } else {
      isLoggedIn = true;
    }
    cout << "User " << user << "  is logged in? :  " << isLoggedIn << endl;
    return isLoggedIn;
*/
}


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

  LearningCompanion server(url.c_str(), username.c_str(), password.c_str());
  labelInfo->setText("Connecting...");  
  qApp->processEvents();

  std::string msg;  
  //if( server.Login(msg))
//  {
//  cout << "MOOCLoginDialog::slotAcceptLogin" << endl;
//  cout << "  URL: " << url << endl;
//  cout << "  USER: " << username << endl;
//  cout << "  password: " << password <<  endl; 
    emit acceptLogin(u, user, pass);
    // close the dialog with success exit code
    accept();
//  }

}





