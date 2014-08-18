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

#ifndef _INREGIONEVENTSOURCE_HH_
#define _INREGIONEVENTSOURCE_HH_

#include "EventSource.hh"

namespace gazebo
{
  class Volume
  {
    public: virtual bool PointInVolume(const math::Vector3 &p) const;
    public: math::Vector3 min, max;
  
  };

  typedef boost::shared_ptr<Volume> VolumePtr;

  class Region 
  {
    public: void Load(const sdf::ElementPtr &_sdf);
    public: bool PointInRegion(const math::Vector3 &p) const;
    public: std::string name;
    public: std::vector<VolumePtr> volumes;
  };

  typedef boost::shared_ptr<Region> RegionPtr;


  std::ostream& operator << (std::ostream &out, const Region &_region);

  class  InRegionEventSource: public EventSource
  {
    /// \brief Constructor
    /// \param[in] _world Pointer to the world.
    public: InRegionEventSource(transport::PublisherPtr pub, 
                                physics::WorldPtr _world, 
                                const std::map<std::string, RegionPtr> &_regions);

    /// \brief Initialize the event
    public: virtual void Init();
   
    /// \brief Called every simulation step
    public: void Update();

    /// Documentation Inherited
    public: virtual void Load(const sdf::ElementPtr &_sdf);

    /// \brief Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;

    /// \brief The model used for the in region check.
    private: std::string modelName;

    /// \brief a Pointer to the model
    private: physics::ModelPtr model;

    /// \brief The region used for the in region check.
    private: std::string regionName;

    /// \brief The region pointer
    private: RegionPtr region;

    /// \brief A map of region names to region pointers.
    private: const std::map<std::string, RegionPtr> &regions;
 
    /// \brief the current state
    private: bool isInside;
    
  };

}


#endif
