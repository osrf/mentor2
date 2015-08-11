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


#include <sdf/sdf.hh>

#include <gazebo/common/Events.hh>
#include <gazebo/gui/qt.h>
//#include <gazebo/gui/Editor.hh>

#include "CMLRender.hh"

namespace gazebo
{
  namespace gui
  {
    class ModelEditor;
    class MainWindow;
    class CMLRender;

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

      private: void Parse(sdf::ElementPtr _sdf, const std::string &_name);

      private: void LoadModels();

      /// \brief Callback when a nested mdoel is inserted.
      /// \param[in] _node Name of nested model.
      private: void OnNestedModelInserted(const std::string &_name);

      /// \brief Callback when a nested model is removed.
      /// \param[in] _node Name of nested model.
      private: void OnNestedModelRemoved(const std::string &_name);

      /// \brief Callback when a link is inserted.
      /// \param[in] _linkName Scoped link name.
      private: void OnLinkInserted(const std::string &_linkId);

      /// \brief Callback when a link is removed.
      /// \param[in] _linkId Unique link identifying name.
      private: void OnLinkRemoved(const std::string &_linkId);

      /// \brief Callback when a joint is added.
      /// \param[in] _jointId Unique joint identifying name.
      /// \param[in] _jointName Scoped name which can be changed by the user.
      /// \param[in] _type Type of joint.
      /// \param[in] _parentName Scoped name of the parent link.
      /// \param[in] _childName Scoped name of the child link.
      private: void OnJointInserted(const std::string &_jointId,
          const std::string &_jointName, const std::string &_type,
          const std::string &_parentName, const std::string &_childName);

      /// \brief Callback when a joint is removed.
      /// \param[in] _jointId Unique joint identifier.
      private: void OnJointRemoved(const std::string &_jointId);

      /// \brief Qt callback when an entity is to be spawned
      private slots: void SpawnEntity();

      /// \brief Qt callback when an electrical connection is to be created.
      private slots: void OnElectricalConnection();

      private: gazebo::gui::MainWindow *mainWindow;

      private: std::string insertName;

      private: std::map<std::string, sdf::SDFPtr> models;

      private: ModelEditor *modelEditor;

      private: CMLRender *renderProxy;

      /// \brief Communication Node
      private: transport::NodePtr node;

      /// \brief Publish rest posts
      private: transport::PublisherPtr restPub;

      /// \brief A list of gazebo event connects.
      private: std::vector<event::ConnectionPtr> connections;
    };
  }
}
#endif
