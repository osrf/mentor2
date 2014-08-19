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

#ifndef _SIMSTATE_EVENTSOURCE_HH_
#define _SIMSTATE_EVENTSOURCE_HH_

#include "EventSource.hh"


namespace gazebo
{

 
  class SimStateEventSource: public EventSource
  {
    /// \brief Constructor
    /// \param[in] _world Pointer to the world.
    public: SimStateEventSource(transport::PublisherPtr pub, physics::WorldPtr _world);
   
    /// Documentation Inherited
    public: virtual void Load(const sdf::ElementPtr &_sdf);

    /// \brief Callback for the pause event
    public: void OnPause(bool p);

    /// brief True if the simulation is paused.
    private: bool hasPaused;

    /// \brief Pointer to the Gazebo pause event connection
    private: event::ConnectionPtr pauseConnection;
    
  };

}
 

#endif
