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
#include "CMLEditorWidget.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditorWidget::CMLEditorWidget(QWidget *_parent)
  : QWidget(_parent)
{
  this->setObjectName("CMLEditorWidget");

  CMLEditorView *view = new CMLEditorView();
  this->scene = new QGraphicsScene();

  QColor c(250, 250, 250);
  QBrush brush(c, Qt::SolidPattern);
  this->scene->setBackgroundBrush(brush);

  this->minimumWidth = 1240*2;
  this->minimumHeight = 1024*2;
  this->scene->setSceneRect(-this->minimumWidth/2, -this->minimumHeight/2,
      this->minimumWidth, this->minimumHeight);
  QHBoxLayout *canvasLayout = new QHBoxLayout(this);
  canvasLayout->addWidget(view);
  canvasLayout->setAlignment(Qt::AlignHCenter);

  view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  view->setScene(this->scene);
  view->centerOn(QPointF(0, 0));
  view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  view->setDragMode(QGraphicsView::ScrollHandDrag);

  canvasLayout->setContentsMargins(0, 0, 0, 0);
  canvasLayout->setSpacing(0);
  this->setLayout(canvasLayout);
}

/////////////////////////////////////////////////
CMLEditorWidget::~CMLEditorWidget()
{
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
