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
#ifndef _CML_PROPERTY_MANAGER_HH_
#define _CML_PROPERTY_MANAGER_HH_


#include <map>
#include <gazebo/gui/qt.h>

namespace gazebo
{
  namespace gui
  {
    class CMLComponentInspector;

    /// \class CMLPropertyManager CMLPropertyManager.hh gui/gui.hh
    /// \brief Manager of simple model properties.
    class CMLPropertyManager : public QObject
    {
      Q_OBJECT

      /// \brief Constructor.
      public: CMLPropertyManager();

      /// \brief Destructor.
      public: ~CMLPropertyManager();

      /// \brief A map of component name to its inspector.
      public: std::map<std::string, CMLComponentInspector *>
          componentInspectors;

      /// \brief Callback when component properties have changed.
      public slots: void OnComponentProperyChanged();
    };
  }
}
#endif
