/*
 * Copyright (C) 2015 Open Source Robotics Foundation
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

#include <gazebo/physics/physics.hh>

#include <gazebo/msgs/msgs.hh>
#include <gazebo/physics/GearboxJoint.hh>

#include "GearboxPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(GearboxPlugin)

/////////////////////////////////////////////////
GearboxPlugin::GearboxPlugin()
{
  this->gearRatio = 1.0;
  this->parentLinkName = "";
  this->childLinkName = "";
}

/////////////////////////////////////////////////
GearboxPlugin::~GearboxPlugin()
{
  if (this->gearboxJoint)
  {
    this->gearboxJoint->Detach();
    this->gearboxJoint.reset();
  }
}

/////////////////////////////////////////////////
void GearboxPlugin::LoadImpl(sdf::ElementPtr _sdf)
{
}

/////////////////////////////////////////////////
void GearboxPlugin::Init()
{
  SimpleModelPlugin::Init();
}

/////////////////////////////////////////////////
void GearboxPlugin::UpdateImpl(double _timeSinceLastUpdate)
{

  // GearboxPlugin: Dynamically creates a gearbox joint between two links
  // that are connected to the parent model
  // e.g. Gearbox joint will be created between link1 and link2:
  // link1 -- joint1 -- PARENT_MODEL -- joint2 -- link2

  this->gearRatio = this->GetProperty<double>("gear_ratio");
  this->parentLinkName = this->GetProperty<std::string>("parent");
  this->childLinkName = this->GetProperty<std::string>("child");

  if (!this->gearboxJoint)
  {

    physics::WorldPtr world = this->parent->GetWorld();
    physics::LinkPtr parentLink = boost::dynamic_pointer_cast<physics::Link>(
      world->GetByName(this->parentLinkName));
    physics::LinkPtr childLink = boost::dynamic_pointer_cast<physics::Link>(
      world->GetByName(this->childLinkName));

    if (!parentLink || !childLink)
      return;


    parentLink->SetAutoDisable(false);
    childLink->SetAutoDisable(false);
    physics::PhysicsEnginePtr physics = world->GetPhysicsEngine();
    this->gearboxJoint = physics->CreateJoint("gearbox", this->parent);
    this->gearboxJoint->SetModel(parentLink->GetModel());

    sdf::ElementPtr jointSDF;
    jointSDF.reset(new sdf::Element);
    sdf::initFile("joint.sdf", jointSDF);
    jointSDF->GetElement("parent")->Set(this->parentLinkName);
    jointSDF->GetElement("child")->Set(this->childLinkName);
    jointSDF->GetElement("gearbox_ratio")->Set(this->gearRatio);
    std::cerr << "gear ratio " << this->gearRatio << std::endl;
    jointSDF->GetElement("gearbox_reference_body")->Set(
        this->parentLinkName);
    jointSDF->GetElement("axis")->GetElement("xyz")->Set(
        math::Vector3::UnitZ);

    this->gearboxJoint->Load(jointSDF);

/*    physics::GearboxJoint *gbJoint =
        dynamic_cast<physics::GearboxJoint *>(this->gearboxJoint.get());

    if (gbJoint)
      gbJoint->SetGearboxRatio(this->gearRatio);*/

    this->gearboxJoint->Init();

/*        physics::GearboxJoint *gbJoint =
        dynamic_cast<physics::GearboxJoint *>(this->gearboxJoint.get());

    if (gbJoint)
      gbJoint->SetGearboxRatio(this->gearRatio);
    gearboxJoint->Init();*/

  }

  return;
}
