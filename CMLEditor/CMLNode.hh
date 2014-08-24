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

 #ifndef CML_NODE_HH
 #define CML_NODE_HH

#include <gazebo/gui/gui.hh>

namespace gazebo
{
  namespace gui
  {
    class CMLEdge;
    class CMLEditorScene;

    /// \class CMLNode CMLNode.hh
    /// \brief A node in the CML editor
    class CMLNode : public QObject, public QGraphicsPixmapItem
    {
      Q_OBJECT

      public: enum { Type = UserType + 1 };

      /// \brief Constructor.
      /// \param[in] _name Name of the node.
      /// \param[in] _scene Scene where the node will be added to.
      public: CMLNode(const std::string &_name, CMLEditorScene *_scene);

      /// \brief Add an edge to the node.
      /// \param[in] _edge Edge to be added to the node.
      public: void AddEdge(CMLEdge *_edge);

      /// \brief Get all the edges connected to this node.
      /// \return A list of connected edges.
      public: QList<CMLEdge *> GetEdges() const;

      /// \brief Check if the node has the specified edge.
      /// \param[in] _edge Edge
      /// \return True if the node has the edge.
      public: bool HasEdge(CMLEdge *_edge);

      /// \brief Remove an edge from the node.
      /// \param[in] _edge Edge to be removed.
      public: void RemoveEdge(CMLEdge *_edge);

      /// \brief Get the name of this graphics item.
      /// \return Name of this item.
      public: std::string GetName() const;

      /// \brief Get the type of this graphics item.
      /// \return Type of this item.
      public: int GetType() const { return Type; }

      void calculateForces();
      bool advance();

/*      /// \brief Get the bounding rectangle of the node
      /// \return The node's bounding rectangle
      public: QRectF boundingRect() const;
*/
      /// \brief Get the bounding rect of this pixmap.
      /// \return the pixmap's bounding rect.
      public: QRectF GetPixmapBoundingRect();

      /// \brief Get the bounding rect of this node.
      /// \return the bounding rect.
      public: QRectF boundingRect() const;

      /// \brief Get the shape of this node
      /// \return the shape in described using a QPainterPath object.
      public: QPainterPath shape() const;

      /// \brief Return Type enum for this custrom graphics item.
      /// \return Type enum.
      public: int type() const;

      /// \brief Set the icon for this node.
      /// \param[in] _url Icon url.
      public: void SetIcon(const std::string &_url);

      /// \brief Qt paint function for drawing the node item.
      /// \param[in] _painter Qt painter object.
      /// \param[in] _option Qt style options for the item.
      /// \param[in] _widget Qt widget being painted on.
      protected: void paint(QPainter *_painter,
          const QStyleOptionGraphicsItem *_option, QWidget *_widget);

      protected: QVariant
          itemChange(GraphicsItemChange change, const QVariant &value);

      /// \brief Qt mouse press event.
      /// \param[in] _event Qt mouse event.
      protected: void mousePressEvent(QGraphicsSceneMouseEvent *event);

      /// \brief Qt mouse release event.
      /// \param[in] _event Qt mouse event.
      protected: void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

      /// \brief Qt context menu event received on a mouse right click.
      /// \param[in] Qt context menu event.
      private: virtual void contextMenuEvent(
          QGraphicsSceneContextMenuEvent *_event);

      /// \brief Qt Callback to open component inspector
      private slots: void OnOpenInspector();

      /// \brief Update the name label bounding box.
      private: void UpdateTextBoundingRect();

      /// \brief List of edges connected to this node.
      private: QList<CMLEdge *> edgeList;

      private: QPointF newPos;

      private: CMLEditorScene *graph;

      /// \brief Name of the node.
      private: std::string name;

      /// \brief Bounding rect for the name label.
      private: QRectF textBoundingRect;

      /// \brief Font for the name label.
      private: QFont textFont;

      /// \brief Qt action for opening the component inspector.
      private: QAction *inspectAct;
    };
  }
}

 #endif
