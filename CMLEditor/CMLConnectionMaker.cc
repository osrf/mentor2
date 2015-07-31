/*
 * Copyright (C) 2013-2014 Open Source Robotics Foundation
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

#include <boost/thread/recursive_mutex.hpp>
#include <string>
#include <vector>

#include <gazebo/common/common.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/gui/model/ModelEditorEvents.hh>
#include <gazebo/gui/model/ModelEditor.hh>
#include <gazebo/gui/gui.hh>

#include "CMLEvents.hh"
#include "CMLManager.hh"
#include "CMLPortInspector.hh"
#include "CMLConnectionMaker.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLConnectionMaker::CMLConnectionMaker()
{
  this->newConnectionCreated = false;
  this->mouseConnection = NULL;

  this->connectType = CMLConnectionMaker::CONNECT_NONE;
  this->connectCounter = 0;

  this->connectionMaterials[CONNECT_MECHANICAL] = "Gazebo/White";
  this->connectionMaterials[CONNECT_ELECTRICAL] = "Gazebo/Black";

  this->connections.push_back(
      event::Events::ConnectPreRender(
        boost::bind(&CMLConnectionMaker::Update, this)));

  this->connections.push_back(
      gui::model::Events::ConnectFinishModel(
      boost::bind(&CMLConnectionMaker::OnFinish, this)));

  this->updateMutex = new boost::recursive_mutex();
}

/////////////////////////////////////////////////
CMLConnectionMaker::~CMLConnectionMaker()
{
  this->DisableEventHandlers();
  this->Reset();
}

/////////////////////////////////////////////////
void CMLConnectionMaker::Reset()
{
  boost::recursive_mutex::scoped_lock lock(*this->updateMutex);
  this->newConnectionCreated = false;
  if (this->mouseConnection)
  {
    delete this->mouseConnection;
    this->mouseConnection = NULL;
  }

  this->connectType = CMLConnectionMaker::CONNECT_NONE;
  this->selectedVis.reset();
  this->hoverVis.reset();
  this->selectedConnection.reset();

  while (this->connects.size() > 0)
    this->RemoveConnection(this->connects.begin()->first);
  this->connects.clear();
}

/////////////////////////////////////////////////
void CMLConnectionMaker::EnableEventHandlers()
{
  MouseEventHandler::Instance()->AddReleaseFilter("cml_connection",
      boost::bind(&CMLConnectionMaker::OnMouseRelease, this, _1));

  KeyEventHandler::Instance()->AddPressFilter("cml_connection",
      boost::bind(&CMLConnectionMaker::OnKeyPress, this, _1));
}

/////////////////////////////////////////////////
void CMLConnectionMaker::DisableEventHandlers()
{
  MouseEventHandler::Instance()->RemoveReleaseFilter("cml_connection");
  KeyEventHandler::Instance()->RemovePressFilter("cml_connection");
}

/////////////////////////////////////////////////
void CMLConnectionMaker::RemoveConnection(const std::string &_connectionName)
{
  boost::recursive_mutex::scoped_lock lock(*this->updateMutex);
  if (this->connects.find(_connectionName) != this->connects.end())
  {
    ConnectionData *connect = this->connects[_connectionName];
    rendering::ScenePtr scene = connect->hotspot->GetScene();
    scene->RemoveVisual(connect->hotspot);
    scene->RemoveVisual(connect->visual);
    connect->hotspot.reset();
    connect->visual.reset();
    connect->parent.reset();
    connect->child.reset();
    delete connect;
    this->connects.erase(_connectionName);
  }
}

/////////////////////////////////////////////////
void CMLConnectionMaker::RemoveConnectionsByEntity(
    const std::string &_entityName)
{
  std::vector<std::string> toDelete;
  std::map<std::string, ConnectionData *>::iterator it;
  for (it = this->connects.begin(); it != this->connects.end(); ++it)
  {
    ConnectionData *connect = it->second;

    if (connect->child->GetName() == _entityName ||
        connect->parent->GetName() == _entityName)
    {
      toDelete.push_back(it->first);
    }
  }

  for (unsigned i = 0; i < toDelete.size(); ++i)
    this->RemoveConnection(toDelete[i]);

  toDelete.clear();
}

/////////////////////////////////////////////////
bool CMLConnectionMaker::OnMousePress(const common::MouseEvent &_event)
{
  this->mouseEvent = _event;

  if (_event.Button() != common::MouseEvent::LEFT)
    return false;

  if (this->connectType != CMLConnectionMaker::CONNECT_NONE)
    return false;

  // intercept mouse press events when user clicks on the connect hotspot visual
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::ScenePtr scene = camera->GetScene();
  rendering::VisualPtr vis = camera->GetVisual(_event.Pos());
  if (vis)
  {
    if (this->connects.find(vis->GetName()) != this->connects.end())
    {
      // stop event propagation as we don't want users to manipulate the
      // hotspot
      return true;
    }
  }
  return false;
}

/////////////////////////////////////////////////
bool CMLConnectionMaker::OnMouseRelease(const common::MouseEvent &_event)
{
  this->mouseEvent = _event;

  if (this->connectType == CMLConnectionMaker::CONNECT_NONE)
  {
    rendering::UserCameraPtr camera = gui::get_active_camera();
    rendering::ScenePtr scene = camera->GetScene();
    rendering::VisualPtr vis = camera->GetVisual(_event.Pos());
    if (vis)
    {
      if (this->selectedConnection)
        this->selectedConnection->SetHighlighted(false);
      this->selectedConnection.reset();

      // check if mouse pick is not a connection visual
      if (this->connects.find(vis->GetName()) != this->connects.end())
      {
        // trigger connect inspector on right click
        if (_event.Button() == common::MouseEvent::RIGHT)
        {
          /*this->inspectVis = vis;
          QMenu menu;
          menu.addAction(this->inspectAct);
          menu.exec(QCursor::pos());*/
        }
        else if (_event.Button() == common::MouseEvent::LEFT)
        {
          // turn off model selection so we don't end up with
          // both connection visual and model selected at the same time
          //event::Events::setSelectedEntity("", "normal");

          // this->selectedConnection = vis->GetRootVisual();
          // this->selectedConnection->SetHighlighted(true);
        }
        // stop event propagation as we don't want users to manipulate the
        // hotspot
        return true;
      }
      return false;
    }
  }
  else
  {
    if (this->hoverVis)
    {
      // Pressed parent part
      if (!this->selectedVis)
      {
        if (this->mouseConnection)
          return false;

        // select port, blocking call.
        std::string port = this->SelectPort();
        if (port.empty())
        {
          this->Stop();
          return false;
        }

        this->hoverVis->SetEmissive(common::Color(0, 0, 0));
        this->selectedVis = this->hoverVis;
        this->hoverVis.reset();

        // Create connection data with selected visual as parent
        // the child will be set on the second mouse release.
        this->mouseConnection = this->CreateConnection(this->selectedVis,
            rendering::VisualPtr());
        this->mouseConnection->parentPort = port;
      }
      // Pressed child part
      else if (this->selectedVis != this->hoverVis)
      {
        // select port, blocking call.
        std::string port = this->SelectPort();
        if (port.empty())
        {
          return false;
        }

        this->hoverVis->SetEmissive(common::Color(0, 0, 0));
        this->mouseConnection->child = this->hoverVis;
        this->mouseConnection->childPort = port;

        // reset variables.
        this->selectedVis.reset();
        this->hoverVis.reset();
        this->AddConnection(CMLConnectionMaker::CONNECT_NONE);

        this->newConnectionCreated = true;

        this->InsertConnectionElement(this->mouseConnection);

        gui::CMLEvents::connectionCreated(
            this->mouseConnection->parent->GetName(),
            this->mouseConnection->parentPort,
            this->mouseConnection->child->GetName(),
            this->mouseConnection->childPort);

        /*// Create connection SDF
        sdf::ElementPtr connectionElem(new sdf::Element);
        connectionElem->SetName("connection");

        sdf::ElementPtr sourceElem(new sdf::Element);
        sourceElem->SetName("source");

        std::string leafName = this->mouseConnection->parent->GetName();
        size_t pIdx = leafName.find_last_of("::");
        if (pIdx != std::string::npos)
          leafName = leafName.substr(pIdx+1);

        sourceElem->AddValue("string", leafName, "_none_", "source");
        connectionElem->InsertElement(sourceElem);

        sdf::ElementPtr sourcePortElem(new sdf::Element);
        sourcePortElem->SetName("source_port");
        sourcePortElem->AddValue("string", this->mouseConnection->parentPort,
            "_none_", "sourcePort");
        connectionElem->InsertElement(sourcePortElem);

        sdf::ElementPtr targetElem(new sdf::Element);
        targetElem->SetName("target");

        leafName = this->mouseConnection->child->GetName();
        pIdx = leafName.find_last_of("::");
        if (pIdx != std::string::npos)
          leafName = leafName.substr(pIdx+1);

        targetElem->AddValue("string", leafName, "_none_", "target");
        connectionElem->InsertElement(targetElem);

        sdf::ElementPtr targetPortElem(new sdf::Element);
        targetPortElem->SetName("target_port");
        targetPortElem->AddValue("string", this->mouseConnection->childPort,
            "_none_", "targetPort");
        connectionElem->InsertElement(targetPortElem);

        // Append to model SDF
        gazebo::gui::MainWindow *mainWindow = gui::get_main_window();
        gazebo::gui::ModelEditor *modelEditor = dynamic_cast<
            gazebo::gui::ModelEditor *>(mainWindow->GetEditor("model"));
        modelEditor->AppendPluginElement("simple_connections",
            "libSimpleConnectionsPlugin.so", connectionElem);*/
      }
    }

    return true;
  }
  return false;
}

