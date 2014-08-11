/*
 * Copyright 2013 Open Source Robotics Foundation
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

#include "CMLRender.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLRender::CMLRender()
{
  MouseEventHandler::Instance()->AddDoubleClickFilter("CML_Render",
    boost::bind(&CMLRender::OnMouseDoubleClick, this, _1));

  MouseEventHandler::Instance()->AddPressFilter("CML_Render",
      boost::bind(&CMLRender::OnMousePress, this, _1));

  // Add an event filter, which allows the CMLRender to capture mouse events.
  MouseEventHandler::Instance()->AddReleaseFilter("CML_Render",
      boost::bind(&CMLRender::OnMouseRelease, this, _1));

  MouseEventHandler::Instance()->AddMoveFilter("CML_Render",
      boost::bind(&CMLRender::OnMouseMove, this, _1));

  this->connections.push_back(
      event::Events::ConnectPreRender(
        boost::bind(&CMLRender::Update, this)));
}

/////////////////////////////////////////////////
CMLRender::~CMLRender()
{
  MouseEventHandler::Instance()->RemovePressFilter("CML_Render");
  MouseEventHandler::Instance()->RemoveReleaseFilter("CML_Render");
  MouseEventHandler::Instance()->RemoveMoveFilter("CML_Render");
  MouseEventHandler::Instance()->RemoveDoubleClickFilter("CML_Render");
}

/////////////////////////////////////////////////
bool CMLRender::OnMousePress(const common::MouseEvent &_event)
{
  if (_event.button != common::MouseEvent::LEFT)
    return false;

  return false;
}

/////////////////////////////////////////////////
bool CMLRender::OnMouseRelease(const common::MouseEvent &_event)
{
  std::cerr << " intercepting mouse release events " << std::endl;
  // Get the active camera and scene.
  rendering::UserCameraPtr camera = gui::get_active_camera();
  rendering::ScenePtr scene = camera->GetScene();
  rendering::VisualPtr vis = camera->GetVisual(_event.pos);

  if (_event.button == common::MouseEvent::RIGHT)
  {
    if (vis)
    {
        // QMenu menu;
        // menu.addAction(this->inspectAct);
        // menu.exec(QCursor::pos());
        // return true;
    }
  }

  if (_event.button != common::MouseEvent::LEFT)
    return false;


  return false;
}

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

}
