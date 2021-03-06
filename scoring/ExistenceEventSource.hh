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

#ifndef _EXISTENCEVENTSOURCE_HH_
#define _EXISTENCEVENTSOURCE_HH_

#include "EventSource.hh"

namespace gazebo
{

  class  ExistenceEventSource: public EventSource
  {
    /// \brief Constructor
    public: ExistenceEventSource(transport::PublisherPtr pub, physics::WorldPtr _world);

    /// \brief Reads the model filter and event name
    public: virtual void Load(const sdf::ElementPtr &_sdf);

    /// \brief callback for gazebo event 
    public: void OnExistence(std::string _model, bool _alive);

    /// \brief A filter to raise the event only for models
    /// with a name that starts with a specific prefix
    /// when empy, the existence event is raised for each
    /// model creation and destruction
    private: std::string model;

    /// \brief Allows us to get called when a new model is spawned
    /// or deleted
    private: event::ConnectionPtr existenceConnection;

  };

}

#endif
