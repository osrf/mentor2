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

#ifndef _CML_EDITOR_WIDGET_HH_
#define _CML_EDITOR_WIDGET_HH_

#include <gazebo/gui/qt.h>

namespace gazebo
{
  namespace gui
  {
    /// \class CMLEditorWidget CMLEditorWidget.hh
    /// \brief The parent widget of the CML editor
    class CMLEditorWidget : public QWidget
    {
      /// \brief Constructor
      /// \param[in] _parent Parent QWidget.
      public: CMLEditorWidget(QWidget *_parent = 0);

      /// \brief Destructor
      public: ~CMLEditorWidget();

      /// \brief Qt event received when the widget is being resized
      /// \param[in] _event Resize event.
      private: void resizeEvent(QResizeEvent *_event);

      /// \brief Qt Graphics Scene where graphics items are drawn in
      private: QGraphicsScene *scene;

      /// \brief Minimum width of the Qt graphics scene
      private: int minimumWidth;

      /// \brief Minimum height of the Qt graphics scene
      private: int minimumHeight;
    };
  }
}

#endif
