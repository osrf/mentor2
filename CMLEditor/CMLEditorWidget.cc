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

#include "gazebo/common/Events.hh"
#include "gazebo/rendering/RenderEvents.hh"
#include <gazebo/transport/Node.hh>

//#include "QGVCore/QGVScene.h"

#include "SimpleModel.pb.h"

#include "CMLNode.hh"
#include "CMLManager.hh"
#include "CMLEditorScene.hh"
#include "CMLEditorView.hh"
#include "CMLEditorWidget.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditorWidget::CMLEditorWidget(QWidget *_parent)
  : QWidget(_parent)
{
  this->setObjectName("CMLEditorWidget");

  this->scene = new CMLEditorScene(this);
  /*this->scene = new QGVScene("qgvscene", this);

  //Configure scene attributes
  //this->scene->setGraphAttribute("label", "qgvscene");
  this->scene->setGraphAttribute("splines", "ortho");
  this->scene->setGraphAttribute("rankdir", "LR");
  //_scene->setGraphAttribute("concentrate", "true"); //Error !
  this->scene->setGraphAttribute("nodesep", "0.4");

  this->scene->setNodeAttribute("shape", "box");
  this->scene->setNodeAttribute("style", "filled");
  this->scene->setNodeAttribute("fillcolor", "white");
  this->scene->setNodeAttribute("height", "1.2");
  this->scene->setEdgeAttribute("minlen", "3");*/

  CMLEditorView *view = new CMLEditorView(_parent);

  /*QColor c(220, 220, 220);
  QBrush brush(c, Qt::SolidPattern);
  view->setBackgroundBrush(brush);*/

/*  this->minimumWidth = 1240*2;
  this->minimumHeight = 1024*2;
  this->scene->setSceneRect(-this->minimumWidth/2, -this->minimumHeight/2,
      this->minimumWidth, this->minimumHeight);*/
  QHBoxLayout *canvasLayout = new QHBoxLayout(this);
  canvasLayout->addWidget(view);
  canvasLayout->setAlignment(Qt::AlignHCenter);

  view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  view->setScene(this->scene);
  view->centerOn(QPointF(0, 0));
  view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  view->setDragMode(QGraphicsView::ScrollHandDrag);
  view->show();

  canvasLayout->setContentsMargins(0, 0, 0, 0);
  canvasLayout->setSpacing(0);
  this->setLayout(canvasLayout);

  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();

  this->simpleConnectionSub = this->node->Subscribe("~/simple/connection",
      &CMLEditorWidget::OnSimpleConnection, this);

  this->simpleModelSub = this->node->Subscribe("~/simple/model/info",
      &CMLEditorWidget::OnSimpleModel, this);

  this->modelInfoSub = this->node->Subscribe("~/model/info",
      &CMLEditorWidget::OnModelMsg, this);

  this->sceneSub = this->node->Subscribe("~/scene",
      &CMLEditorWidget::OnSceneMsg, this);

  // request response for getting the scene msg
  this->requestPub = this->node->Advertise<msgs::Request>("~/request");
  this->responseSub = this->node->Subscribe("~/response",
      &CMLEditorWidget::OnResponse, this, true);

  this->receiveMutex = new boost::mutex();
  this->connections.push_back(event::Events::ConnectPreRender(
      boost::bind(&CMLEditorWidget::PreRender, this)));

  this->requestPub->WaitForConnection();
  this->requestMsg = msgs::CreateRequest("scene_info");
  this->requestPub->Publish(*this->requestMsg);

  this->requestSub = this->node->Subscribe("~/request",
      &CMLEditorWidget::OnRequest, this);
}

/////////////////////////////////////////////////
CMLEditorWidget::~CMLEditorWidget()
{
  delete this->requestMsg;
  this->requestMsg = NULL;
}

/////////////////////////////////////////////////
void CMLEditorWidget::OnRequest(ConstRequestPtr &_msg)
{
  if (_msg->request() == "entity_delete")
  {
    boost::mutex::scoped_lock lock(*this->receiveMutex);
    this->removeEntityList.insert(_msg->data());
  }
}

