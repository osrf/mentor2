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

#include <gazebo/common/common.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/gui/gui.hh>
#include <gazebo/gui/qt.h>

#include "CMLManager.hh"
#include "CMLEvents.hh"
#include "CMLConnectionMaker.hh"

#include "Event.pb.h"
#include "SimpleConnection.pb.h"
#include "SimpleModel.pb.h"

#include "CMLPortInspector.hh"
#include "CMLComponentInspector.hh"
#include "CMLRender.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLRender::CMLRender()
{
/*  MouseEventHandler::Instance()->AddDoubleClickFilter("CML_Render",
    boost::bind(&CMLRender::OnMouseDoubleClick, this, _1));

  MouseEventHandler::Instance()->AddPressFilter("CML_Render",
      boost::bind(&CMLRender::OnMousePress, this, _1));

  MouseEventHandler::Instance()->AddMoveFilter("CML_Render",
      boost::bind(&CMLRender::OnMouseMove, this, _1));*/

  // Add an event filter, which allows the CMLRender to capture mouse events.
  MouseEventHandler::Instance()->AddReleaseFilter("CML_Render",
      boost::bind(&CMLRender::OnMouseRelease, this, _1));

/*  this->connections.push_back(
      event::Events::ConnectPreRender(
        boost::bind(&CMLRender::Update, this)));*/

  this->connections.push_back(
      CMLEvents::ConnectCreateConnection(
        boost::bind(&CMLRender::OnCreateConnection, this, _1)));

  this->connections.push_back(
      CMLEvents::ConnectConnectionCreated(
        boost::bind(&CMLRender::OnConnectionCreated, this, _1, _2)));

  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();

  this->connectionPub =
      this->node->Advertise<Simple_msgs::msgs::SimpleConnection>(
      "~/simple/connection");

  this->restPub =
      this->node->Advertise<Event_msgs::msgs::RestPost>(
      "~/event/rest_post");

  this->requestSub = this->node->Subscribe("~/request",
    &CMLRender::OnRequest, this);

  this->inspectAct = new QAction(tr("Open Inspector"), this);
  connect(this->inspectAct, SIGNAL(triggered()), this,
      SLOT(OnOpenInspector()));

}

/////////////////////////////////////////////////
CMLRender::~CMLRender()
{
//  MouseEventHandler::Instance()->RemovePressFilter("CML_Render");
  MouseEventHandler::Instance()->RemoveReleaseFilter("CML_Render");
//  MouseEventHandler::Instance()->RemoveMoveFilter("CML_Render");
//  MouseEventHandler::Instance()->RemoveDoubleClickFilter("CML_Render");
}

/*/////////////////////////////////////////////////
bool CMLRender::OnMousePress(const common::MouseEvent &_event)
{
  if (_event.button != common::MouseEvent::LEFT)
    return false;

  return false;
}*/


/////////////////////////////////////////////////
void CMLRender::OnRequest(ConstRequestPtr &_msg)
{
  if (_msg->request() == "entity_delete")
    CMLConnectionMaker::Instance()->RemoveConnectionsByEntity(_msg->data());
}

/////////////////////////////////////////////////
bool CMLRender::OnMouseRelease(const common::MouseEvent &_event)
{
  // Get the active camera and scene.
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::ScenePtr scene = camera->GetScene();
  rendering::VisualPtr vis = camera->GetVisual(_event.pos);

  if (_event.button == common::MouseEvent::RIGHT)
  {
    if (vis)
    {
      rendering::VisualPtr rootVis = vis->GetRootVisual();
      std::string name = rootVis->GetName();
      Simple_msgs::msgs::SimpleModel msg;
      msg = CMLManager::Instance()->GetModelInfo(name);

      if (!msg.name().empty())
      {
        std::cerr << " name " << name << " " << msg.name() << std::endl;
        this->inspectMsg = msg;

        QMenu menu;
        menu.addAction(this->inspectAct);
        menu.exec(QCursor::pos());
        return true;
      }
    }
  }

  /*if (_event.button == common::MouseEvent::LEFT)
  {
    if (vis)
    {
      std::string name = vis->GetRootVisual()->GetName();
      Simple_msgs::msgs::SimpleModel msg;
      msg = CMLManager::Instance()->GetModelInfo(name);

      std::cerr << " name " << name << " " << msg.name() << std::endl;

      CMLPortInspector *inspector =
          new CMLPortInspector(gui::get_main_window());
      inspector->Load(&msg);
      inspector->show();
    }
    //connect(inspector, Applied(), this,
    return false;
  }*/


  return false;
}
/*
/////////////////////////////////////////////////
bool CMLRender::OnMouseMove(const common::MouseEvent &_event)
{
  if (_event.dragging)
    return false;

  // Get the active camera and scene.
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::ScenePtr scene = camera->GetScene();

  rendering::VisualPtr vis = camera->GetVisual(_event.pos);

  return false;
}

/////////////////////////////////////////////////
bool CMLRender::OnMouseDoubleClick(const common::MouseEvent &_event)
{
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::VisualPtr vis = camera->GetVisual(_event.pos);

  if (vis)
  {
  }

  return false;
}

/////////////////////////////////////////////////
void CMLRender::Update()
{

}*/

/////////////////////////////////////////////////
void CMLRender::OnCreateConnection(const std::string &_type)
{
  CMLConnectionMaker::Instance()->AddConnection(_type);

  std::cerr << " create connection! " << std::endl;
}

/////////////////////////////////////////////////
void CMLRender::OnConnectionCreated(const std::string &_parent,
    const std::string &_child)
{
  std::cerr << " connection created " << _parent << " " << _child << std::endl;

  Simple_msgs::msgs::SimpleConnection msg;
  msg.set_parent(_parent);
  msg.set_child(_child);
  this->connectionPub->Publish(msg);


  Event_msgs::msgs::RestPost restMsg;
  restMsg.set_route("/events/new");

  std::string postStr;
  postStr = "{";
  postStr += "\"type\": \"connection\",";
  postStr += "\"name\": \"simple_connection\",";
  postStr += "\"data\": {";
  postStr += "\"parent\": \"" + _parent +"\",";
  postStr += "\"child\": \"" + _child +"\"";
  postStr += "}";
  postStr += "}";
  restMsg.set_json(postStr);
  this->restPub->Publish(restMsg);

  //CMLConnectionMaker::Instance()->Start();
}

/////////////////////////////////////////////////
void CMLRender::OnOpenInspector()
{
  CMLComponentInspector *inspector = NULL;
  if (this->componentInspectors.find(this->inspectMsg.name()) !=
      this->componentInspectors.end())
  {
    inspector = this->componentInspectors[this->inspectMsg.name()];
    inspector->activateWindow();
    inspector->setFocus();
  }
  else
  {
    inspector = new CMLComponentInspector(gui::get_main_window());
    inspector->Load(&this->inspectMsg);
    this->componentInspectors[this->inspectMsg.name()] = inspector;
    /*connect(inspector, SIGNAL(Applied()), this,
        SLOT(OnComponentProperyChanged()));*/
  }
  inspector->show();
}
