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

#include <sdf/sdf.hh>

#include <gazebo/msgs/msgs.hh>

#include <gazebo/common/ModelDatabase.hh>
#include <gazebo/gui/qt.h>
#include <gazebo/gui/TimePanel.hh>
#include <gazebo/gui/MainWindow.hh>
#include <gazebo/gui/RenderWidget.hh>
#include <gazebo/gui/Actions.hh>
#include <gazebo/gui/model/ModelEditorEvents.hh>
#include <gazebo/gui/model/ModelEditor.hh>

#include "SimpleModel.pb.h"
#include "SimpleConnection.pb.h"

#include "CMLManager.hh"
#include "CMLEvents.hh"
#include "CMLPropertyManager.hh"
#include "CMLEditor.hh"

using namespace gazebo;
using namespace gui;

/////////////////////////////////////////////////
CMLEditor::CMLEditor(MainWindow *_mainWindow)
  : mainWindow(_mainWindow)
{
  if (!_mainWindow)
  {
    gzerr << "Main window is NULL!" << std::endl;
    return;
  }

  // handle user interaction with the 3d render window
  this->renderProxy = new gui::CMLRender();

  this->modelEditor =
      dynamic_cast<ModelEditor *>(_mainWindow->Editor("model"));

  if (!this->modelEditor)
  {
    gzerr << "Model Editor not found. Unable to start CMLEditor plugin"
        << std::endl;
    return;
  }
  // wiring button
  QPushButton *wiringButton = new QPushButton(tr("Wires"));
  wiringButton->setCheckable(false);
  wiringButton->setChecked(false);
  connect(wiringButton, SIGNAL(clicked()), this,
      SLOT(OnElectricalConnection()));

  this->modelEditor->AddItemToPalette(wiringButton, "Wiring");


  // other components
  QPushButton *motorButton = new QPushButton(tr("Motor"));
  motorButton->setCheckable(false);
  motorButton->setChecked(false);
  motorButton->setProperty("type", QVariant("motor"));
  connect(motorButton, SIGNAL(clicked()), this,
      SLOT(SpawnEntity()));

  QPushButton *batteryButton = new QPushButton(tr("Battery"));
  batteryButton->setCheckable(false);
  batteryButton->setChecked(false);
  batteryButton->setProperty("type", QVariant("AA_battery"));
  connect(batteryButton, SIGNAL(clicked()), this,
      SLOT(SpawnEntity()));

  QPushButton *switchButton = new QPushButton(tr("Switch"));
  switchButton->setCheckable(false);
  switchButton->setChecked(false);
  switchButton->setProperty("type", QVariant("power_switch"));
  connect(switchButton, SIGNAL(clicked()), this,
      SLOT(SpawnEntity()));

  QPushButton *motorGearboxButton = new QPushButton(tr("Motor with Gearbox"));
  motorGearboxButton->setCheckable(false);
  motorGearboxButton->setChecked(false);
  motorGearboxButton->setProperty("type", QVariant("motor_gearbox"));
  connect(motorGearboxButton, SIGNAL(clicked()), this,
      SLOT(SpawnEntity()));


  this->modelEditor->AddItemToPalette(motorButton, "Components");
  this->modelEditor->AddItemToPalette(batteryButton, "Components");
  this->modelEditor->AddItemToPalette(switchButton, "Components");
  this->modelEditor->AddItemToPalette(motorGearboxButton, "Components");

  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();

  this->restPub =
      this->node->Advertise<gazebo::msgs::RestPost>(
      "/gazebo/rest/rest_post");

  this->connections.push_back(gui::model::Events::ConnectNestedModelInserted(
      boost::bind(&CMLEditor::OnNestedModelInserted, this, _1)));

  this->connections.push_back(gui::model::Events::ConnectNestedModelRemoved(
      boost::bind(&CMLEditor::OnNestedModelRemoved, this, _1)));

  this->connections.push_back(gui::model::Events::ConnectLinkInserted(
      boost::bind(&CMLEditor::OnLinkInserted, this, _1)));

  this->connections.push_back(gui::model::Events::ConnectLinkRemoved(
      boost::bind(&CMLEditor::OnLinkRemoved, this, _1)));

  this->connections.push_back(gui::model::Events::ConnectJointInserted(
      boost::bind(&CMLEditor::OnJointInserted, this, _1, _2, _3, _4, _5)));

  this->connections.push_back(gui::model::Events::ConnectJointRemoved(
      boost::bind(&CMLEditor::OnJointRemoved, this, _1)));

  this->connections.push_back(
      gui::model::Events::ConnectOpenNestedModelInspector(
      boost::bind(&CMLEditor::OpenInspector, this, _1)));

  this->LoadModels();
}

