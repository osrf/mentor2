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



#include "InRegionEventSource.hh"

using namespace std;
using namespace gazebo;


////////////////////////////////////////////////////////////////////////////////
InRegionEventSource::InRegionEventSource( transport::PublisherPtr _pub,
                                          physics::WorldPtr _world, 
                              const std::map<std::string, RegionPtr> &_regions) 
  :EventSource(_pub, "region", _world), isInside(false), regions(_regions)
{
}

////////////////////////////////////////////////////////////////////////////////
void InRegionEventSource::Load(const sdf::ElementPtr &_sdf)
{
  EventSource::Load(_sdf);
  if (_sdf->HasElement("model"))
    this->modelName = _sdf->GetElement("model")->Get<std::string>();  
  else
    gzerr << this->name << " is missing a model element" << std::endl;

  if (_sdf->HasElement("region"))
    this->regionName = _sdf->GetElement("region")->Get<std::string>();
  else
    gzerr << this->name << " is missing a reigon element" << std::endl;

  // Listen to the update event. This event is broadcast every
  // simulation iteration.
  this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      boost::bind(&InRegionEventSource::Update, this));
}

////////////////////////////////////////////////////////////////////////////////
void InRegionEventSource::Init()
{
  this->model = this->world->GetModel(this->modelName);
  if (!model)
  {
    gzerr << this->name << ": Model '" << this->modelName 
        << "' does not exist" << std::endl;
  }

  std::map<std::string, RegionPtr>::const_iterator it = 
    this->regions.find(this->regionName); 
  if (it != this->regions.end())
  {
    this->region = it->second;
  }
  else 
  {
    gzerr << this->name << ": Region '" << this->regionName 
        << "' does not exist" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
void InRegionEventSource::Update()
{
  if (!this->model)
    return;

  math::Vector3 point = this->model->GetWorldPose().pos;
  //  cout << "POINT " << point << endl;
  bool oldState = this->isInside;
  this->isInside = this->region->PointInRegion(point);
  if( oldState != this->isInside)
  {
    string json = "{";
    if(this->isInside)
    {
      json += "\"state\":\"inside\",";
    }
    else
    {
      json += "\"state\":\"outside\",";
    }
    json += "\"region\":\"" + this->regionName + "\", ";
    json += "\"model\":\"" + this->modelName + "\"";
    json += "}";
    this->Emit(json.c_str());
  }
}

////////////////////////////////////////////////////////////////////////////////
bool Volume::PointInVolume(const math::Vector3 &_p) const
{

  if(_p.x >= min.x && _p.x <= max.x)
  {
    if(_p.y >= min.y && _p.y <= max.y)
    {
      if(_p.z >= min.z && _p.z <= max.z)
      {
        return true;
      }
    }
  }
  return false;
}

/*
ScoreRegion::ScoreRegion()
{
}

////////////////////////////////////////////////////////////////////////////////
void ScoreRegion::Load(const sdf::ElementPtr &_sdf)
{
 
  sdf::ElementPtr child = _sdf->GetFirstElement();
  while(child)
  {
    std::string name = child->GetName();
    if (name == "volume")
    {
      VolumePtr volume = VolumePtr(new Volume());
      volume->min = child->GetElement("min")->Get<math::Vector3>();
      volume->max = child->GetElement("max")->Get<math::Vector3>();
      this->volumes.push_back(volume);
    }   
    else if (name == "name")
    {
      this->name = child->Get<string>(); 
    }
    else
    {
      string m;
      m += "Unexpected element \"" + name + "\" in Region element";
      throw ScoreException(m.c_str()); 
    }
    child = child->GetNextElement();
  }
 
}

////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream &out, const ScoreRegion &_region)
{
  cout << _region.name << " [";
  for(vector<VolumePtr>::const_iterator i= _region.volumes.begin(); i != _region.volumes.end(); i++)
  {
    VolumePtr vol = *i;
    cout << " (";
    cout << vol->min << ", ";
    cout << vol->max << ")";
    
  }
  cout << "]";
  return out;
} 


*/


////////////////////////////////////////////////////////////////////////////////
bool Region::PointInRegion(const math::Vector3 &_p) const
{
  for (unsigned int i=0; i< volumes.size(); ++i)
  {
    if (volumes[i]->PointInVolume(_p))
    {
      return true;
    }
  }
  return false;
}


void Region::Load(const sdf::ElementPtr &_sdf)
{
  sdf::ElementPtr child = _sdf->GetFirstElement();
  while(child)
  {
    std::string name = child->GetName();
    if (name == "volume")
    {
      VolumePtr volume = VolumePtr(new Volume());
      volume->min = child->GetElement("min")->Get<math::Vector3>();
      volume->max = child->GetElement("max")->Get<math::Vector3>();
      this->volumes.push_back(volume);
    }   
    else if (name == "name")
    {
      this->name = child->Get<string>(); 
    }
    else
    {
      string m;
      m += "Unexpected element \"" + name + "\" in Region element";
      throw ScoreException(m.c_str()); 
    }
    child = child->GetNextElement();
  }
 
}

 

