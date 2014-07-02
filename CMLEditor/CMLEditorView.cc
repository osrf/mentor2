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
  QMenu menu(this);
  //menu.addAction(this->addLevelAct);
  menu.exec(_event->globalPos());
  _event->accept();
}

/////////////////////////////////////////////////
void CMLEditorView::wheelEvent(QWheelEvent *_event)
{
  _event->accept();
}

/////////////////////////////////////////////////
void CMLEditorView::mousePressEvent(QMouseEvent *_event)
{
  QGraphicsView::mousePressEvent(_event);
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
