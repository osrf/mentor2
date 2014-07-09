/*
 * Copyright 2014 Open Source Robotics Foundation
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
#include <curl/curl.h>

#include "MOOCLoginDialog.hh"
#include "MOOCUIWidget.hh"
#include "SimpleMOOC.pb.h"

using namespace gazebo;
using namespace std;



MOOCUIWidget::MOOCUIWidget(QWidget *_parent)
  : QWidget(_parent), isLoggedIn(false),
    node(new gazebo::transport::Node())
{
  node->Init();
  pub = node->Advertise<SimpleMOOC_msgs::msgs::SimpleMOOCLoginRequest>("~/MOOC");
}
 

MOOCUIWidget::~MOOCUIWidget()
{
}

void MOOCUIWidget::LoginMOOC()
{
  std::cout << "LOGIN to the MOOC" << std::endl;
  gui::MOOCLoginDialog dialog(this, "https://mentor2.companion.org");

  if(dialog.exec() == QDialog::Rejected) {
    cout << "MOOCUIWidget::LoginMOOC CANCELLED" << endl;
  } else {
    cout << "MOOCUIWidget::LoginMOOC Accepted " << endl;
    cout << "  user: " << dialog.getUsername() << endl;
    cout << "  pass: " << dialog.getPassword() << endl;
    cout << "  url:  " << dialog.getUrl() << endl;


    SimpleMOOC_msgs::msgs::SimpleMOOCLoginRequest msg;
   //loginPub->WaitForConnection();
    msg.set_url(dialog.getUrl());
    msg.set_username(dialog.getUsername());
    msg.set_password(dialog.getPassword());
    cout << "PUB ... " << endl;
    pub->Publish(msg);
    cout<< "LISHED!!! " << endl;

  } 
}