/////////////////////////////////////////////////
void CMLConnectionMaker::InsertConnectionElement(ConnectionData *_connection)
{
  // Create connection SDF
  sdf::ElementPtr connectionElem(new sdf::Element);
  connectionElem->SetName("connection");

  sdf::ElementPtr sourceElem(new sdf::Element);
  sourceElem->SetName("source");

//  std::string leafName = _connection->parent->GetName();
  std::string scopedName = _connection->parent->GetName();
  size_t pIdx = scopedName.find("::");
  if (pIdx != std::string::npos)
    scopedName = scopedName.substr(pIdx+2);

  sourceElem->AddValue("string", scopedName, "_none_", "source");
  connectionElem->InsertElement(sourceElem);

  sdf::ElementPtr sourcePortElem(new sdf::Element);
  sourcePortElem->SetName("source_port");
  sourcePortElem->AddValue("string", _connection->parentPort,
      "_none_", "sourcePort");
  connectionElem->InsertElement(sourcePortElem);

  sdf::ElementPtr targetElem(new sdf::Element);
  targetElem->SetName("target");

  scopedName = _connection->child->GetName();
  pIdx = scopedName.find("::");
  if (pIdx != std::string::npos)
    scopedName = scopedName.substr(pIdx+2);

  targetElem->AddValue("string", scopedName, "_none_", "target");
  connectionElem->InsertElement(targetElem);

  sdf::ElementPtr targetPortElem(new sdf::Element);
  targetPortElem->SetName("target_port");
  targetPortElem->AddValue("string", _connection->childPort,
      "_none_", "targetPort");
  connectionElem->InsertElement(targetPortElem);

  // Append to model SDF
  gazebo::gui::MainWindow *mainWindow = gui::get_main_window();
  gazebo::gui::ModelEditor *modelEditor = dynamic_cast<
      gazebo::gui::ModelEditor *>(mainWindow->GetEditor("model"));

  modelEditor->AppendPluginElement("simple_connections",
      "libSimpleConnectionsPlugin.so", connectionElem);
}

