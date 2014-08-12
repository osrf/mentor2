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
#ifndef _CML_EVENTS_HH_
#define _CML_EVENTS_HH_

#include <string>

#include <gazebo/common/common.hh>

namespace gazebo
{
  namespace gui
  {
    class CMLEvents
    {
      /// \brief Connect a boost::slot to the create connection signal
      /// \param[in] _subscriber the subscriber to this event
      /// \return a connection
      public: template<typename T>
          static event::ConnectionPtr ConnectCreateConnection(T _subscriber)
        { return createConnection.Connect(_subscriber); }

      /// \brief Disconnect a boost::slot to the create connection signal
      /// \param[in] _subscriber the subscriber to this event
      public: static void DisconnectCreateConnection(
          event::ConnectionPtr _subscriber)
        { createConnection.Disconnect(_subscriber); }

      /// \brief Connect a boost::slot to the connection created signal
      /// \param[in] _subscriber the subscriber to this event
      /// \return a connection
      public: template<typename T>
          static event::ConnectionPtr ConnectConnectionCreated(T _subscriber)
        { return connectionCreated.Connect(_subscriber); }

      /// \brief Disconnect a boost::slot to the connection created signal
      /// \param[in] _subscriber the subscriber to this event
      public: static void DisconnectConnectionCreated(
          event::ConnectionPtr _subscriber)
        { connectionCreated.Disconnect(_subscriber); }

      /// \brief A connection is to be created
      public: static event::EventT<void (std::string)> createConnection;

      /// \brief A connection has been ben created
      public: static event::EventT<void ()> connectionCreated;

    };
  }
}
#endif
