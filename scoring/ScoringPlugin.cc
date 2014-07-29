#include "ScoringPlugin.hh"

using namespace gazebo;
using namespace std;
using namespace sdf;
using namespace physics;


////////////////////////////////////////////////////////////////////////////////
EventSource::EventSource(physics::WorldPtr _world)
{
  this->name = "";
  this->world = _world;
  this->active = false;
}

////////////////////////////////////////////////////////////////////////////////
void EventSource::Init()
{
}

////////////////////////////////////////////////////////////////////////////////
void EventSource::Load(const sdf::ElementPtr &_sdf)
{
  this->name = _sdf->GetElement("name")->Get<string>();
}


////////////////////////////////////////////////////////////////////////////////
bool EventSource::IsActive()
{
  return this->active;
}


////////////////////////////////////////////////////////////////////////////////
MotionEventSource::MotionEventSource(physics::WorldPtr _world) 
  : EventSource(_world)
{
}

////////////////////////////////////////////////////////////////////////////////
void MotionEventSource::Load(const sdf::ElementPtr &_sdf)
{
  EventSource::Load(_sdf);
}

////////////////////////////////////////////////////////////////////////////////
bool MotionEventSource::Update()
{
  return false;
}

////////////////////////////////////////////////////////////////////////////////
SimStateEventSource::SimStateEventSource(physics::WorldPtr _world) 
  : EventSource(_world)
{
  this->isPaused = false;
}

////////////////////////////////////////////////////////////////////////////////
void SimStateEventSource::Load(const sdf::ElementPtr &_sdf)
{
  EventSource::Load(_sdf); 
}

////////////////////////////////////////////////////////////////////////////////
bool SimStateEventSource::Update()
{
  bool oldState = this->isPaused;
  this->isPaused = this->world->IsPaused();
  this->active = !this->isPaused;

  return oldState != this->isPaused;  
}

////////////////////////////////////////////////////////////////////////////////
InRegionEventSource::InRegionEventSource(physics::WorldPtr _world, 
    const std::map<std::string, RegionPtr> &_regions) 
  : EventSource(_world), isInside(false), regions(_regions)
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

std::cerr << " getting region name done " << std::endl;
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
    this->region = it->second;
  else 
  {
    gzerr << this->name << ": Region '" << this->regionName 
        << "' does not exist" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
bool InRegionEventSource::Update()
{
  if (!this->model)
    return false;

  math::Vector3 point = this->model->GetWorldPose().pos;
  bool oldState = this->isInside;
  this->isInside = this->region->PointInRegion(point);
  return oldState != this->isInside;

}


////////////////////////////////////////////////////////////////////////////////
bool Volume::PointInVolume(const math::Vector3 &_p) const
{
  if(_p.x >= min.x && _p.x <= max.x)
    if(_p.y >= min.y && _p.y <= max.y)
      if(_p.z >= min.z && _p.z <= max.z)
        return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Region::PointInRegion(const math::Vector3 &_p) const
{
  for (unsigned int i=0; i<= volumes.size(); ++i)
  {
    if (volumes[i]->PointInVolume(_p))
      return true;
  }
  return false;
}
 
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream &out, const Region &_region)
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


////////////////////////////////////////////////////////////////////////////////
void ScoringPlugin::Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf)
{
  this->world = _parent;
  this->sdf = _sdf;

  // Create a new transport node
  transport::NodePtr node(new transport::Node());

  // Initialize the node with the world name
  node->Init(_parent->GetName());

  // Create a publisher on the ~/MOOCEvent topic
  transport::PublisherPtr pub =
  node->Advertise<SimpleMOOC_msgs::msgs::MOOCEvent>("~/MOOCEvent");

  cout << this->sdf->GetName() << endl;

  sdf::ElementPtr child = this->sdf->GetElement("region");
  while(child)
  {
    RegionPtr region;
    region.reset(new Region);
    region->Load(child);
    this->regions[region->name] = region;
    cout << "region " << *(region.get()) << endl; 
      
    child = child->GetNextElement("region");
  }

  child = this->sdf->GetElement("event");
  while (child)
  {
    string eventType = child->GetElement("type")->Get<string>();
    EventSourcePtr event;
    if (eventType == "simState")
    {
      event.reset(new SimStateEventSource(this->world));
    }
    else if (eventType == "motion")
    {
      event.reset(new MotionEventSource(this->world));
    }
    else if (eventType == "inclusion")
    {
      event.reset(new InRegionEventSource(this->world, this->regions));
    }
    else if (eventType == "contact")
    {
      // event.reset(new MotionEventSource(this->world));
    }
    else
    {
      string m;
      m = "Unknown event type: \"" + eventType + "\" in scoring plugin"; 
      throw ScoreException(m.c_str());
    }
    if (event)
    {
      event->Load(child);
      events.push_back(event);
    }
    child = child->GetNextElement("event");
  }

std::cerr << " done while loop " << std::endl;
  // Listen to the update event. This event is broadcast every
  // simulation iteration.
  this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      boost::bind(&ScoringPlugin::Update, this));
}


////////////////////////////////////////////////////////////////////////////////
void ScoringPlugin::Init()
{
  cout << "Init!!!" << endl;
  cout << " ScoringPlugin::Load" << endl;


  for (unsigned int i = 0; i < events.size(); ++i)
    events[i]->Init();

 // Create the message
  // msgs::Factory msg;
  // Pose to initialize the model to
  // msgs::Set(msg.mutable_pose(),
  // math::Pose(math::Vector3(1, -2, 0), math::Quaternion(0, 0, 0)));

  // Send the message
  // pub->Publish(msg);
 

}

////////////////////////////////////////////////////////////////////////////////
void ScoringPlugin::Update()
{
  for (unsigned int i = 0; i < events.size(); ++i)
  {
    if (events[i]->Update())
    {
//      std::string eventData = events[i]->GetEventString();
      std::cout << "event fired " << events[i]->name << std::endl;
      // publish to rest server;
    }
  }
}


// Register this plugin with the simulator
GZ_REGISTER_WORLD_PLUGIN(ScoringPlugin)
