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

#include <gazebo/msgs/msgs.hh>

#include <gazebo/gui/gui.hh>
#include <gazebo/gui/qt.h>
#include <gazebo/gui/model/ModelEditorEvents.hh>
#include <gazebo/gui/model/ModelEditor.hh>

#include "CMLManager.hh"
#include "CMLEvents.hh"
#include "CMLConnectionMaker.hh"

#include "SimpleConnection.pb.h"
#include "SimpleModel.pb.h"

#include "CMLPortInspector.hh"
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

/*  this->connections.push_back(
      event::Events::ConnectPreRender(
        boost::bind(&CMLRender::Update, this)));*/
  this->active = false;

  this->connections.push_back(
      CMLEvents::ConnectCreateConnection(
        boost::bind(&CMLRender::OnCreateConnection, this, _1)));

  this->connections.push_back(
      CMLEvents::ConnectConnectionCreated(
        boost::bind(&CMLRender::OnConnectionCreated, this, _1, _2, _3, _4)));

  this->connections.push_back(
      gui::model::Events::ConnectEditModel(
      boost::bind(&CMLRender::OnEditModel, this, _1, _2, _3)));

  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();

  this->connectionPub =
      this->node->Advertise<Simple_msgs::msgs::SimpleConnection>(
      "~/simple/connection");

  this->restPub =
      this->node->Advertise<gazebo::msgs::RestPost>(
      "/gazebo/rest/rest_post");

  this->requestSub = this->node->Subscribe("~/request",
    &CMLRender::OnRequest, this);

  this->inspectName = "";
  this->inspectTopLevelName = "";
  this->inspectAct = new QAction(tr("Open Inspector"), this);
  connect(this->inspectAct, SIGNAL(triggered()), this,
      SLOT(OnOpenInspector()));

  if (g_editModelAct)
  {
    QObject::connect(g_editModelAct, SIGNAL(toggled(bool)), this,
      SLOT(OnModelEditorMode(bool)));
  }
}

/////////////////////////////////////////////////
CMLRender::~CMLRender()
{
//  MouseEventHandler::Instance()->RemovePressFilter("CML_Render");

//  MouseEventHandler::Instance()->RemoveMoveFilter("CML_Render");
//  MouseEventHandler::Instance()->RemoveDoubleClickFilter("CML_Render");
}

/*/////////////////////////////////////////////////
bool CMLRender::OnMousePress(const common::MouseEvent &_event)
{
  if (_event.Button() != common::MouseEvent::LEFT)
    return false;

  return false;
}*/
/////////////////////////////////////////////////
void CMLRender::OnModelEditorMode(bool _enabled)
{
  if (_enabled)
  {
    this->EnableEventHandlers();
    CMLConnectionMaker::Instance()->EnableEventHandlers();
  }
  else
  {
    this->DisableEventHandlers();
    CMLConnectionMaker::Instance()->DisableEventHandlers();
  }
}

/////////////////////////////////////////////////
void CMLRender::EnableEventHandlers()
{
  this->active = true;
  // Add an event filter, which allows the CMLRender to capture mouse events.
  MouseEventHandler::Instance()->AddReleaseFilter("CML_Render",
      boost::bind(&CMLRender::OnMouseRelease, this, _1));
  MouseEventHandler::Instance()->AddDoubleClickFilter("CML_Render",
      boost::bind(&CMLRender::OnMouseDoubleClick, this, _1));
}

/////////////////////////////////////////////////
void CMLRender::DisableEventHandlers()
{
  this->active = false;
  MouseEventHandler::Instance()->RemoveReleaseFilter("CML_Render");
  MouseEventHandler::Instance()->RemoveDoubleClickFilter("CML_Render");
}

/////////////////////////////////////////////////
void CMLRender::OnRequest(ConstRequestPtr &_msg)
{
  if (_msg->request() == "entity_delete")
    CMLConnectionMaker::Instance()->RemoveConnectionsByEntity(_msg->data());
}

/////////////////////////////////////////////////
bool CMLRender::OnMouseRelease(const common::MouseEvent &_event)
{
  if (!this->active)
    return false;

  // Get the active camera and scene.
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::ScenePtr scene = camera->GetScene();
  rendering::VisualPtr vis = camera->GetVisual(_event.Pos());

  this->inspectName = "";
  this->inspectTopLevelName = "";
  if (vis)
  {
    unsigned int depth = 2;
    rendering::VisualPtr modelVis;
    while (depth < vis->GetDepth())
    {
      rendering::VisualPtr childVis = vis->GetNthAncestor(depth);
      if (!childVis)
        break;

      std::string name = childVis->GetName();
      Simple_msgs::msgs::SimpleModel msg;
      msg = CMLManager::Instance()->GetModelInfo(name);
      if (msg.key_size() > 0)
        modelVis = childVis;
      depth++;
    }

    if (!modelVis)
      return false;

    this->inspectName = modelVis->GetName();
    rendering::VisualPtr topLevelVis = vis->GetNthAncestor(2);
    if (topLevelVis)
      this->inspectTopLevelName = topLevelVis->GetName();

    if (_event.Button() == common::MouseEvent::RIGHT)
    {
      QMenu menu;
      if (this->inspectAct)
      {
        menu.addAction(this->inspectAct);
      }
      QAction *deleteAct = new QAction(tr("Delete"), this);
      connect(deleteAct, SIGNAL(triggered()), this, SLOT(OnDelete()));
      menu.addAction(deleteAct);
      menu.exec(QCursor::pos());
      return true;
    }
  }

  return false;
}