/////////////////////////////////////////////////
CMLEditor::~CMLEditor()
{
  delete this->renderProxy;
}

/////////////////////////////////////////////////
void CMLEditor::OnElectricalConnection()
{
  emit CMLEvents::createConnection("electrical");
}

/////////////////////////////////////////////////
void CMLEditor::LoadModels()
{
  std::vector<std::string> names;
  names.push_back("motor");
  names.push_back("power_switch");
//  models.push_back("gearbox");
  names.push_back("AA_battery");
  names.push_back("motor_gearbox");

  for (unsigned int i = 0; i < names.size(); ++i)
  {
    std::string modelURI = "model://" + names[i];
    std::string uri =
        common::ModelDatabase::Instance()->GetModelFile(modelURI);
    sdf::SDFPtr modelSDF;
    modelSDF.reset(new sdf::SDF);
    sdf::initFile("root.sdf", modelSDF);

    if (!sdf::readFile(uri, modelSDF))
    {
      gzerr << "Unable to load [" << names[i] << "]\n";
      continue;
    }

    this->models[names[i]] = modelSDF;

//    this->Parse(modelSDF->Root()->GetElement("model")->GetElement("plugin"));
  }
}

/////////////////////////////////////////////////
void CMLEditor::Parse(sdf::ElementPtr _sdf, const std::string &_name)
{
  std::map<std::string, boost::any> ports;
  std::map<std::string, Simple_msgs::msgs::Variant> properties;

  if (_sdf->HasElement("port"))
  {
    sdf::ElementPtr childElem = _sdf->GetElement("port");
    while (childElem)
    {
      std::string name = childElem->Get<std::string>("name");

      ports[name] = 0;
      childElem = childElem->GetNextElement("port");
    }
  }
  if (_sdf->HasElement("properties"))
  {
    sdf::ElementPtr propertiesElem = _sdf->GetElement("properties");
    sdf::ElementPtr childElem = propertiesElem->GetFirstElement();
    while (childElem)
    {
      Simple_msgs::msgs::Variant valueVariant;
      std::string key = childElem->GetName();
      std::string value = childElem->GetValue()->GetAsString();
      std::transform(value.begin(), value.end(), value.begin(), ::tolower);
      if (value == "true")
      {
        valueVariant.set_type(Simple_msgs::msgs::Variant::BOOL);
        valueVariant.set_v_bool(true);
      }
      else if (value == "false")
      {
        valueVariant.set_type(Simple_msgs::msgs::Variant::BOOL);
        valueVariant.set_v_bool(false);
      }
      else
      {
        try
        {
          double valueD = boost::lexical_cast<double>(value);
          valueVariant.set_type(Simple_msgs::msgs::Variant::DOUBLE);
          valueVariant.set_v_double(valueD);
        }
        catch (const boost::bad_lexical_cast &)
        {
          valueVariant.set_type(Simple_msgs::msgs::Variant::STRING);
          valueVariant.set_v_string(value);
        }
      }

      /// FIXME hack
      if (key == "closed")
      {
        valueVariant.set_type(Simple_msgs::msgs::Variant::BOOL);
        if (value == "true" || value == "1")
          valueVariant.set_v_bool(true);
        else
          valueVariant.set_v_bool(false);
        //valueVariant.set_v_bool(boost::lexical_cast<bool>(value));
      }

      properties[key] = valueVariant;
      childElem = childElem->GetNextElement("");
    }
  }

  // DEBUG ============
  std::map<std::string, boost::any>::iterator dportIt;
  for (dportIt = ports.begin() ; dportIt != ports.end(); ++dportIt)
    std::cerr << " got port " << dportIt->first << std::endl;

  //for (unsigned int i = 0 ; i < this->ports.size(); ++i)
    //std::cerr << " got port " << this->ports[i] << std::endl;

  std::map<std::string, Simple_msgs::msgs::Variant>::iterator pIt;
  for (pIt = properties.begin() ; pIt != properties.end(); ++pIt)
  {
    boost::any value = CMLPropertyManager::ConvertVariant(pIt->second);
    std::string valueStr;
    try
    {
      valueStr = boost::any_cast<std::string>(value);
    }
    catch (const boost::bad_any_cast &)
    {
      try
      {
        std::stringstream ss;
        ss << boost::any_cast<double>(value);
        valueStr = ss.str();
      }
      catch (const boost::bad_any_cast &)
      {
        std::stringstream ss;
        ss << boost::any_cast<bool>(value);
        valueStr = ss.str();
      }
    }
    std::cerr << " got property " << pIt->first << ": " <<
        valueStr << std::endl;
  }
  // DEBUG ============

  Simple_msgs::msgs::SimpleModel msg;
  //msg.set_name(this->parent->GetScopedName());
  msg.set_name(_name);
  msg.set_schematic_type("");

  std::map<std::string, boost::any>::iterator portIt;
  for (portIt = ports.begin() ; portIt != ports.end(); ++portIt)
  {
    msg.add_port(portIt->first);
  }

  std::map<std::string, Simple_msgs::msgs::Variant>::iterator it;
  for (it = properties.begin(); it != properties.end(); ++it)
  {
    msg.add_key(it->first);
    Simple_msgs::msgs::Variant *property = msg.add_value();
    property->CopyFrom(it->second);
  }
  CMLManager::Instance()->AddSimpleModel(msg);
}


