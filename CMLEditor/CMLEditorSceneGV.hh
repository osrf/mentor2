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

#ifndef _CML_EDITOR_SCENE_QGV_HH_
#define _CML_EDITOR_SCENE_QGV_HH_

#include <QGVScene.h>
//#include <qgv.h>
//#include <map>
#include <string>
//#include <vector>
#include <gazebo/gui/qt.h>

namespace gazebo
{
  namespace gui
  {
//    class QGVNode;
//    class QGVEdge;

    /// \class CMLEditorSceneGV CMLEditorSceneGV.hh
    /// \brief Control the editor view and manage contents in the editor scene.
    class CMLEditorSceneGV : public QGVScene
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent Widget.
      public: CMLEditorSceneGV(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLEditorSceneGV();

      /// \brief Add a node to the scene.
      /// \param[in] _name Name of the node.
      /// \return the Node created.
      public: QGVNode *AddNode(const std::string &_name);

      /// \brief Remove a node from the scene.
      /// \param[in] _name Name of the name.
      public: void RemoveNode(const std::string &_name);

      /// \brief Check if a node exists in the scene.
      /// \param[in] _name Name of the name.
      /// \return True if the node exists.
      public: bool HasNode(const std::string &_name);

      /// \brief Get a node from the scene.
      /// \param[in] _name Name of the name.
      /// \return Pointer to the node, NULL if it does not exist.
      public: QGVNode *GetNode(const std::string &_name);

      /// \brief Add an edge to connect two nodes.
      /// \param[in] _node1 Name of the first node.
      /// \param[in] _node2 Name of the second node.
      /// \return the Edge created.
      public: QGVEdge *AddEdge(const std::string &_node1,
          const std::string &_node2);

      /// \brief Remove an edge between two nodes.
      /// \param[in] _node1 Name of the first node.
      /// \param[in] _node2 Name of the second node.
      public: void RemoveEdge(const std::string &_node1,
          const std::string &_node2);

      //public: void itemMoved();

      //protected: void timerEvent(QTimerEvent *event);

      /// \brief Overrides the default background with grid lines.
      /// \param[in] _painter Qt painter object.
      /// \param[in] _rect Qt scene background rectangle
      private: void drawBackground(QPainter * _painter, const QRectF & _rect);

      //private: int timerId;

      /// \brief A map of node names to corresponding graphics items.
      //private: std::map<std::string, QGVNode *> nodes;
    };
  }
}

#endif
