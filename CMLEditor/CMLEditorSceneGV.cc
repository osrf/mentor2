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

//#include "QGVEdge.h"
//#include "QGVNode.h"


//#include "CMLEditorItem.hh"
#include "CMLEditorSceneGV.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditorSceneGV::CMLEditorSceneGV(QWidget *_parent)
  : QGVScene("qgvscene", _parent)
{
  this->setObjectName("CMLEditorSceneGV");
//  this->timerId = 0;

  //Configure scene attributes
  //this->scene->setGraphAttribute("label", "qgvscene");
  this->setGraphAttribute("splines", "ortho");
  this->setGraphAttribute("rankdir", "LR");
  //_scene->setGraphAttribute("concentrate", "true"); //Error !
  this->setGraphAttribute("nodesep", "0.4");
  this->setNodeAttribute("shape", "box");
  this->setNodeAttribute("style", "filled");
  this->setNodeAttribute("fillcolor", "white");
  this->setNodeAttribute("height", "1.2");
  this->setEdgeAttribute("minlen", "3");
}

/////////////////////////////////////////////////
CMLEditorSceneGV::~CMLEditorSceneGV()
{
}

/////////////////////////////////////////////////
QGVNode *CMLEditorSceneGV::AddNode(const std::string &_name)
{
  // Note don't show the ground plane
  if (_name.find("plane") != std::string::npos)
    return NULL;

/*  if (this->nodes.find(_name) != this->nodes.end())
  {
    std::cerr << "A node with the name: '" << _name <<  "' already exists"
        << std::endl;
    return NULL;
  }*/

  QGVNode *node = this->addNode(tr(_name.c_str()));

  //CMLNode *node = new CMLNode(_name, this);
//  this->nodes[_name] = node;

  //this->addItem(node);

  return node;
}

/////////////////////////////////////////////////
void CMLEditorSceneGV::RemoveNode(const std::string &_name)
{
/*  if (this->nodes.find(_name) != this->nodes.end())
  {
    CMLNode *node = this->nodes[_name];
    QList<CMLEdge *> edges = node->GetEdges();
    for (int i = 0; i < edges.size(); ++i)
    {
      if (edges[i]->GetSourceNode() == node)
      {
        edges[i]->GetDestNode()->RemoveEdge(edges[i]);
      }
      else
      {
        edges[i]->GetSourceNode()->RemoveEdge(edges[i]);
      }

      this->removeItem(edges[i]);
    }
    this->removeItem(node);
    this->nodes.erase(_name);
  }*/

  this->removeNode(tr(_name.c_str()));
}

/////////////////////////////////////////////////
bool CMLEditorSceneGV::HasNode(const std::string &_name)
{
  return this->hasNode(tr(_name.c_str()));
//  return (this->nodes.find(_name) != this->nodes.end());
}

/////////////////////////////////////////////////
QGVNode *CMLEditorSceneGV::GetNode(const std::string &_name)
{
/*  if (this->nodes.find(_name) != this->nodes.end())
    return this->nodes[_name];
  else
    return NULL;*/

  return this->getNode(tr(_name.c_str()));
}

/////////////////////////////////////////////////
QGVEdge *CMLEditorSceneGV::AddEdge(const std::string &_node1,
    const std::string &_node2)
{
/*  if (this->nodes.find(_node1) == this->nodes.end())
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
  this->addItem(edge);*/

  QGVEdge *edge = this->addEdge(tr(_node1.c_str()), tr(_node2.c_str()));

  return edge;
}

/////////////////////////////////////////////////
void CMLEditorSceneGV::RemoveEdge(const std::string &_node1,
    const std::string &_node2)
{
  this->removeEdge(tr(_node1.c_str()), tr(_node2.c_str()));
}

/////////////////////////////////////////////////
void CMLEditorSceneGV::drawBackground(QPainter * _painter, const QRectF & _rect)
{
  QColor c(250, 250, 250);
  QBrush brush(c, Qt::SolidPattern);
  _painter->setBrush(brush);
  _painter->drawRect(_rect);

  const int gridSize = 25;

  const qreal left = int(_rect.left()) - (int(_rect.left()) % gridSize);
  const qreal top = int(_rect.top()) - (int(_rect.top()) % gridSize);

  QVarLengthArray<QLineF, 100> lines;

  for (qreal x = left; x < _rect.right(); x += gridSize)
      lines.append(QLineF(x, _rect.top(), x, _rect.bottom()));
  for (qreal y = top; y < _rect.bottom(); y += gridSize)
      lines.append(QLineF(_rect.left(), y, _rect.right(), y));

  _painter->setRenderHint(QPainter::Antialiasing, false);

  _painter->setPen(QColor(200, 200, 200, 125));
  _painter->drawLines(lines.data(), lines.size());
  _painter->setPen(Qt::black);
}
/*
/////////////////////////////////////////////////
void CMLEditorSceneGV::itemMoved()
{
   if (!timerId)
       timerId = startTimer(1000 / 25);
}

/////////////////////////////////////////////////
void CMLEditorSceneGV::timerEvent(QTimerEvent *event)
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
}*/