/////////////////////////////////////////////////
bool CMLRender::OnMouseDoubleClick(const common::MouseEvent &_event)
{
  if (!this->active)
    return false;

  // Get the active camera and scene.
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::ScenePtr scene = camera->GetScene();
  rendering::VisualPtr vis = camera->GetVisual(_event.Pos());

  if (vis)
  {
    unsigned int depth = 2;
    rendering::VisualPtr modelVis;
    while (depth < vis->GetDepth())
    {
      rendering::VisualPtr childVis = vis->GetNthAncestor(depth);
      if (!childVis)
        break;

      std::string name = childVis->GetName();
      Simple_msgs::msgs::SimpleModel msg;
      msg = CMLManager::Instance()->GetModelInfo(name);
      if (msg.key_size() > 0)
        modelVis = childVis;
      depth++;
    }

    if (!modelVis)
      return false;

    CMLManager::Instance()->ShowInspector(modelVis->GetName());
    return true;
  }

  return false;
}

/*
/////////////////////////////////////////////////
bool CMLRender::OnMouseMove(const common::MouseEvent &_event)
{
  if (_event.Dragging())
    return false;

  // Get the active camera and scene.
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::ScenePtr scene = camera->GetScene();

  rendering::VisualPtr vis = camera->GetVisual(_event.Pos());

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
}

/////////////////////////////////////////////////
void CMLRender::OnConnectionCreated(const std::string &_parent,
    const std::string &_parentPort, const std::string &_child,
    const std::string &_childPort)
{
  Simple_msgs::msgs::SimpleConnection msg;
  msg.set_parent(_parent);
  msg.set_child(_child);
  msg.set_parent_port(_parentPort);
  msg.set_child_port(_childPort);
  this->connectionPub->Publish(msg);

  gazebo::msgs::RestPost restMsg;
  restMsg.set_route("/events/new");

  // remove model preview prefix
  std::string parentScopedName = _parent;
  size_t pIdx = parentScopedName.find("::");
  if (pIdx != std::string::npos)
    parentScopedName = parentScopedName.substr(pIdx+2);
  std::string childScopedName = _child;
  pIdx = childScopedName.find("::");
  if (pIdx != std::string::npos)
    childScopedName = childScopedName.substr(pIdx+2);

  std::string postStr;
  postStr = "\"type\": \"connection\",";
  postStr += "\"name\": \"simple_connection\",";
  postStr += "\"data\": {";
  postStr += "\"parent\": \"" + parentScopedName +"\",";
  postStr += "\"parent_port\": \"" + _parentPort +"\",";
  postStr += "\"child\": \"" + childScopedName +"\",";
  postStr += "\"child_port\": \"" + _childPort +"\"";
  postStr += "}";
  restMsg.set_json(postStr);
  this->restPub->Publish(restMsg);
}

/////////////////////////////////////////////////
void CMLRender::RemoveEntity(const std::string &_name)
{
  // this function is called when user invokes the delete option in
  // context menu, or the ModelCreator fires the nestedModelRemoved event.

  MainWindow *mainWindow = gui::get_main_window();
  if (mainWindow)
  {
    CMLConnectionMaker::Instance()->RemoveConnectionsByEntity(_name);

    ModelEditor *modelEditor =
        dynamic_cast<ModelEditor *>(mainWindow->Editor("model"));

    if (modelEditor)
      modelEditor->RemoveEntity(_name);
  }
}

/////////////////////////////////////////////////
void CMLRender::OnDelete()
{
  this->RemoveEntity(this->inspectTopLevelName);

  this->inspectTopLevelName = "";
  this->inspectName = "";
}

/////////////////////////////////////////////////
void CMLRender::OnOpenInspector()
{
  CMLManager::Instance()->ShowInspector(this->inspectName);
  this->inspectTopLevelName = "";
  this->inspectName = "";
}

/////////////////////////////////////////////////
void CMLRender::OnEditModel(const std::string &/*_modelName*/,
    const std::string &_modelPreviewName,
    const std::string &_sdfData)
{
//  std::cerr << "on edit  model " << _modelName << " " <<
//    _modelPreviewName << " " << _sdfData << std::endl;
  // Parse the string into sdf
  sdf::SDF sdfParsed;
  sdfParsed.SetFromString(_sdfData);

  if (sdfParsed.Root()->HasElement("model"))
  {
    sdf::ElementPtr modelElem = sdfParsed.Root()->GetElement("model");
    if (modelElem->HasElement("plugin"))
    {
      sdf::ElementPtr pluginElem = modelElem->GetElement("plugin");
      if (pluginElem->HasElement("connection"))
      {
        sdf::ElementPtr connectionElem = pluginElem->GetElement("connection");
        while (connectionElem)
        {
          CMLConnectionMaker::Instance()->CreateConnectionFromSDF(
              connectionElem, _modelPreviewName);
          connectionElem = connectionElem->GetNextElement("connection");
        }
      }
    }
  }

}
