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
#ifndef _CML_EDITOR_HH_
#define _CML_EDITOR_HH_

#include <gazebo/gui/qt.h>
//#include <gazebo/gui/Editor.hh>

namespace gazebo
{
  namespace gui
  {
    class MainWindow;
//    class CMLEditorPalette;
    class CMLEditorWidget;

    /// \class CMLEditor CMLEditor.hh gui/gui.hh
    /// \brief Interface to the CML editor.
    class CMLEditor : public QObject/*: public Editor*/
    {
      Q_OBJECT

      /// \brief Constuctor.
      /// \param[in] _mainWindow Pointer to the mainwindow.
      public: CMLEditor(MainWindow *_mainWindow);

      /// \brief Destuctor.
      public: virtual ~CMLEditor();

      /// \brief QT callback when entering CML edit mode
      /// \param[in] _checked True if the menu item is checked
      private slots: void OnEdit(bool _checked);

      /// \brief Create menus
      //private: void CreateMenus();

//      private: gazebo::gui::CMLEditorPalette *CMLPalette;

      private: gazebo::gui::MainWindow *mainWindow;

      private: gazebo::gui::CMLEditorWidget *CMLEditorWidget;
    };
  }
}
#endif