/////////////////////////////////////////////////
std::string CMLConnectionMaker::SelectPort()
{
  if (!this->hoverVis)
    return "";

  // show the port inspector if there are ports
  std::string name = this->hoverVis->GetName();
  Simple_msgs::msgs::SimpleModel msg;
  msg = CMLManager::Instance()->GetModelInfo(name);

  bool accept = true;
  std::string selectedPort = "";
  if (msg.port_size() > 0)
  {
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));

    CMLPortInspector inspector(gui::get_main_window());
    inspector.move(QCursor::pos());
    inspector.setModal(true);
    inspector.Load(&msg);
    int ret = inspector.exec();
    if (ret != QDialog::Accepted)
      return selectedPort;

    selectedPort = inspector.GetPort();
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
  }
  return selectedPort;
}

/////////////////////////////////////////////////
ConnectionData *CMLConnectionMaker::CreateConnection(
    rendering::VisualPtr _parent, rendering::VisualPtr _child)
{
  std::stringstream ss;
  ss << _parent->GetName() << "_CONNECT_" << this->connectCounter++;
  rendering::VisualPtr connectVis(new rendering::Visual(ss.str(), _parent));
  connectVis->Load();
  rendering::DynamicLines *connectLine =
      connectVis->CreateDynamicLine(rendering::RENDERING_LINE_LIST);
  math::Vector3 origin = math::Vector3::Zero;
  connectLine->AddPoint(origin);
  connectLine->AddPoint(origin + math::Vector3(0, 0, 0.1));
  connectVis->GetSceneNode()->setInheritScale(false);
  connectVis->GetSceneNode()->setInheritOrientation(false);

  ConnectionData *connectionData = new ConnectionData;
  connectionData->dirty = false;
  connectionData->visual = connectVis;
  connectionData->parent = _parent;
  connectionData->child = _child;
  connectionData->line = connectLine;
  connectionData->type = this->connectType;
  connectionData->line->setMaterial(
      this->connectionMaterials[connectionData->type]);
  return connectionData;
}


