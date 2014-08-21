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

#ifndef _CML_EDITOR_WIDGET_HH_
#define _CML_EDITOR_WIDGET_HH_

#include <boost/thread/mutex.hpp>

#include <gazebo/msgs/msgs.hh>
#include <gazebo/gui/qt.h>

#include "SimpleConnection.pb.h"


typedef const boost::shared_ptr<const Simple_msgs::msgs::SimpleConnection>
    ConstSimpleConnectionPtr;

//class QGVScene;
//class QGVNode;

namespace gazebo
{
  namespace gui
  {
    class CMLEditorScene;

    /// \class CMLEditorWidget CMLEditorWidget.hh
    /// \brief The parent widget of the CML editor
    class CMLEditorWidget : public QWidget
    {
      /// \brief Constructor
      /// \param[in] _parent Parent QWidget.
      public: CMLEditorWidget(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLEditorWidget();

      /// \brief Qt event received when the widget is being resized
      /// \param[in] _event Resize event.
      private: void resizeEvent(QResizeEvent *_event);

      /// \brief Scene message callback.
      /// \param[in] _msg The message data.
      private: void OnSceneMsg(ConstScenePtr &_msg);

      /// \brief Model message callback.
      /// \param[in] _msg The message data.
      private: void OnModelMsg(ConstModelPtr &_msg);

      /// \brief Process a scene message.
      /// \param[in] _msg The message data.
      private: bool ProcessSceneMsg(ConstScenePtr &_msg);

      /// \brief Process a model message.
      /// \param[in] _msg The message data.
      private: bool ProcessModelMsg(const msgs::Model &_msg);

      /// \brief Process a sipmle connection message.
      /// \param[in] _msg The message data.
      private: bool ProcessSimpleConnectionMsg(
          const Simple_msgs::msgs::SimpleConnection &_msg);

      /// \brief Process all received messages.
      private: void PreRender();

      /// \brief Response callback
      /// \param[in] _msg The message data.
      private: void OnResponse(ConstResponsePtr &_msg);

      /// \brief Model connection message callback
      /// \param[in] _msg The message data.
      private: void OnSimpleConnection(ConstSimpleConnectionPtr &_msg);

      /// \brief Get the schematic representation of a component
      /// \param[in] _schematicType The schematic type for the component.
      /// \return Image url
      private: std::string GetSchematicIcon(const std::string &_schematicType);

      /// \brief Qt Graphics Scene where graphics items are drawn in
//      private: QGVScene *scene;
//      private: QGraphicsScene *scene;
      private: CMLEditorScene *scene;

      /// \brief Minimum width of the Qt graphics scene
      private: int minimumWidth;

      /// \brief Minimum height of the Qt graphics scene
      private: int minimumHeight;

      /// \brief Event connections
      private: std::vector<event::ConnectionPtr> connections;

      /// \brief Mutex to lock the various message buffers.
      private: boost::mutex *receiveMutex;

      /// \def ModelMsgs_L
      /// \brief List of model messages.
      typedef std::list<boost::shared_ptr<msgs::Model const> > ModelMsgs_L;

      /// \brief List of model message to process.
      private: ModelMsgs_L modelMsgs;

      /// \def SceneMsgs_L
      /// \brief List of scene messages.
      typedef std::list<boost::shared_ptr<msgs::Scene const> > SceneMsgs_L;

      /// \brief List of scene message to process.
      private: SceneMsgs_L sceneMsgs;

      /// \def ModelConnectionMsgs_L
      /// \brief List of simple connection messages.
      typedef std::list<boost::shared_ptr<
          Simple_msgs::msgs::SimpleConnection const> >
          SimpleConnectionMsgs_L;

      /// \brief List of simple connection message to process.
      private: SimpleConnectionMsgs_L simpleConnectionMsgs;

      /// \brief Communication Node
      private: transport::NodePtr node;

      /// \brief Subscribe to model info updates
      private: transport::SubscriberPtr simpleConnectionSub;

      /// \brief Subscribe to model info updates
      private: transport::SubscriberPtr modelInfoSub;

      /// \brief Subscribe to scene topic
      private: transport::SubscriberPtr sceneSub;

      /// \brief Subscribe to reponses.
      private: transport::SubscriberPtr responseSub;

      /// \brief Publish requests
      private: transport::PublisherPtr requestPub;

      /// \brief Keep around our request message.
      private: msgs::Request *requestMsg;

      //private: std::vector<QGVNode *> graphNodes;
    };
  }
}

#endif
