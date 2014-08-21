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

#include <iostream>

 #include "CMLEdge.hh"
 #include "CMLNode.hh"

#include "CMLEditorItem.hh"
#include "CMLEditorScene.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditorScene::CMLEditorScene(QWidget *_parent)
  : QGraphicsScene(_parent)
{
  this->setObjectName("CMLEditorScene");
  this->timerId = 0;
}

/////////////////////////////////////////////////
CMLEditorScene::~CMLEditorScene()
{
}

/////////////////////////////////////////////////
CMLNode *CMLEditorScene::AddNode(const std::string &_name)
{
  if (this->nodes.find(_name) != this->nodes.end())
  {
    std::cerr << "A node with the name: '" << _name <<  "' already exists"
        << std::endl;
    return NULL;
  }

  CMLNode *node = new CMLNode(_name, this);
  this->nodes[_name] = node;

  this->addItem(node);

  return node;
}

/////////////////////////////////////////////////
void CMLEditorScene::RemoveNode(const std::string &_name)
{
  if (this->nodes.find(_name) != this->nodes.end())
    this->removeItem(this->nodes[_name]);
}

/////////////////////////////////////////////////
bool CMLEditorScene::HasNode(const std::string &_name)
{
  return (this->nodes.find(_name) != this->nodes.end());
}

/////////////////////////////////////////////////
CMLNode *CMLEditorScene::GetNode(const std::string &_name)
{
  if (this->nodes.find(_name) != this->nodes.end())
    return this->nodes[_name];
  else
    return NULL;
}

/////////////////////////////////////////////////
CMLEdge *CMLEditorScene::AddEdge(const std::string &_node1,
    const std::string &_node2)
{
  if (this->nodes.find(_node1) == this->nodes.end())
  {
    std::cerr << "Unable to find node: '" << _node1 <<
        "' when creating an edge" << std::endl;
    return NULL;
  }

  if (this->nodes.find(_node2) == this->nodes.end())
  {
    std::cerr << "Unable to find node: '" << _node2 <<
        "' when creating an edge" << std::endl;
    return NULL;
  }

  CMLNode *node1 = this->nodes[_node1];
  CMLNode *node2 = this->nodes[_node2];
  CMLEdge *edge = new CMLEdge(node1, node2);
  this->addItem(edge);

  return edge;
}

/////////////////////////////////////////////////
void CMLEditorScene::drawBackground(QPainter * _painter, const QRectF & _rect)
{
    const int gridSize = 25;

    const qreal left = int(_rect.left()) - (int(_rect.left()) % gridSize);
    const qreal top = int(_rect.top()) - (int(_rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < _rect.right(); x += gridSize)
        lines.append(QLineF(x, _rect.top(), x, _rect.bottom()));
    for (qreal y = top; y < _rect.bottom(); y += gridSize)
        lines.append(QLineF(_rect.left(), y, _rect.right(), y));

    _painter->setRenderHint(QPainter::Antialiasing, false);

    _painter->setPen(QColor(Qt::lightGray).lighter(50));
    _painter->drawLines(lines.data(), lines.size());
    _painter->setPen(Qt::black);
    //_painter->drawRect(sceneRect());
}

/////////////////////////////////////////////////
void CMLEditorScene::itemMoved()
{
   if (!timerId)
       timerId = startTimer(1000 / 25);
}

/////////////////////////////////////////////////
void CMLEditorScene::timerEvent(QTimerEvent *event)
{
   Q_UNUSED(event);

   QList<CMLNode *> nodes;
   foreach (QGraphicsItem *item, this->items()) {
     if (CMLNode *node = qgraphicsitem_cast<CMLNode *>(item))
       nodes << node;
   }

   foreach (CMLNode *node, nodes)
       node->calculateForces();

   bool itemsMoved = false;
   foreach (CMLNode *node, nodes) {
       if (node->advance())
           itemsMoved = true;
   }

   if (!itemsMoved) {
       killTimer(timerId);
       timerId = 0;
   }
}