/*/////////////////////////////////////////////////
void CMLConnectionMaker::CreateConnection(
    sdf::ElementPtr _connectionElem, const std::string &_modelPreviewName)
{
  boost::recursive_mutex::scoped_lock lock(*this->updateMutex);
  std::pair<sdf::ElementPtr, std::string> pair(_connectionElem,
      _modelPreviewName);
  this->connectionsToAdd.push_back(pair);
}*/

/////////////////////////////////////////////////
void CMLConnectionMaker::CreateConnectionFromSDF(
    sdf::ElementPtr _connectionElem, const std::string &_modelName)
{
  std::string source = _connectionElem->Get<std::string>("source");
  std::string target = _connectionElem->Get<std::string>("target");
  std::string sourcePort = _connectionElem->Get<std::string>("source_port");
  std::string targetPort = _connectionElem->Get<std::string>("target_port");

 // source
  std::string sourceName = _modelName + "::" + source;
  rendering::VisualPtr sourceVis =
      gui::get_active_camera()->GetScene()->GetVisual(sourceName);
  if (!sourceVis)
  {
    std::string unscopedName =
        source.substr(source.find("::")+2);
    sourceVis = gui::get_active_camera()->GetScene()->GetVisual(
        _modelName + "::" + unscopedName);
  }
  // target
  std::string targetName = _modelName + "::" + target;
  rendering::VisualPtr targetVis =
      gui::get_active_camera()->GetScene()->GetVisual(targetName);
  if (!targetVis)
  {
    std::string unscopedName =
        target.substr(target.find("::")+2);
    targetVis = gui::get_active_camera()->GetScene()->GetVisual(
        _modelName + "::" + unscopedName);
  }

  if (!sourceVis || !targetVis)
  {
    std::cerr << "No source or target visual found" << std::endl;
    return;
  }

  this->connectType = CONNECT_ELECTRICAL;
  ConnectionData *connection = this->CreateConnection(sourceVis, targetVis);
  connection->parentPort = sourcePort;
  connection->childPort = targetPort;
  this->connectType = CONNECT_NONE;

  std::cerr << " create connection from sdf " <<
    sourceVis->GetName() << " " << targetVis->GetName() << std::endl;

  this->CreateHotSpot(connection);
  this->InsertConnectionElement(connection);
  gui::CMLEvents::connectionCreated(
      connection->parent->GetName(),
      connection->parentPort,
      connection->child->GetName(),
      connection->childPort);
}