/////////////////////////////////////////////////
void CMLEditor::OnNestedModelInserted(const std::string &_name)
{
  sdf::ElementPtr entitySDF = this->modelEditor->GetEntitySDF(_name);
  if (entitySDF)
  {
    if (entitySDF->HasElement("plugin"))
    {
      this->Parse(entitySDF->GetElement("plugin"), _name);
    }
  }

  gazebo::msgs::RestPost restMsg;
  restMsg.set_route("/events/new");

  std::string postStr;
  postStr = "\"type\": \"existence\",";
  postStr += "\"name\": \"editor_model_inserted\",";
  postStr += "\"data\": {";
  postStr += "\"link\": \"" + _name + "\",";
  postStr += "\"state\": \"creation\"";
  postStr += "}";
  restMsg.set_json(postStr);
  this->restPub->Publish(restMsg);
}

/////////////////////////////////////////////////
void CMLEditor::OnNestedModelRemoved(const std::string &_name)
{
  CMLManager::Instance()->RemoveSimpleModel(_name);

  this->renderProxy->RemoveEntity(_name);

  gazebo::msgs::RestPost restMsg;
  restMsg.set_route("/events/new");

  std::string postStr;
  postStr = "\"type\": \"existence\",";
  postStr += "\"name\": \"editor_model_removed\",";
  postStr += "\"data\": {";
  postStr += "\"link\": \"" + _name + "\",";
  postStr += "\"state\": \"deletion\"";
  postStr += "}";
  restMsg.set_json(postStr);
  this->restPub->Publish(restMsg);
}

