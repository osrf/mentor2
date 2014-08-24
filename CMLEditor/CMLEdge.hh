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

#ifndef CML_EDGE_HH
#define CML_EDGE_HH

 #include <QGraphicsItem>

namespace gazebo
{
  namespace gui
  {
    class CMLNode;

    class CMLEdge : public QGraphicsItem
    {
      public: enum { Type = UserType + 2 };

      /// \brief Constructor.
      /// \param[in] _sourceNode Source node of the edge.
      /// \param[in] _destNode Destination node of the edge.
      public: CMLEdge(CMLNode *_sourceNode, CMLNode *_destNode);

      /// \brief Destructor;
      public: ~CMLEdge();

      /// \brief Get the source node.
      /// \return Source node.
      public: CMLNode *GetSourceNode() const;

      /// \brief Get the destination node.
      /// \return Destination node.
      public: CMLNode *GetDestNode() const;

      /// \brief Adjust the source and destination points of the edge.
      public: void Adjust();

      /// \brief Get the type of this graphics item.
      /// \return Type of this item.
      public: int GetType() const { return Type; }

      /// \brief Get the bounding rectangle of the edge
      /// \return The edge's bounding rectangle
      protected: QRectF boundingRect() const;

      /// \brief Qt paint function for drawing the rect item.
      /// \param[in] _painter Qt painter object.
      /// \param[in] _option Qt style options for the item.
      /// \param[in] _widget Qt widget being painted on.
      protected: void paint(QPainter *_painter,
          const QStyleOptionGraphicsItem *_option, QWidget *_widget);

      /// \brief Source node.
      private: CMLNode *source;

      /// \brief Destination node.
      private: CMLNode *dest;

      /// \brief Point where the edge is connected to the source node.
      private: QPointF sourcePoint;

      /// \brief Point where the edge is connected to the destination node.
      private: QPointF destPoint;

      /// \brief Size of the arrow at the edge points.
      private: qreal arrowSize;
    };
  }
}
 #endif