/////////////////////////////////////////////////
void CMLConnectionMaker::AddConnection(const std::string &_type)
{
  if (_type == "electrical")
  {
    this->AddConnection(CMLConnectionMaker::CONNECT_ELECTRICAL);
  }
  else if (_type == "mechanical")
  {
    this->AddConnection(CMLConnectionMaker::CONNECT_MECHANICAL);
  }
  else if (_type == "none")
  {
    this->AddConnection(CMLConnectionMaker::CONNECT_NONE);
  }
}

/////////////////////////////////////////////////
void CMLConnectionMaker::AddConnection(CMLConnectionMaker::ConnectType _type)
{
  this->connectType = _type;
  if (_type != CMLConnectionMaker::CONNECT_NONE)
  {
    // Add an event filter, which allows the CMLConnectionMaker to capture
    // mouse events.
    MouseEventHandler::Instance()->AddMoveFilter("cml_connection",
        boost::bind(&CMLConnectionMaker::OnMouseMove, this, _1));

    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
  }
  else
  {
    // Remove the event filters.
    MouseEventHandler::Instance()->RemoveMoveFilter("cml_connection");

    // Press event added only after a connect is created. Needs to be added here
    // instead of in the constructor otherwise GLWidget would get the event
    // first and connection maker would not receive it.
    MouseEventHandler::Instance()->AddPressFilter("cml_connection",
        boost::bind(&CMLConnectionMaker::OnMousePress, this, _1));
  }
}

/////////////////////////////////////////////////
void CMLConnectionMaker::Stop()
{
  if (this->connectType != CMLConnectionMaker::CONNECT_NONE)
  {
    this->newConnectionCreated = false;
    if (this->mouseConnection)
    {
      this->mouseConnection->visual->DeleteDynamicLine(
          this->mouseConnection->line);
      rendering::ScenePtr scene = this->mouseConnection->visual->GetScene();
      scene->RemoveVisual(this->mouseConnection->visual);
      this->mouseConnection->visual.reset();
      delete this->mouseConnection;
      this->mouseConnection = NULL;
    }
    if (this->hoverVis)
      this->hoverVis->SetEmissive(common::Color(0, 0, 0));
    if (this->selectedVis)
      this->selectedVis->SetEmissive(common::Color(0, 0, 0));
    this->selectedVis.reset();
    this->hoverVis.reset();

    // this->AddConnection(CMLConnectionMaker::CONNECT_NONE);
    this->connectType = CMLConnectionMaker::CONNECT_NONE;

    // Remove the event filters.
    MouseEventHandler::Instance()->RemoveMoveFilter("cml_connection");

    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
  }
}

