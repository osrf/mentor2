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

#ifndef _CML_ITEM_LIST_WIDGET_HH_
#define _CML_ITEM_LIST_WIDGET_HH_

#include <string>
#include <vector>

#include <gazebo/gui/qt.h>

namespace gazebo
{
  namespace gui
  {
    /// \class CMLItemListWidget CMLItemListWidget.hh
    /// \brief A widget to display a list of components or connections and their
    /// properties.
    class CMLItemListWidget : public QWidget
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent QWidget.
      public: CMLItemListWidget(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLItemListWidget();

      /// \brief Set the item list to be displayed.
      /// \param[in] _name Name of the item list to be displayed.
      public: void SetCurrentItemList(const std::string &_name);

      /// \brief Populate the widget with a list of item names.
      /// \param[in] _items A list of items used to populate the widget.
      public: void Populate(const std::map<std::string, std::string> &_items);

      /// \brief Get the number of items in the current item list widget.
      /// \return Number of items.
      public: unsigned int GetItemCount();

      /// \brief Qt callback when an item is clicked.
      /// \param[in] _item Name of the name that is clicked.
      private slots: void OnItemSelected(QString _url);

      /// \brief A Qt signal mapper to map item clicked signals to its name.
      private: QSignalMapper *itemsSignalMapper;

      /// \brief A Qt label to display the name.
      private: QLabel *nameLabel;

      /// \brief A map of item list names to the associated widget.
      private: std::map<std::string, QWidget *> itemsMap;

      /// \brief Stacked widget for storing item list widgets.
      private: QStackedWidget *stackedWidget;
    };
  }
}

#endif
