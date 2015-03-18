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

#include <set>
#include <boost/thread/mutex.hpp>

#include <gazebo/msgs/msgs.hh>
#include <gazebo/gui/qt.h>

#include "SimpleConnection.pb.h"
#include "SimpleModel.pb.h"

typedef const boost::shared_ptr<const Simple_msgs::msgs::SimpleConnection>
    ConstSimpleConnectionPtr;

typedef const boost::shared_ptr<const Simple_msgs::msgs::SimpleModel>
    ConstSimpleModelPtr;

//class QGVScene;
//class QGVNode;

namespace gazebo
{
  namespace gui
  {
    class CMLEditorView;
    class CMLEditorScene;
    class CMLEditorSceneGV;

    /// \class CMLEditorWidget CMLEditorWidget.hh
    /// \brief The parent widget of the CML editor
    class CMLEditorWidget : public QWidget
    {
      /// \brief Constructor
      /// \param[in] _parent Parent QWidget.
      public: CMLEditorWidget(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLEditorWidget();

      /// \brief Initialize the widget to listen to events/topics
      public: void Init();

      /// \brief Add a node to the scene in the widget.
      /// \param[in] _node Name of node.
      public: void AddNode(const std::string &_node);

      /// \brief Remove a node from the scene in the widget
      /// \param[in] _node Name of node.
      public: void RemoveNode(const std::string &_node);

      /// \brief Add an edge to the scene in the widget
      /// \param[in] _node Name of parent node.
      /// \param[in] _node Name of child node.
      public: void AddEdge(const std::string &_parent,
          const std::string &_child);

      /// \brief Remove an edge from the scene in the widget
      /// \param[in] _node Name of parent node.
      /// \param[in] _node Name of child node.
      public: void RemoveEdge(const std::string &_parent,
          const std::string &_child);

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
      /// \return True if the message is processed.
      private: bool ProcessSceneMsg(ConstScenePtr &_msg);

      /// \brief Process a model message.
      /// \param[in] _msg The message data.
      /// \return True if the message is processed.
      private: bool ProcessModelMsg(const msgs::Model &_msg);

      /// \brief Process a simple connection message.
      /// \param[in] _msg The message data.
      /// \return True if the message is processed.
      private: bool ProcessSimpleConnectionMsg(
          const Simple_msgs::msgs::SimpleConnection &_msg);

      /// \brief Process a simple model message.
      /// \param[in] _msg The message data.
      /// \return True if the message is processed.
      private: bool ProcessSimpleModelMsg(
          const Simple_msgs::msgs::SimpleModel &_msg);

      /// \brief Process remove entity requests.
      /// \param[in] _name Name of the entity removed.
      /// \return True if the remove entity process is successful.
      private: bool ProcessRemoveEntity(const std::string &_name);

      /// \brief Process all received messages.
      private: void PreRender();

      /// \brief Response callback
      /// \param[in] _msg The message data.
      private: void OnResponse(ConstResponsePtr &_msg);

      /// \brief Request callback
      /// \param[in] _msg Teh message data.
      private: void OnRequest(ConstRequestPtr &_msg);

      /// \brief Simple connection message callback
      /// \param[in] _msg The message data.
      private: void OnSimpleConnection(ConstSimpleConnectionPtr &_msg);

      /// \brief Simple model message callback
      /// \param[in] _msg The message data.
      private: void OnSimpleModel(ConstSimpleModelPtr &_msg);

      /// \brief Get the schematic representation of a component
      /// \param[in] _schematicType The schematic type for the component.
      /// \return Image url
      private: std::string GetSchematicIcon(const std::string &_schematicType);

      /// \brief Qt Graphics Scene where graphics items are drawn in
//      private: QGVScene *scene;
//      private: QGraphicsScene *scene;
//      private: CMLEditorScene *scene;
      private: CMLEditorSceneGV *scene;

      /// \brief Qt Graphics View attached to the scene
      private: CMLEditorView *view;

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

      /// \def SimpleConnectionMsgs_L
      /// \brief List of simple connection messages.
      typedef std::list<boost::shared_ptr<
          Simple_msgs::msgs::SimpleConnection const> > SimpleConnectionMsgs_L;

      /// \brief List of simple connection message to process.
      private: SimpleConnectionMsgs_L simpleConnectionMsgs;

      /// \def SimpleModelMsgs_L
      /// \brief List of simple model messages.
      typedef std::list<boost::shared_ptr<
          Simple_msgs::msgs::SimpleModel const> > SimpleModelMsgs_L;

      /// \brief List of simple model message to process.
      private: SimpleModelMsgs_L simpleModelMsgs;

      /// \brief Communication Node
      private: transport::NodePtr node;

      /// \brief Subscribe to model connection updates
      private: transport::SubscriberPtr simpleConnectionSub;

      /// \brief Subscribe to model info updates
      private: transport::SubscriberPtr simpleModelSub;

      /// \brief Subscribe to model info updates
      private: transport::SubscriberPtr modelInfoSub;

      /// \brief Subscribe to scene topic
      private: transport::SubscriberPtr sceneSub;

      /// \brief Subscribe to reponses.
      private: transport::SubscriberPtr responseSub;

      /// \brief Subscribe to requests.
      private: transport::SubscriberPtr requestSub;

      /// \brief Publish requests
      private: transport::PublisherPtr requestPub;

      /// \brief Keep around our request message.
      private: msgs::Request *requestMsg;

      // \brief A list of entities to be removed.
      private: std::set<std::string> removeEntityList;

      // \brief A list of entities to be removed.
      private: bool editorMode;
    };
  }
}

#endif
