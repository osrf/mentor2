/*
 * Copyright (C) 2016 Open Source Robotics Foundation
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

#include <functional>

#include <gazebo/common/Plugin.hh>

#include "TugOfWarPlugin.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(TugOfWarPlugin)

/////////////////////////////////////////////////
TugOfWarPlugin::TugOfWarPlugin()
{
}

/////////////////////////////////////////////////
TugOfWarPlugin::~TugOfWarPlugin()
{
  if (this->towJoint)
  {
    this->towJoint->Detach();
    this->towJoint.reset();
  }
  this->updateConnection.reset();
}

/////////////////////////////////////////////////
void TugOfWarPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
  this->model = _model;
  if (_sdf->HasElement("link"))
  {
    this->towLinkName = _sdf->Get<std::string>("link");
  }

  // Listen to the update event. This event is broadcast every
  // simulation iteration.
  this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      std::bind(&TugOfWarPlugin::Update, this));
}

/////////////////////////////////////////////////
void TugOfWarPlugin::Init()
{
  if (this->towLinkName.empty())
  {
    gzerr << "Tug of war link name is empty. Plugin not loaded" << std::endl;
    return;
  }

  physics::WorldPtr world = this->model->GetWorld();
  for (auto m : world->GetModels())
  {
    if (m == this->model)
      continue;
    this->towLink = m->GetLink(this->towLinkName);
    if (this->towLink)
    {
      this->towModel = m;
      break;
    }
  }
}

/////////////////////////////////////////////////
void TugOfWarPlugin::Update()
{
  if (!this->towLink || !this->towModel || this->towJoint || !this->model)
    return;

  std::string parentLinkName =
      this->model->GetName() + "::" + this->towLinkName;

  std::string childLinkName =
      this->towModel->GetName() + "::" + this->towLinkName;

  std::cerr << " creating joint " << parentLinkName << ", " <<
      childLinkName << std::endl;

  physics::WorldPtr world = this->model->GetWorld();
  physics::PhysicsEnginePtr physics = world->GetPhysicsEngine();
  this->towJoint = physics->CreateJoint("fixed", this->model);


  sdf::ElementPtr jointSDF;
  jointSDF.reset(new sdf::Element);
  sdf::initFile("joint.sdf", jointSDF);
  jointSDF->GetElement("parent")->Set(parentLinkName);
  jointSDF->GetElement("child")->Set(childLinkName);
  this->towJoint->Load(jointSDF);
  this->towJoint->Init();
  std::cerr << " create tow joint done " << parentLinkName << ", " <<
      childLinkName << std::endl;
}

/////////////////////////////////////////////////
void TugOfWarPlugin::Reset()
{
}
