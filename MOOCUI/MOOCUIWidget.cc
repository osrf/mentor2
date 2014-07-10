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

#include "MOOCUIWidget.hh"

using namespace gazebo;
using namespace std;



MOOCUIWidget::MOOCUIWidget(QWidget *_parent)
  : QWidget(_parent),
    node(new gazebo::transport::Node()),
    dialog(this, "https://mentor2.companion.org")
{
  node->Init();
  pub = node->Advertise<SimpleMOOC_msgs::msgs::LoginRequest>("~/MOOC");
  sub = node->Subscribe("~/MOOCResponse", &MOOCUIWidget::OnLoginResponse, this);
}
 

MOOCUIWidget::~MOOCUIWidget()
{
  cout << "MOOCUIWidget::~MOOCUIWidget()" << endl;
}

void MOOCUIWidget::LoginMOOC()
{
  std::cout << "MOOCUI login" << std::endl;
  gui::MOOCLoginDialog dialog(this, "https://mentor2.companion.org");

  if(dialog.exec() == QDialog::Rejected) {
    cout << "MOOCUIWidget::LoginMOOC CANCELLED" << endl;
  } else {
    SimpleMOOC_msgs::msgs::LoginRequest msg;
    msg.set_url(dialog.getUrl());
    msg.set_username(dialog.getUsername());
    msg.set_password(dialog.getPassword());
    pub->Publish(msg);

    cout << "Login request sent [";
    cout << dialog.getUrl() << ", ";
    cout << dialog.getUsername() << ", ";
    cout << dialog.getPassword() << "]"; 
    cout << endl;
  } 
}

void MOOCUIWidget::OnLoginResponse(ConstLoginResponsePtr &_msg )
{
  cout << "MOOCUI Login response received: ";
  cout << " success: " << _msg->success();
  cout << " msg: " << _msg->msg();
  cout << endl;

  if(_msg->success()) {
    cout << "LOGIN confirmed";
  }
  else {
    // alert!
    cout << "ALERT??????" << endl;
  }

}


