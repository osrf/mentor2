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


#include "ScoringPlugin.hh"

using namespace gazebo;
using namespace std;
using namespace sdf;
using namespace physics;


void ScoringPlugin::OnModelInfo(ConstModelPtr &_msg)
{
  string modelName = _msg->name();
  // if the model is not in the set, emit event
  if(models.insert(modelName).second)
  {
    cout << "SPAWNED " << modelName << endl;
    ScoringEvents::spawnModel(modelName, true);
  }
}

void ScoringPlugin::OnRequest(ConstRequestPtr &_msg)
{
  if (_msg->request() == "entity_delete")
  {
    string modelName = _msg->data();
    if(models.erase(modelName) == 1)
    {
      cout << "DELETED " << modelName << endl;
      ScoringEvents::spawnModel(modelName, false);
    } 
  }
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
  pub = node->Advertise<SimpleMOOC_msgs::msgs::MOOCEvent>("~/MOOCEvent");

  // Subscribe to model spawning
  spawnSub = node->Subscribe("~/model/info", &ScoringPlugin::OnModelInfo, this );

  // detect model deletion
  requestSub = node->Subscribe("~/request", &ScoringPlugin::OnRequest, this);

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


  cout << "\nReading scoring events" << endl;
  child = this->sdf->GetElement("event");
  while (child)
  {
    string eventName = child->GetElement("name")->Get<string>();
    string eventType = child->GetElement("type")->Get<string>();
    cout << "Event " << eventName << " [" << eventType << "]" << endl;

    EventSourcePtr event;
    if (eventType == "sim_state")
    {
      event.reset(new SimStateEventSource(this->pub, this->world));
    }
    else if (eventType == "motion")
    {
      event.reset(new MotionEventSource(this->pub, this->world));
    }
    else if (eventType == "inclusion")
    {
      event.reset(new InRegionEventSource(this->pub, this->world, this->regions));
    }
    else if (eventType == "existence" )
    {
      event.reset(new ExistenceEventSource(this->pub, this->world) ); 
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
  std::cerr << "Scoring events loaded" << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
void ScoringPlugin::Init()
{
  cout << "ScoringPlugin::Init " << this << endl;
  cout << "Initialization of scoring events!" << endl;
  for (unsigned int i = 0; i < events.size(); ++i){
    events[i]->Init();
  }
  // seed the map with the initial models
  for (int i=0; i < world->GetModelCount(); ++i)
  {
    string name = world->GetModel(i)->GetName();
    models.insert(name);
  }
}



// Register this plugin with the simulator
GZ_REGISTER_WORLD_PLUGIN(ScoringPlugin)