/////////////////////////////////////////////////
bool CMLConnectionMaker::OnMouseMove(const common::MouseEvent &_event)
{
  this->mouseEvent = _event;

  if (_event.Dragging())
    return false;

  // de-highlight previously highlighted visual
  if (this->hoverVis && this->hoverVis != this->selectedVis)
  {
    this->hoverVis->SetEmissive(common::Color(0.0, 0.0, 0.0));
  }

  // Get the active camera and scene.
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::ScenePtr scene = camera->GetScene();

  // Highlight visual on hover
  rendering::VisualPtr vis = camera->GetVisual(_event.Pos());

  if (!vis)
    return true;

  // Get lowest level visual that is a component model.
  rendering::VisualPtr componentVis = this->GetLowestLevelComponentVisual(vis);

  // check is not the currently selected one
  if (componentVis && componentVis != this->selectedVis)
  {
    this->hoverVis = componentVis;
    this->hoverVis->SetEmissive(common::Color(0.5, 0.5, 0.5));
  }
  else
  {
    this->hoverVis.reset();
  }

  // Case when a parent part is already selected and currently
  // extending the connect line to a child part
  if (this->selectedVis && this->mouseConnection && this->mouseConnection->line)
  {
    math::Vector3 parentPos, currentPos;
    if (this->mouseConnection->parent)
      parentPos = this->mouseConnection->parent->GetWorldPose().pos;

    // Set end point to center of child part
    if (this->hoverVis)
    {
      currentPos = this->hoverVis->GetWorldPose().pos - parentPos;
    }
    // Set end point to mouse plane intersection
    else if (!this->hoverVis && vis->GetRootVisual() &&
             vis->GetRootVisual()->IsPlane())
    {
      math::Vector3 pt;
      camera->GetWorldPointOnPlane(_event.Pos().X(), _event.Pos().Y(),
          math::Plane(math::Vector3(0, 0, 1)), pt);

      currentPos = vis->GetRootVisual()->GetWorldPose().pos - parentPos + pt;
    }
    if (currentPos != math::Vector3::Zero)
      this->mouseConnection->line->SetPoint(1, currentPos);
  }

  return true;
}

/////////////////////////////////////////////////
bool CMLConnectionMaker::OnKeyPress(const common::KeyEvent &_event)
{
  if (_event.key == Qt::Key_Delete)
  {
    if (this->selectedConnection)
    {
      this->RemoveConnection(this->selectedConnection->GetName());
      this->selectedConnection.reset();
    }
  }
  else if (_event.key == Qt::Key_Escape)
  {
    this->Stop();
  }
  return false;
}

/////////////////////////////////////////////////
void CMLConnectionMaker::CreateHotSpot(ConnectionData *_connect)
{
  if (!_connect)
    return;

  rendering::UserCameraPtr camera = gui::get_active_camera();

  std::string hotSpotName = _connect->visual->GetName() + "_HOTSPOT_";
  rendering::VisualPtr hotspotVisual(
      new rendering::Visual(hotSpotName, camera->GetScene()->GetWorldVisual(),
      false));

  _connect->hotspot = hotspotVisual;

  // create a cylinder to represent the connect
  hotspotVisual->InsertMesh("unit_cylinder");
  Ogre::MovableObject *hotspotObj =
      (Ogre::MovableObject*)(camera->GetScene()->GetManager()->createEntity(
      "__HOTSPOT__" + _connect->visual->GetName(), "unit_cylinder"));
  hotspotObj->setUserAny(Ogre::Any(hotSpotName));
  hotspotVisual->GetSceneNode()->attachObject(hotspotObj);
  hotspotVisual->SetMaterial(this->connectionMaterials[_connect->type]);
  hotspotVisual->SetTransparency(0.5);

  hotspotVisual->SetVisibilityFlags(GZ_VISIBILITY_GUI |
      GZ_VISIBILITY_SELECTABLE);
  hotspotVisual->GetSceneNode()->setInheritScale(false);

  this->connects[hotSpotName] = _connect;
  camera->GetScene()->AddVisual(hotspotVisual);

  // remove line as we are using a cylinder hotspot visual to
  // represent the connect
  _connect->visual->DeleteDynamicLine(_connect->line);

  _connect->dirty = true;
}

