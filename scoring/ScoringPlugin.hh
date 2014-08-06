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
#define _SCORING_PLUGIN_HH_j


#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"

#include <iostream>

#include "SimpleMOOC.pb.h"


namespace gazebo
{

  class ScoreException : public std::runtime_error
  {
    public: ScoreException(const char*m) :std::runtime_error(m){}
  };
  
  
  class Volume
  {
    public: virtual bool PointInVolume(const math::Vector3 &p) const;
    public: math::Vector3 min, max;
  
  };
  
  typedef boost::shared_ptr<Volume> VolumePtr;
  
  class Region
  {
    public: void Load(const sdf::ElementPtr &_sdf);
    public: std::string name;
    public: std::vector<VolumePtr> volumes;
    public: bool PointInRegion(const math::Vector3 &p) const; 
  };
  
  typedef boost::shared_ptr<Region> RegionPtr;
  
  // std::ostream& operator << (std::ostream &out, const Region &_region);
  
  class EventSource
  {
    /// \brief Constructor
    /// \param[in] _world Pointer to the world.
    public: EventSource(physics::WorldPtr _world);

    /// \brief Load sdf param
    public: virtual void Load(const sdf::ElementPtr &_sdf);

    /// \brief Initialize the event
    public: virtual void Init();
  
    /// \brief called periodically to check wether the event should be triggered
    /// \return true if the event should be triggered, false otherwise
    public: virtual bool Update() = 0;
  
    /// \brief An event source can be used to enable other events
    public: virtual bool IsActive();
 
    /// \brief Get current event specific data
    public: virtual std::string GetEventData();
 
    /// \brief Name of the event.
    public: std::string name;

    /// \brief Pointer to the world.
    protected: physics::WorldPtr world;
 
    /// \brief True if the event source is active.
    protected: bool active;
  };
  
  typedef boost::shared_ptr<EventSource> EventSourcePtr;
  
  /// \brief link movement triggered event, like a fall
  class MotionEventSource: public EventSource
  {
    /// \brief Constructor
    /// \param[in] _world Pointer to the world.
    public: MotionEventSource(physics::WorldPtr _world);
    
     /// Documentation Inherited
    public: virtual void Load(const sdf::ElementPtr &_sdf);
  
    /// Documentation Inherited
    public: virtual bool Update();
  
  };
  
  class SimStateEventSource: public EventSource
  {
    /// \brief Constructor
    /// \param[in] _world Pointer to the world.
    public: SimStateEventSource(physics::WorldPtr _world);
   
    /// Documentation Inherited
    public: virtual bool Update();

    /// Documentation Inherited
    public: virtual void Load(const sdf::ElementPtr &_sdf);

    /// brief True if the simulation is paused.
    private: bool isPaused;
  };

  
  
  class  InRegionEventSource: public EventSource
  {
    /// \brief Constructor
    /// \param[in] _world Pointer to the world.
    public: InRegionEventSource(physics::WorldPtr _world, 
        const std::map<std::string, RegionPtr> &_regions);

    /// \brief Initialize the event
    public: virtual void Init();
   
    /// Documentation Inherited
    public: virtual bool Update();

    /// Documentation Inherited
    public: virtual void Load(const sdf::ElementPtr &_sdf);

    /// Documentation Inherited 
    public: virtual std::string GetEventData();

    /// \brief The model used for the in region check.
    private: std::string modelName;

    /// \brief a Pointer to the model
    private: physics::ModelPtr model;

    /// \brief The region used for the in region check.
    private: std::string regionName;

    /// \brief A map of region names to region pointers.
    private: const std::map<std::string, RegionPtr> &regions;
 
    /// \brief A region to test the point inclusion.
    private: RegionPtr region;

    /// \brief the current state
    private: bool isInside;
    
  };
  
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
  
    /// \brief Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  
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
