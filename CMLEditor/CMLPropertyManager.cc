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

#include <iostream>

#include <gazebo/common/MouseEvent.hh>
#include <gazebo/gui/gui.hh>
#include <gazebo/gui/model/ModelEditor.hh>

#include "SimpleModel.pb.h"
#include "CMLComponentInspector.hh"
#include "CMLManager.hh"
#include "CMLPropertyManager.hh"

using namespace gazebo;
using namespace gui;


/////////////////////////////////////////////////
CMLPropertyManager::CMLPropertyManager()
{
}

/////////////////////////////////////////////////
CMLPropertyManager::~CMLPropertyManager()
{
}

/////////////////////////////////////////////////
void CMLPropertyManager::OnComponentProperyChanged()
{
  CMLComponentInspector *sender =
      qobject_cast<CMLComponentInspector *>(QObject::sender());
  Simple_msgs::msgs::SimpleModel msg = sender->GetSimpleModelMsg();


  CMLManager::Instance()->UpdateModelInfo(msg.name(), msg);
  // std::cerr << " msg " << msg.DebugString() << std::endl;

  ModelEditor *modelEditor =
        dynamic_cast<ModelEditor *>(gui::get_main_window()->Editor("model"));
  if (modelEditor)
  {
    sdf::ElementPtr entitySDF = modelEditor->GetEntitySDF(msg.name());
    if (entitySDF && entitySDF->HasElement("plugin"))
    {
      sdf::ElementPtr pluginElem = entitySDF->GetElement("plugin");
      if (pluginElem->HasElement("properties"))
      {
        sdf::ElementPtr propertiesElem = pluginElem->GetElement("properties");
        for (unsigned int i = 0; i < msg.key_size(); ++i)
        {
          Simple_msgs::msgs::Variant valueMsg = msg.value(i);
          std::string key = msg.key(i);

          if (propertiesElem->HasElement(key))
          {
            boost::any value = ConvertVariant(valueMsg);
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
                bool boolValue = boost::any_cast<bool>(value);
                valueStr = boolValue ? "true" : "false";
              }
            }
            propertiesElem->GetElement(key)->Set(valueStr);
          }
        }
      }
    }
  }
}

/////////////////////////////////////////////////
boost::any CMLPropertyManager::ConvertVariant(
    Simple_msgs::msgs::Variant _variant)
{
  switch (_variant.type())
  {
    case Simple_msgs::msgs::Variant::UINT32:
    {
      return _variant.v_uint32();
    }
    case Simple_msgs::msgs::Variant::INT32:
    {
      return _variant.v_int32();
    }
    case Simple_msgs::msgs::Variant::DOUBLE:
    {
      return _variant.v_double();
    }
    case Simple_msgs::msgs::Variant::STRING:
    {
      return _variant.v_string();
    }
    case Simple_msgs::msgs::Variant::BOOL:
    {
      return _variant.v_bool();
    }
    default:
    {
      return _variant.v_string();
    }
  }
  return _variant.v_string();
}