/////////////////////////////////////////////////
void CMLConnectionMaker::Update()
{
  boost::recursive_mutex::scoped_lock lock(*this->updateMutex);
  if (this->newConnectionCreated)
  {
    this->CreateHotSpot(this->mouseConnection);

    this->mouseConnection = NULL;
    this->newConnectionCreated = false;
  }

  /*while (!this->connectionsToAdd.empty())
  {
    std::pair<sdf::ElementPtr, std::string > p = this->connectionsToAdd.front();
    this->connectionsToAdd.pop_front();
    this->CreateConnectionFromSDF(p.first, p.second);
  }*/

  // update connect line and hotspot position.
  for (auto it : connects)
  {
    ConnectionData *connect = it.second;
    if (connect->hotspot)
    {
      if (connect->child && connect->parent)
      {
        bool poseUpdate = false;
        if (connect->parentPose != connect->parent->GetWorldPose() ||
            connect->childPose != connect->child->GetWorldPose())
         {
           connect->parentPose = connect->parent->GetWorldPose();
           connect->childPose = connect->child->GetWorldPose();
           poseUpdate = true;
         }

        if (connect->dirty || poseUpdate)
        {
        // get origin of parent part visuals
        math::Vector3 parentOrigin = connect->parent->GetWorldPose().pos;

        // get origin of child part visuals
        math::Vector3 childOrigin = connect->child->GetWorldPose().pos;

        // set orientation of connect hotspot
        math::Vector3 dPos = (childOrigin - parentOrigin);
        math::Vector3 center = dPos * 0.5;
        double length = std::max(dPos.GetLength(), 0.001);
        connect->hotspot->SetScale(math::Vector3(0.003, 0.003, length));
        connect->hotspot->SetWorldPosition(parentOrigin + center);
        math::Vector3 u = dPos.Normalize();
        math::Vector3 v = math::Vector3::UnitZ;
        double cosTheta = v.Dot(u);
        double angle = acos(cosTheta);
        math::Vector3 w = (v.Cross(u)).Normalize();
        math::Quaternion q;
        q.SetFromAxis(w, angle);
        connect->hotspot->SetWorldRotation(q);

        connect->dirty = false;
        }
      }
    }
  }
}

/////////////////////////////////////////////////
CMLConnectionMaker::ConnectType CMLConnectionMaker::GetState() const
{
  return this->connectType;
}

/*
/////////////////////////////////////////////////
unsigned int CMLConnectionMaker::GetConnectionCount()
{
  return this->connects.size();
}*/

/////////////////////////////////////////////////
void CMLConnectionMaker::OnFinish()
{
  this->Reset();
}

/////////////////////////////////////////////////
bool CMLConnectionMaker::IsComponent(const std::string &_name)
{
  // FIXME: this is a hack
  std::string leafName = _name;
  size_t pIdx = leafName.find_last_of("::");
  if (pIdx != std::string::npos)
    leafName = leafName.substr(pIdx+1);

  bool isComponent = false;
  if ((leafName.find("AA_battery") != std::string::npos ||
       leafName.find("motor") != std::string::npos ||
       leafName.find("switch") != std::string::npos) &&
       leafName.find("_HOTSPOT_") == std::string::npos)
  {
    isComponent = true;
  }
  return isComponent;
}

/////////////////////////////////////////////////
rendering::VisualPtr CMLConnectionMaker::GetLowestLevelComponentVisual(
    rendering::VisualPtr _visual)
{
  unsigned int depth = 2;
  rendering::VisualPtr modelVis;
  while (depth < _visual->GetDepth())
  {
    rendering::VisualPtr childVis = _visual->GetNthAncestor(depth);
    if (!childVis)
      break;

    std::string name = childVis->GetName();
    Simple_msgs::msgs::SimpleModel msg;
    msg = CMLManager::Instance()->GetModelInfo(name);
    if (!msg.name().empty())
      modelVis = childVis;
    depth++;
  }
  return modelVis;
}
