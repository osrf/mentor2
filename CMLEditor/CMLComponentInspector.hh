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

#ifndef _CML_COMPONENT_INSPECTOR_HH_
#define _CML_COMPONENT_INSPECTOR_HH_

#include <gazebo/gui/qt.h>

#include "SimpleModel.pb.h"

namespace Simple_msgs
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
    /// \class CMLComponentInspector CMLComponentInspector.hh
    /// \brief Dialog for configuring component properties.
    class CMLComponentInspector : public QDialog
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent QWidget.
      public: CMLComponentInspector(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLComponentInspector();

      /// \brief Load the inspector with a simple model message
      /// \param[in] _msg A simple model message that contains property
      /// information.
      public: void Load(const Simple_msgs::msgs::SimpleModel *_msg);

      /// \brief Update the inspector from a simple model message
      /// \param[in] _msg A simple model message that contains property
      /// information.
      public: void UpdateFromMsg(const Simple_msgs::msgs::SimpleModel *_msg);

      /// \brief Get a copy of the simple model message.
      /// \return the simple model message.
      public: Simple_msgs::msgs::SimpleModel GetSimpleModelMsg();

      /// \brief Get the most suitable widget for configuring this property.
      /// \param[in] _msg Variant message containing property type and value.
      /// \return a Qt widget for configuring the property.
      private: QWidget *GetValueWidget(
          const Simple_msgs::msgs::Variant *_msg);

      /// \brief Update the internal copy of simple model message from widgets.
      private: void Update();

      /// \brief Qt signal emitted to indicate that changes should be applied.
      Q_SIGNALS: void Applied();

      /// \brief Qt callback when the bool property changed.
      /// \param[in] _state New bool state.
      private slots: void BoolPropChanged(int _state);

      /// \brief Qt callback when the Cancel button is pressed.
      private slots: void OnCancel();

      /// \brief Qt callback when the Apply button is pressed.
      private slots: void OnApply();

      /// \brief Qt callback when the Ok button is pressed.
      private slots: void OnOK();

      /// \brief A layout for the properties.
      private: QGridLayout *propertyLayout;

      /// \brief A map of key name to the associated widget.
      private: std::map<std::string, QWidget *> valueWidgets;

      /// \brief A simple model message containing configurable properties.
      private: Simple_msgs::msgs::SimpleModel simpleModelMsg;
    };
  }
}

#endif
