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
 #include "CMLEditorScene.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLNode::CMLNode(const std::string &_name, CMLEditorScene *_scene)
   : graph(_scene)
{
  this->setFlag(ItemIsMovable);
  this->setFlag(ItemSendsGeometryChanges);
  this->setCacheMode(DeviceCoordinateCache);
  this->setZValue(-1);
  this->name = _name;
  //this->setPlainText(tr(this->name.c_str()));
}

/////////////////////////////////////////////////
void CMLNode::AddEdge(CMLEdge *_edge)
{
  edgeList << _edge;
  _edge->Adjust();
}

/////////////////////////////////////////////////
QList<CMLEdge *> CMLNode::GetEdges() const
{
  return edgeList;
}

/////////////////////////////////////////////////
void CMLNode::calculateForces()
{
  if (!scene() || scene()->mouseGrabberItem() == this)
  {
    newPos = pos();
    return;
  }

  // Sum up all forces pushing this item away
  qreal xvel = 0;
  qreal yvel = 0;
  foreach (QGraphicsItem *item, scene()->items())
  {
    CMLNode *node = qgraphicsitem_cast<CMLNode *>(item);
    if (!node)
       continue;

    QPointF vec = mapToItem(node, 0, 0);
    qreal dx = vec.x();
    qreal dy = vec.y();
    double l = 2.0 * (dx * dx + dy * dy);
    if (l > 0)
    {
      xvel += (dx * 150.0) / l;
      yvel += (dy * 150.0) / l;
    }
  }

  // Now subtract all forces pulling items together
  double weight = (edgeList.size() + 1) * 10;
  foreach (CMLEdge *edge, edgeList)
  {
     QPointF vec;
     if (edge->GetSourceNode() == this)
         vec = mapToItem(edge->GetDestNode(), 0, 0);
     else
         vec = mapToItem(edge->GetSourceNode(), 0, 0);
     xvel -= vec.x() / weight;
     yvel -= vec.y() / weight;
  }

  if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
     xvel = yvel = 0;

  QRectF sceneRect = scene()->sceneRect();
  newPos = pos() + QPointF(xvel, yvel);
  newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10),
      sceneRect.right() - 10));
  newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10),
      sceneRect.bottom() - 10));
}

/////////////////////////////////////////////////
bool CMLNode::advance()
{
  if (newPos == pos())
    return false;

  setPos(newPos);
  return true;
}

/////////////////////////////////////////////////
QRectF CMLNode::boundingRect() const
{
  /*qreal adjust = 2;
  return QRectF( -10 - adjust, -10 - adjust,
               23 + adjust, 23 + adjust);*/

  QRectF rect = QGraphicsPixmapItem::boundingRect();

  if (this->textBoundingRect.width() > 0)
    return rect.united(this->textBoundingRect);
  else return rect;
//  return rect;
}

/////////////////////////////////////////////////
QPainterPath CMLNode::shape() const
{
  QPainterPath path;
  path.addRect(this->boundingRect());
  return path;
}

/////////////////////////////////////////////////
void CMLNode::paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option,
    QWidget *_widget)
{

  QRectF pixRect = QGraphicsPixmapItem::boundingRect();
  _painter->save();
  _painter->setPen(Qt::NoPen);
  _painter->setBrush(Qt::darkGray);
  _painter->drawRect(pixRect);

  QRadialGradient gradient(-3, -3, 10);
  if (_option->state & QStyle::State_Sunken)
  {
    QPointF center = pixRect.center();
    gradient.setCenter(center.x(), center.y());
    gradient.setFocalPoint(center.x(), center.y());
    gradient.setColorAt(1, QColor(230, 230, 230).light(120));
    gradient.setColorAt(0, QColor(200, 200, 200).light(120));
  } else
  {
    gradient.setColorAt(1, QColor(230, 230, 230));
    gradient.setColorAt(0, QColor(200, 200, 200));
  }
  _painter->setBrush(gradient);

  _painter->setPen(QPen(Qt::black, 0));
  _painter->drawRect(pixRect);


/*  QFont font;
  font.setPixelSize(20);
  font.setBold(false);
  font.setFamily("Calibri");

  QPainterPath path;
  QPointF center = this->boundingRect().center();
  path.addText(center.x(), center.y(), font,  "Hello World!!");
//  _painter->drawPath(path);*/

  int margin = 6;
  QRectF textRect = pixRect;
  double halfHeight = pixRect.height()/2;
  textRect.moveTop(pixRect.height());
  textRect.setHeight(halfHeight);

  QString nameText(this->name.c_str());
  QFont font = _painter->font();
  QFontMetrics fm(font);
  int w = fm.width(nameText) + margin;
  int dw = textRect.width() - w;
  textRect.setWidth(w);
  textRect.moveLeft(dw/2);

  if (this->pixmap().isNull())
  {
    int h = fm.height() + margin;
    textRect.setHeight(h);
    _painter->drawRect(textRect);
  }

  this->textBoundingRect = textRect;

  _painter->drawText(textRect, Qt::AlignCenter, nameText);


  _painter->restore();

  QGraphicsPixmapItem::paint(_painter, _option, _widget);

  /*_painter->setPen(Qt::NoPen);
  _painter->setBrush(Qt::darkGray);
  _painter->drawEllipse(-7, -7, 20, 20);

  QRadialGradient gradient(-3, -3, 10);
  if (_option->state & QStyle::State_Sunken)
  {
    gradient.setCenter(3, 3);
    gradient.setFocalPoint(3, 3);
    gradient.setColorAt(1, QColor(Qt::yellow).light(120));
    gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
  } else
  {
    gradient.setColorAt(0, Qt::yellow);
    gradient.setColorAt(1, Qt::darkYellow);
  }
  _painter->setBrush(gradient);

  _painter->setPen(QPen(Qt::black, 0));
  _painter->drawEllipse(-10, -10, 20, 20);*/
}

/////////////////////////////////////////////////
QVariant CMLNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
  switch (change)
  {
    case ItemPositionHasChanged:
      foreach (CMLEdge *edge, edgeList)
         edge->Adjust();
      graph->itemMoved();
      break;
    default:
      break;
  };

  return QGraphicsItem::itemChange(change, value);
}

/////////////////////////////////////////////////
void CMLNode::mousePressEvent(QGraphicsSceneMouseEvent *_event)
{
  update();
  QGraphicsPixmapItem::mousePressEvent(_event);
}

/////////////////////////////////////////////////
void CMLNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *_event)
{
  update();
  QGraphicsPixmapItem::mouseReleaseEvent(_event);
}

/////////////////////////////////////////////////
void CMLNode::SetIcon(const std::string &_url)
{
  this->setPixmap(QPixmap(QString(_url.c_str())));
}
