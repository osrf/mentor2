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

#ifndef _CML_EDITOR_PALETTE_HH_
#define _CML_EDITOR_PALETTE_HH_

#include <string>
#include <vector>
#include <list>

#include <gazebo/gui/qt.h>

namespace gazebo
{
  namespace gui
  {
    /// \class CMLEditorPalette CMLEditorPalette.hh
    /// \brief A palette of items which can be added to the editor.
    class CMLEditorPalette : public QWidget
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent QWidget.
      public: CMLEditorPalette(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLEditorPalette();

      /// \brief Received item selection user input.
      /// \param[in] _item Item selected.
      /// \param[in] _column Column index.
      private slots: void OnItemSelection(QTreeWidgetItem *_item, int _column);

      /// \brief Qt callback when the back button is pushed.
      private slots: void OnBack();

      /// \brief Qt callback when an electrical connection is to be created.
      private slots: void OnElectricalConnection();

      /// \brief Widget that display components.
      private: QTreeWidget *componentTreeWidget;

      /// \brief Widget that handles pagination.
      private: QStackedWidget *pageStackWidget;

      /// \brief Back button for page navigation.
      private: QPushButton *backButton;
    };
  }
}

#endif