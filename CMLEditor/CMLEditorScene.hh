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

#ifndef _CML_EDITOR_SCENE_HH_
#define _CML_EDITOR_SCENE_HH_

#include <map>
#include <string>
#include <vector>
#include <gazebo/gui/qt.h>

namespace gazebo
{
  namespace gui
  {
    class CMLNode;

    /// \class CMLEditorScene CMLEditorScene.hh
    /// \brief Control the editor view and manage contents in the editor scene.
    class CMLEditorScene : public QGraphicsScene
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent Widget.
      public: CMLEditorScene(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLEditorScene();

      /// \brief Add a node to the scene.
      /// \param[in] _name Name of the node.
      /// \return the Node created.
      public: CMLNode *AddNode(const std::string &_name);

      /// \brief Remove a node from the scene.
      /// \param[in] _name Name of the node.
      public: void RemoveNode(const std::string &_name);

      /// \brief Check if a node exists in the scene.
      /// \return True if the node exists.
      public: bool HasNode(const std::string &_name);

      public: void itemMoved();

      protected: void timerEvent(QTimerEvent *event);

      /// \brief Overrides the default background with grid lines.
      /// \param[in] _painter Qt painter object.
      /// \param[in] _rect Qt scene background rectangle
      private: void drawBackground(QPainter * _painter, const QRectF & _rect);

      private: int timerId;

      /// \brief A map of node names to corresponding graphics items.
      private: std::map<std::string, QGraphicsItem *> nodes;
    };
  }
}

#endif