/////////////////////////////////////////////////
void CMLEditorWidget::resizeEvent(QResizeEvent *_event)
{
  qreal boundingWidth = std::max(this->minimumWidth, _event->size().width());
  boundingWidth = std::max(boundingWidth, this->scene->sceneRect().width());
  qreal boundingHeight = std::max(this->minimumHeight,
      _event->size().height());
  boundingHeight = std::max(boundingHeight, this->scene->sceneRect().height());
  this->scene->setSceneRect(-boundingWidth/2, -boundingHeight/2,
      boundingWidth, boundingHeight);
}

/////////////////////////////////////////////////
void CMLEditorWidget::OnSceneMsg(ConstScenePtr &_msg)
{
  boost::mutex::scoped_lock lock(*this->receiveMutex);
  this->sceneMsgs.push_back(_msg);
}

/////////////////////////////////////////////////
void CMLEditorWidget::OnModelMsg(ConstModelPtr &_msg)
{
  boost::mutex::scoped_lock lock(*this->receiveMutex);
  this->modelMsgs.push_back(_msg);
}

//////////////////////////////////////////////////
void CMLEditorWidget::OnSimpleModel(ConstSimpleModelPtr &_msg)
{
  boost::mutex::scoped_lock lock(*this->receiveMutex);
  this->simpleModelMsgs.push_back(_msg);
}

//////////////////////////////////////////////////
void CMLEditorWidget::OnSimpleConnection(ConstSimpleConnectionPtr &_msg)
{
  boost::mutex::scoped_lock lock(*this->receiveMutex);
  this->simpleConnectionMsgs.push_back(_msg);
}

/////////////////////////////////////////////////
void CMLEditorWidget::OnResponse(ConstResponsePtr &_msg)
{
  if (!this->requestMsg || _msg->id() != this->requestMsg->id())
    return;

  msgs::Scene sceneMsg;
  sceneMsg.ParseFromString(_msg->serialized_data());
  boost::shared_ptr<msgs::Scene> sm(new msgs::Scene(sceneMsg));
  this->sceneMsgs.push_back(sm);
  this->requestMsg = NULL;
}

/////////////////////////////////////////////////
bool CMLEditorWidget::ProcessSceneMsg(ConstScenePtr &_msg)
{
  for (int i = 0; i < _msg->model_size(); ++i)
    this->ProcessModelMsg(_msg->model(i));

  return true;
}

/////////////////////////////////////////////////
bool CMLEditorWidget::ProcessModelMsg(const msgs::Model &_msg)
{
  //QGVNode *node = this->scene->addNode(tr(_msg.name().c_str()));
  if (!this->scene->HasNode(_msg.name()))
  {
    CMLNode * node = this->scene->AddNode(_msg.name());
    Simple_msgs::msgs::SimpleModel msg =
        CMLManager::Instance()->GetModelInfo(_msg.name());
    if (!msg.name().empty() && !msg.schematic_type().empty())
      node->SetIcon(this->GetSchematicIcon(msg.schematic_type()));
    this->scene->itemMoved();
    this->scene->advance();
  }


  //this->graphNodes.push_back(node);

  //this->scene->clearLayout();
  //Layout scene
  //this->scene->applyLayout();

  //Fit in view
  //ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
  return true;
}

/////////////////////////////////////////////////
bool CMLEditorWidget::ProcessSimpleModelMsg(
    const Simple_msgs::msgs::SimpleModel &_msg)
{
  CMLNode *node = this->scene->GetNode(_msg.name());
  if (node && node->pixmap().isNull())
  {
    Simple_msgs::msgs::SimpleModel msg =
        CMLManager::Instance()->GetModelInfo(_msg.name());
    if (!msg.name().empty() && !msg.schematic_type().empty())
      node->SetIcon(this->GetSchematicIcon(msg.schematic_type()));
  }
}

