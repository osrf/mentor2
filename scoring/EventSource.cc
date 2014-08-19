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

#include "EventSource.hh"

using namespace gazebo;
using namespace std;

event::EventT<void (std::string, bool)> ScoringEvents::spawnModel;


////////////////////////////////////////////////////////////////////////////////
EventSource::EventSource( transport::PublisherPtr _pub,
                          const char* _type,
                          physics::WorldPtr _world)
  :type(_type), pub(_pub)
{
  this->name = "";
  this->world = _world;
  this->active = true;
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
//  adapted from TimePanel
std::string FormatTime(common::Time &_t)
{
  std::ostringstream stream;
  unsigned int day, hour, min, sec, msec;

  stream.str("");

  sec = _t.sec;

  day = sec / 86400;
  sec -= day * 86400;

  hour = sec / 3600;
  sec -= hour * 3600;

  min = sec / 60;
  sec -= min * 60;

  msec = rint(_t.nsec * 1e-6);

  stream << std::setw(2) << std::setfill('0') << day << " ";
  stream << std::setw(2) << std::setfill('0') << hour << ":";
  stream << std::setw(2) << std::setfill('0') << min << ":";
  stream << std::setw(2) << std::setfill('0') << sec << ".";
  stream << std::setw(3) << std::setfill('0') << msec;

  return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
void EventSource::Emit(const char* data )
{
  if(this->IsActive())
  {
    std::cout << "scoring/EventSource::Emit()  event fired " << this->name << " " << data << "" << std::endl;
    // publish to MOOC topic
    Event_msgs::msgs::RestPost msg;
    msg.set_route("/events/new");
    string json("{");
    json += "\"type\": \"";
    json += this->type;
    json += "\", ";
    json += "\"name\": \"";
    json += this->name + "\", ";

    common::Time t;

    json += "\"clock_time\": ";
    json += "\"";
    json += common::Time::GetWallTimeAsISOString();
    json += "\", ";    

    json += "\"real_time\": ";
    json += "\"";
    t = this->world->GetRealTime();
    json += FormatTime(t);
    json += "\", ";

    json += "\"sim_time\": ";
    json += "\"";
    t = this->world->GetSimTime();
    json += FormatTime(t);
    json += "\", ";

    json += "\"pause_time\": ";
    json += "\"";
    t = this->world->GetPauseTime();
    json += FormatTime(t);
    json += "\", ";

    json += " \"data\": "; 
    json += data;
    json += "}";
    msg.set_json(json.c_str() );
    std::cout << "JSON " << json << endl;
    pub->Publish(msg);
  }

}

///////////////////////////////////////////////////////////////////////////////
bool EventSource::IsActive()
{
  return this->active;
}


////////////////////////////////////////////////////////////////////////////////
MotionEventSource::MotionEventSource(transport::PublisherPtr _pub, 
                                      physics::WorldPtr _world) 
  :EventSource(_pub, "motion", _world)
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

