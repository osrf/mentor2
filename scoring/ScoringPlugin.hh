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
 * limitations under the License.k *
*/

#ifndef _SCORING_PLUGIN_HH_
#define _SCORING_PLUGIN_HH_

#include "ScoreException.hh"
#include "EventSource.hh"
#include "InRegionEventSource.hh"
#include "ExistenceEventSource.hh"
#include "SimStateEventSource.hh"

#include <iostream>


namespace gazebo
{
 
  // std::ostream& operator << (std::ostream &out, const Region &_region);
 
  class ScoringPlugin : public WorldPlugin
  {
    public: void Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf);
  
    public: void Init();
  
    public: void Update();

    /// \b callback for ~/model/info topic
    public: void OnModelInfo(ConstModelPtr &_msg);

    /// \b callback for ~/request
    public: void OnRequest(ConstRequestPtr &_msg);
 
    /// \brief world pointer to get simulation state
    private: physics::WorldPtr world;
  
    /// \brief sdf element to read event
    private: sdf::ElementPtr sdf;
  
    /// \brief All the regions defined in the XML for scoring  
    private: std::map<std::string, RegionPtr> regions;

    private: std::vector<EventSourcePtr> events;

    /// \brief a way to send messages to the MOOC topic (to the REST)
    private: transport::PublisherPtr pub;

    /// \brief subscription to the model/info
    private: transport::SubscriberPtr spawnSub;

    /// \brief known models that have been spawned already
    private: std::set<std::string> models;

    /// \brief subscription to remove models
    private: transport::SubscriberPtr requestSub;

  };
  

}

#endif
