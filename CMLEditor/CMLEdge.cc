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

#include <QPainter>

#include "CMLEdge.hh"
#include "CMLNode.hh"

#include <math.h>

using namespace gazebo;
using namespace gui;

 static const double Pi = 3.14159265358979323846264338327950288419717;
 static double TwoPi = 2.0 * Pi;

/////////////////////////////////////////////////
CMLEdge::CMLEdge(CMLNode *sourceNode, CMLNode *destNode)
   : arrowSize(10)
{
  setAcceptedMouseButtons(0);
  source = sourceNode;
  dest = destNode;
  source->AddEdge(this);
  dest->AddEdge(this);
  this->Adjust();
}

/////////////////////////////////////////////////
CMLEdge::~CMLEdge()
{
//  std::cerr << "edge destructor " << std::endl;
//  this->source->RemoveEdge(this);
//  this->dest->RemoveEdge(this);
}

/////////////////////////////////////////////////
CMLNode *CMLEdge::GetSourceNode() const
{
  return source;
}

/////////////////////////////////////////////////
CMLNode *CMLEdge::GetDestNode() const
{
  return dest;
}

/////////////////////////////////////////////////
void CMLEdge::Adjust()
{
  if (!source || !dest)
    return;

  QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
  qreal length = line.length();

  prepareGeometryChange();

  if (length > qreal(20.))
  {
    QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
    sourcePoint = line.p1() + edgeOffset;
    destPoint = line.p2() - edgeOffset;
  } else
  {
    sourcePoint = destPoint = line.p1();
  }
}

/////////////////////////////////////////////////
QRectF CMLEdge::boundingRect() const
{
  if (!source || !dest)
    return QRectF();

  qreal penWidth = 1;
  qreal extra = (penWidth + arrowSize) / 2.0;

  return QRectF(sourcePoint,
    QSizeF(destPoint.x() - sourcePoint.x(), destPoint.y() - sourcePoint.y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

/////////////////////////////////////////////////
void CMLEdge::paint(QPainter *_painter,
    const QStyleOptionGraphicsItem */*_option*/, QWidget */*_widget*/)
{
  if (!source || !dest)
    return;

  QLineF line(sourcePoint, destPoint);
  if (qFuzzyCompare(line.length(), qreal(0.)))
     return;

  // Draw the line itself
  _painter->setPen(
      QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  _painter->drawLine(line);

  // Draw the arrows
  double angle = ::acos(line.dx() / line.length());
  if (line.dy() >= 0)
     angle = TwoPi - angle;

  QPointF sourceArrowP1 = sourcePoint +
      QPointF(sin(angle + Pi / 3) * arrowSize,
      cos(angle + Pi / 3) * arrowSize);
  QPointF sourceArrowP2 = sourcePoint +
      QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
      cos(angle + Pi - Pi / 3) * arrowSize);
  QPointF destArrowP1 = destPoint +
      QPointF(sin(angle - Pi / 3) * arrowSize,
      cos(angle - Pi / 3) * arrowSize);
  QPointF destArrowP2 = destPoint +
      QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
      cos(angle - Pi + Pi / 3) * arrowSize);

  _painter->setBrush(Qt::black);
  _painter->drawPolygon(
      QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
  _painter->drawPolygon(
      QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
