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

#ifndef _SCORING_EVENTSOURCE_HH_
#define _SCORING_EVENTSOURCE_HH_

#include "gazebo/gazebo.hh"

#include "Event.pb.h"
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/transport/TransportTypes.hh"

#include "ScoreException.hh"

namespace gazebo
{

  class EventSource
  {
    /// \brief Constructor
    /// \param[in] the type of event
    /// \param[in] _world Pointer to the world.
    public: EventSource( transport::PublisherPtr pub, const char* type, physics::WorldPtr _world);

    /// \brief 
    public: void Emit(const char* data);

    /// \brief Load sdf param
    public: virtual void Load(const sdf::ElementPtr &_sdf);

    /// \brief Initialize the event
    public: virtual void Init();
  
    /// \brief An event source can be used to enable other events
    public: virtual bool IsActive();
 
    /// \brief Name of the event.
    public: std::string name;

    /// \brief Type of event
    public: std::string type;

    /// \brief Pointer to the world.
    protected: physics::WorldPtr world;
 
    /// \brief True if the event source is active.
    protected: bool active;

    /// \brief a way to send messages to the MOOC topic (to the REST)
    protected: transport::PublisherPtr pub;
  };
  
  typedef boost::shared_ptr<EventSource> EventSourcePtr;
 
 
  /// \brief link movement triggered event, like a fall
  class MotionEventSource: public EventSource
  {
    /// \brief Constructor
    /// \param[in] _world Pointer to the world.
    public: MotionEventSource(transport::PublisherPtr pub, physics::WorldPtr _world);
    
     /// Documentation Inherited
    public: virtual void Load(const sdf::ElementPtr &_sdf);
  
    /// Documentation Inherited
    public: virtual bool Update();
  
  };


  /// \brief Gazebo events 
  class  ScoringEvents
  {
        /// \brief Connect a boost::slot to the xxxxxxxx
        /// \param[in] _subscriber the subscriber to this event
        /// \return a connection
        public: template<typename T>
            static event::ConnectionPtr
                ConnectSpawnModel(T _subscriber)
          { return spawnModel.Connect(_subscriber); }

        /// \brief Disconnect a boost::slot to the xxxxxxx
        /// \param[in] _subscriber the subscriber to this event
        public: static void DisconnectSpawnModel(
            event::ConnectionPtr _subscriber)
          { spawnModel.Disconnect(_subscriber); }

        /// \brief A model has been completed and uploaded onto the server.
        public: static event::EventT<void (std::string, bool)> spawnModel;
  };

}
 

#endif
