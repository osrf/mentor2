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

#ifndef _BREADCRUMB_HH_
#define _BREADCRUMB_HH_

#include <string>
#include <vector>

#include <gazebo/gui/qt.h>

namespace gazebo
{
  namespace gui
  {
    /// \class BreadCrumbWidget BreadCrumbWidget.hh
    /// \brief A bread crumb widget for paged stack navigation.
    class BreadCrumbWidget : public QWidget
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent QWidget.
      public: BreadCrumbWidget(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~BreadCrumbWidget();

      /// \brief Push a value at the end of the bread crumbs
      /// \param[in] _value A value to be pushed to the end of the bread crumbs.
      public: void Push(const std::string &_value);

      /// \brief Pop a bread crumb
      public: std::string Pop();

      /// \brief Set the current index of the bread  crumb, this will truncate
      /// the all crumbs after the index.
      /// \param[in] _index Index to set to.
      public: void SetCurrentIndex(unsigned int _index);

      /// \brief Truncate the bread crumb
      /// \param[in] _start Start index of the truncation.
      /// \param[in] _end End index of the truncation.
      public: void Truncate(unsigned int _start, unsigned int _end = 0);

      /// \brief Qt signal emitted when the bread crumb index is changed.
      /// \param[in] _index Index of the bread crumb
      Q_SIGNALS: void IndexChanged(int _index);

      /// \brief Qt callback when a bread crumb is cliked
      private slots: void OnClicked();

      /// \brief Push a separator at the end of the bread crumbs
      private: void PushSeparator();

      /// \brief Pop a separator from the end of the bread crumbs
      private: void PopSeparator();

      /// \brief Qt horizontal layout to store each bread crumb.
      private: QHBoxLayout *breadCrumbLayout;

      /// \brief Current index in the list of bread crumbs.
      private: unsigned int currentIndex;

    };
  }
}

#endif