/////////////////////////////////////////////////
bool CMLEditorWidget::ProcessSimpleConnectionMsg(
    const Simple_msgs::msgs::SimpleConnection &_msg)
{
  this->scene->AddEdge(_msg.parent(), _msg.child());
  return true;
}


//////////////////////////////////////////////////
bool CMLEditorWidget::ProcessRemoveEntity(const std::string &_name)
{
  this->scene->RemoveNode(_name);
  return true;
}

//////////////////////////////////////////////////
void CMLEditorWidget::PreRender()
{
  static SceneMsgs_L::iterator sIter;
  static ModelMsgs_L::iterator modelIter;
  static SimpleConnectionMsgs_L::iterator simpleConnectionIter;
  static SimpleModelMsgs_L::iterator simpleModelIter;

  SceneMsgs_L sceneMsgsCopy;
  ModelMsgs_L modelMsgsCopy;
  SimpleConnectionMsgs_L simpleConnectionMsgsCopy;
  SimpleModelMsgs_L simpleModelMsgsCopy;
  std::set<std::string> removeEntityCopy;

  {
    boost::mutex::scoped_lock lock(*this->receiveMutex);

    std::copy(this->sceneMsgs.begin(), this->sceneMsgs.end(),
        std::back_inserter(sceneMsgsCopy));
    this->sceneMsgs.clear();

    std::copy(this->modelMsgs.begin(), this->modelMsgs.end(),
        std::back_inserter(modelMsgsCopy));
    this->modelMsgs.clear();

    std::copy(this->simpleConnectionMsgs.begin(),
        this->simpleConnectionMsgs.end(),
        std::back_inserter(simpleConnectionMsgsCopy));
    this->simpleConnectionMsgs.clear();

    std::copy(this->simpleModelMsgs.begin(),
        this->simpleModelMsgs.end(),
        std::back_inserter(simpleModelMsgsCopy));
    this->simpleModelMsgs.clear();

    removeEntityCopy = this->removeEntityList;
    this->removeEntityList.clear();
  }

  // Process the scene messages. DO THIS FIRST
  for (sIter = sceneMsgsCopy.begin(); sIter != sceneMsgsCopy.end();)
  {
    if (this->ProcessSceneMsg(*sIter))
      sceneMsgsCopy.erase(sIter++);
    else
      ++sIter;
  }

  // Process the model messages.
  for (modelIter = modelMsgsCopy.begin(); modelIter != modelMsgsCopy.end();)
  {
    if (this->ProcessModelMsg(**modelIter))
      modelMsgsCopy.erase(modelIter++);
    else
      ++modelIter;
  }

  // Process the simple connection messages.
  for (simpleConnectionIter = simpleConnectionMsgsCopy.begin();
      simpleConnectionIter != simpleConnectionMsgsCopy.end();)
  {
    if (this->ProcessSimpleConnectionMsg(**simpleConnectionIter))
      simpleConnectionMsgsCopy.erase(simpleConnectionIter++);
    else
      ++simpleConnectionIter;
  }

  // Process the simple model messages.
  for (simpleModelIter = simpleModelMsgsCopy.begin();
      simpleModelIter != simpleModelMsgsCopy.end();)
  {
    if (this->ProcessSimpleModelMsg(**simpleModelIter))
      simpleModelMsgsCopy.erase(simpleModelIter++);
    else
      ++simpleModelIter;
  }

  // Process the remove entity messages.
  for (std::set<std::string>::iterator removeEntityIter
      = removeEntityCopy.begin(); removeEntityIter != removeEntityCopy.end();
      ++removeEntityIter)
  {
    this->ProcessRemoveEntity(*removeEntityIter);
  }
  removeEntityCopy.clear();
}

//////////////////////////////////////////////////
std::string CMLEditorWidget::GetSchematicIcon(const std::string &_schematicType)
{
  if (_schematicType == "power")
    return ":/images/cell.png";
  else if (_schematicType == "switch")
    return ":/images/switch1.png";
  else if (_schematicType == "motor")
    return ":/images/motor.png";
}
