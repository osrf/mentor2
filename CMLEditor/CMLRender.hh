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

#include <gazebo/common/MouseEvent.hh>

namespace gazebo
{
  namespace gui
  {
    /// \class CMLRender CMLRender.hh
    /// \brief Handle User Interaction with the Gazebo Render Window.
    class CMLRender
    {
      /// \brief Constructor
      public: CMLRender();

      /// \brief Destructor
      public: virtual ~CMLRender();

      /// \brief Update callback on PreRender.
      public: void Update();

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
      private: void OnConnectionCreated();

      /// \brief All the event connections.
      private: std::vector<event::ConnectionPtr> connections;
    };
  }
}

#endif
