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

#include <curl/curl.h>
#include <QMessageBox>
#include "MOOCUIWidget.hh"

using namespace gazebo;
using namespace std;



MOOCUIWidget::MOOCUIWidget(QWidget *_parent)
  : QWidget(_parent),
    node(new gazebo::transport::Node()),
    dialog(this, "https://mentor2.companion.org")
{
  
  cout << "MOOCUIWidget::MOOCUIWidget node setup" << endl;
  node->Init();
  pub = node->Advertise<Event_msgs::msgs::RestLogin>("~/event/rest_login");
  sub = node->Subscribe("~/event/rest_error", &MOOCUIWidget::OnResponse, this);
}
 

MOOCUIWidget::~MOOCUIWidget()
{
  cout << "MOOCUIWidget::~MOOCUIWidget()" << endl;
}

void MOOCUIWidget::LoginMOOC()
{
  std::cout << "MOOCUI login" << std::endl;

  if(dialog.exec() == QDialog::Rejected) {
    cout << "MOOCUIWidget::Login CANCELLED" << endl;
  } else {
    Event_msgs::msgs::RestLogin msg;
    msg.set_url(dialog.getUrl());
    msg.set_username(dialog.getUsername());
    msg.set_password(dialog.getPassword());
    pub->Publish(msg);

    cout << "Login to ~/RestLogin  [";
    cout << dialog.getUrl() << ", ";
    cout << dialog.getUsername() << ", ";
    cout << dialog.getPassword() << "]"; 
    cout << endl;
  } 
}


/*
void MOOCUIWidget::PostLearningEvent()

  string json = "{ \"Type\": \""; 
  json += type;
  json +=  "\", \"Results\": { ";
  json += "\"startTime\": \"2014-07-08T16:01:18.636Z\", ";
  json += "\"completedAt\": \"2014-07-08T16:23:15.636Z\", ";
  json += "\"timeSpent\": \"1087.3\", \"world\": \"simple.world\", ";
  json += "\"score\": \""; 
  std::string d = boost::lexical_cast<std::string>(score);
  json += d;
  json += "\", \"issues\": [\"Simple MOOC plugin\", ";
  json += "\"connecting pully\"] }, \"Resources\": [\"intro.mpg\", ";
  json += "\"lesson1.mpg\"], \"userid\": \"myuser\"  }";

 route =  "/events/new" 

*/


void MOOCUIWidget::OnResponse(ConstRestErrorPtr &_msg )
{
  cout << "Error received:" << endl;
  cout << " type: " << _msg->type() << endl;
  cout << " msg:  " << _msg->msg() << endl;

  // add msg to queue for later processing from
  // the GUI thread
  // msgQueue.push_back(_msg);
  msgRespQ.push_back(_msg);
}

void  MOOCUIWidget::Update()
{
  // Login problem?
  while(!msgRespQ.empty()) {
    ConstRestErrorPtr msg = msgRespQ.front();
    msgRespQ.pop_front();
    if(msg->type().c_str() == string("Error")) 
      QMessageBox::critical(this, tr("MOOC"),tr(msg->msg().c_str()) );
    else
      QMessageBox::information(this, tr("MOOC"),tr(msg->msg().c_str()) );
  }
}