/////////////////////////////////////////////////
void CMLEditor::SpawnEntity()
{
  QPushButton *button =
     qobject_cast<QPushButton *>(QObject::sender());
  if (!button)
    return;

  QVariant type = button->property("type");

  std::map<std::string, sdf::SDFPtr>::iterator it =
      this->models.find(type.toString().toStdString());
  if (it == this->models.end())
    return;

  this->modelEditor->SpawnEntity(it->second->Root()->GetElement("model"));
}


/////////////////////////////////////////////////
void CMLEditor::OnLinkInserted(const std::string &_linkId)
{
  gazebo::msgs::RestPost restMsg;
  restMsg.set_route("/events/new");

  // remove model preview prefix
  std::string linkScopedName = _linkId;
  size_t pIdx = linkScopedName.find("::");
  if (pIdx != std::string::npos)
    linkScopedName = linkScopedName.substr(pIdx+2);

  std::string postStr;
  postStr = "\"type\": \"existence\",";
  postStr += "\"name\": \"editor_link_inserted\",";
  postStr += "\"data\": {";
  postStr += "\"link\": \"" + linkScopedName + "\",";
  postStr += "\"state\": \"creation\"";
  postStr += "}";
  restMsg.set_json(postStr);
  this->restPub->Publish(restMsg);
}

/////////////////////////////////////////////////
void CMLEditor::OnJointInserted(const std::string &_jointId,
    const std::string &_jointName, const std::string &_type,
    const std::string &_parentName, const std::string &_childName)
{
  gazebo::msgs::RestPost restMsg;
  restMsg.set_route("/events/new");

  // remove model preview prefix
  std::string parentScopedName = _parentName;
  size_t pIdx = parentScopedName.find("::");
  if (pIdx != std::string::npos)
    parentScopedName = parentScopedName.substr(pIdx+2);
  std::string childScopedName = _childName;
  pIdx = childScopedName.find("::");
  if (pIdx != std::string::npos)
    childScopedName = childScopedName.substr(pIdx+2);

  std::string postStr;
  if (_type != "wire")
  {
    postStr = "\"type\": \"connection\",";
    postStr += "\"name\": \"editor_joint_inserted\",";
    postStr += "\"data\": {";
    postStr += "\"id\": \"" + _jointName +"\",";
    postStr += "\"parent\": \"" + parentScopedName +"\",";
    postStr += "\"child\": \"" + childScopedName +"\",";
    postStr += "\"type\": \"" + _type +"\"";
    postStr += "}";
  }

  restMsg.set_json(postStr);
  this->restPub->Publish(restMsg);

}

/////////////////////////////////////////////////
void CMLEditor::OnLinkRemoved(const std::string &_linkId)
{
  gazebo::msgs::RestPost restMsg;
  restMsg.set_route("/events/new");

  // remove model preview prefix
  std::string linkScopedName = _linkId;
  size_t pIdx = linkScopedName.find("::");
  if (pIdx != std::string::npos)
    linkScopedName = linkScopedName.substr(pIdx+2);

  std::string postStr;
  postStr = "\"type\": \"existence\",";
  postStr += "\"name\": \"editor_link_removed\",";
  postStr += "\"data\": {";
  postStr += "\"link\": \"" + linkScopedName + "\",";
  postStr += "\"state\": \"deletion\"";
  postStr += "}";
  restMsg.set_json(postStr);
  this->restPub->Publish(restMsg);
}

/////////////////////////////////////////////////
void CMLEditor::OnJointRemoved(const std::string &_jointId)
{
  gazebo::msgs::RestPost restMsg;
  restMsg.set_route("/events/new");

  std::string postStr;
  postStr = "\"type\": \"connection\",";
  postStr += "\"name\": \"editor_joint_removed\",";
  postStr += "\"data\": {";
  postStr += "\"id\": \"" + _jointId +"\"";
  postStr += "}";


  restMsg.set_json(postStr);
  this->restPub->Publish(restMsg);
}

/////////////////////////////////////////////////
void CMLEditor::OpenInspector(const std::string &_name)
{
  std::cerr << " open inspector " << _name << std::endl;
  CMLManager::Instance()->ShowInspector(_name);
}
