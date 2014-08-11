/*
 * Copyright (C) 2012-2014 Open Source Robotics Foundation
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

#ifndef _CML_PORT_INSPECTOR_HH_
#define _CML_PORT_INSPECTOR_HH_

#include <gazebo/gui/qt.h>


namespace SimpleModel_msgs
{
  namespace msgs
  {
    class SimpleModel;
  }
}

namespace gazebo
{
  namespace gui
  {
    /// \class CMLPortInspector CMLPortInspector.hh
    /// \brief Dialog for selecting a ports.
    class CMLPortInspector : public QDialog
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent QWidget.
      public: CMLPortInspector(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLPortInspector();

      /// \brief Load the inspector with a simple model message
      /// \param[in] _msg A simple model message that contains port information.
      public: void Load(const SimpleModel_msgs::msgs::SimpleModel *_msg);

      /// \brief Qt signal emitted to indicate that changes should be applied.
      Q_SIGNALS: void Applied();

      /// \brief Qt callback when the Cancel button is pressed.
      private slots: void OnCancel();

      /// \brief Qt callback when the Apply button is pressed.
      private slots: void OnApply();

      /// \brief Qt callback when the Ok button is pressed.
      private slots: void OnOK();

      /// \brief Radio buttons representing each port.
      private: std::vector<QRadioButton *> portRadioButtons;

      /// \brief A group box container for all ports.
      private: QGroupBox *portsGroupBox;

      /// \brief A group box for hold all ports.
      private: QButtonGroup *portsButtonGroup;

      /// \brief A layout for the ports.
      private: QVBoxLayout *portsLayout;
    };
  }
}

#endif
