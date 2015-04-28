/*
 * Copyright 2014 Open Source Robotics Foundation
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

#ifndef _CMLRENDER_HH_
#define _CMLRENDER_HH_

#include <gazebo/gui/qt.h>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>

#include "SimpleModel.pb.h"

namespace gazebo
{
  namespace gui
  {
    class CMLComponentInspector;

    /// \class CMLRender CMLRender.hh
    /// \brief Handle User Interaction with the Gazebo Render Window.
    class CMLRender : public QObject
    {
      Q_OBJECT

      /// \brief Constructor
      public: CMLRender();

      /// \brief Destructor
      public: virtual ~CMLRender();

      /// \brief Update callback on PreRender.
      public: void Update();

      /// \brief Enable the mouse and key event handlers.
      public: void EnableEventHandlers();

      /// \brief Disable the mouse and key event handlers.
      public: void DisableEventHandlers();

      /// \brief Mouse event filter callback when mouse button is pressed.
      /// \param[in] _event The mouse event.
      /// \return True if the event was handled
      private: bool OnMousePress(const common::MouseEvent &_event);

      /// \brief Mouse event filter callback when mouse button is released.
      /// \param[in] _event The mouse event.
      /// \return True if the event was handled
      private: bool OnMouseRelease(const common::MouseEvent &_event);

      /// \brief Mouse event filter callback when mouse is moved.
      /// \param[in] _event The mouse event.
      /// \return True if the event was handled
      private: bool OnMouseMove(const common::MouseEvent &_event);

      /// \brief Mouse event filter callback when mouse is double clicked.
      /// \param[in] _event The mouse event.
      /// \return True if the event was handled
      private: bool OnMouseDoubleClick(const common::MouseEvent &_event);

      /// \brief Event callback when a connection is to be created.
      /// \param[in] _type Type of connection.
      private: void OnCreateConnection(const std::string &_type);

      /// \brief Event callback when a connection has been created.
      /// \brief Parent of the connection.
      /// \brief Child of the connection.
      private: void OnConnectionCreated(const std::string &_parent,
          const std::string &_parentPort, const std::string &_child,
          const std::string &_childPort);

      /// \brief Callback when a request message is received.
      /// \param[in] _msg The request message.
      private: void OnRequest(ConstRequestPtr &_msg);

      /// \brief Qt Callback to open component inspector
      private slots: void OnOpenInspector();

      /// \brief Qt Callback when a component is to be deleted.
      private slots: void OnDelete();

      /// \brief Callback when the model editor mode has been toggled.
      /// \param[in] _enabled True if the model editor mode is active.
      private slots: void OnModelEditorMode(bool _enabled);

      /// \brief All the event connections.
      private: std::vector<event::ConnectionPtr> connections;

      /// \brief Publish requests
      private: transport::PublisherPtr connectionPub;

      /// \brief Subscriber for request messages.
      private: transport::SubscriberPtr requestSub;

      /// \brief Publish rest posts
      private: transport::PublisherPtr restPub;

      /// \brief Communication Node
      private: transport::NodePtr node;

      /// \brief Qt action for opening the component inspector.
      private: QAction *inspectAct;

      /// \brief SimpleModel message associated to the current component being
      /// inspected.
//      private: Simple_msgs::msgs::SimpleModel inspectMsg;

      /// \brief Name of component to be inspected.
      private: std::string inspectName;
    };
  }
}

#endif
