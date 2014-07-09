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

#ifndef _CML_EDITOR_ITEM_HH_
#define _CML_EDITOR_ITEM_HH_

#include <string>
#include <gazebo/gui/qt.h>

namespace gazebo
{
  namespace gui
  {
    /// \class CMLEditorItem CMLEditorItem.hh
    /// \brief Base class of an item in the CML editor.
    class CMLEditorItem : public QObject
    {
      Q_OBJECT

      /// \brief Constructor
      public: CMLEditorItem();

      /// \brief Destructor
      public: ~CMLEditorItem();

      /// \brief Get the size of the item in pixels.
      /// \return Size in pixels.
      public: virtual QVector3D GetSize() const;

      /// \brief Get the scene position of editor item.
      /// \return Scene position in pixel coordinates.
      public: virtual QVector3D GetScenePosition() const;

      /// \brief Get the scene rotation of the editor item.
      /// \return Scene rotation in degrees.
      public: virtual double GetSceneRotation() const;

      /// \brief Get the type of the editor item.
      /// \return Type of the item.
      public: virtual std::string GetType() const;

      /// \brief Get the name of the editor item.
      /// \return Name of the item.
      public: virtual std::string GetName() const;

      /// \brief Set the name of this editor item.
      /// \param[in] _name Name to set the editor item to.
      public: virtual void SetName(const std::string &_name);

      /// \brief Qt signal emitted when the editor item is being deleted.
      Q_SIGNALS: void ItemDeleted();

      /// \brief Type of editor item.
      protected: std::string editorType;

      /// \brief Name of editor item.
      protected: std::string name;
    };
    /// \}
  }
}

#endif
