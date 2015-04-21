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

#include "CMLEditorItem.hh"
#include "CMLEditorView.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditorView::CMLEditorView(QWidget *_parent)
  : QGraphicsView(_parent)
{
  this->setObjectName("CMLEditorView");
  this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  //this->gridLines = NULL;

  this->viewScale = 1.0;
}

/////////////////////////////////////////////////
CMLEditorView::~CMLEditorView()
{
}

/////////////////////////////////////////////////
void CMLEditorView::scrollContentsBy(int _dx, int _dy)
{
  QGraphicsView::scrollContentsBy(_dx, _dy);
}

/////////////////////////////////////////////////
void CMLEditorView::resizeEvent(QResizeEvent *_event)
{
}

/////////////////////////////////////////////////
void CMLEditorView::contextMenuEvent(QContextMenuEvent *_event)
{

  QGraphicsItem *item = this->scene()->itemAt(this->mapToScene(_event->pos()));
  if (item)
  {
    _event->ignore();
    QGraphicsView::contextMenuEvent(_event);
    return;
  }

/*  QMenu menu(this);
  //menu.addAction(this->addLevelAct);
  menu.exec(_event->globalPos());
  _event->accept();*/
}

/////////////////////////////////////////////////
void CMLEditorView::wheelEvent(QWheelEvent *_event)
{
  int numSteps = (_event->delta()/8) / 15;
  QMatrix mat = matrix();
  QPointF mousePosition = _event->pos();

  mat.translate((width()/2) - mousePosition.x(), (height()/2) -
    mousePosition.y());

  double scaleFactor = 1.15;

  if (numSteps > 0)
  {
    mat.scale(numSteps*scaleFactor, numSteps*scaleFactor);
    this->viewScale *= numSteps*scaleFactor;
  }
  else
  {
    mat.scale(-1/(numSteps*scaleFactor), -1/(numSteps*scaleFactor));
    this->viewScale *= -1/(numSteps*scaleFactor);
  }
  mat.translate(mousePosition.x() - (this->width()/2),
      mousePosition.y() -(this->height()/2));
  this->setMatrix(mat);

  _event->accept();
}

/////////////////////////////////////////////////
void CMLEditorView::mousePressEvent(QMouseEvent *_event)
{
  if (_event->button() != Qt::RightButton)
  {
    QGraphicsItem *mouseItem =
        this->scene()->itemAt(this->mapToScene(_event->pos()));
    if (mouseItem && !mouseItem->isSelected())
    {
      CMLEditorItem *item = dynamic_cast<CMLEditorItem *>(mouseItem);
      if (item)
      {
        this->scene()->clearSelection();
        mouseItem->setSelected(true);
      }
    }
    QGraphicsView::mousePressEvent(_event);
  }
}

/////////////////////////////////////////////////
void CMLEditorView::mouseReleaseEvent(QMouseEvent *_event)
{
  QGraphicsView::mouseReleaseEvent(_event);
}

/////////////////////////////////////////////////
void CMLEditorView::mouseMoveEvent(QMouseEvent *_event)
{
  QGraphicsView::mouseMoveEvent(_event);
}

/////////////////////////////////////////////////
void CMLEditorView::keyPressEvent(QKeyEvent *_event)
{
  if (_event->key() == Qt::Key_Delete || _event->key() == Qt::Key_Backspace)
  {
  }
  else if (_event->key() == Qt::Key_Escape)
  {
  }
}

/////////////////////////////////////////////////
void CMLEditorView::mouseDoubleClickEvent(QMouseEvent *_event)
{
  QGraphicsView::mouseDoubleClickEvent(_event);
}