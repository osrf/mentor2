/*
 * Copyright 2013 Open Source Robotics Foundation
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

#ifndef _CML_CONNECTION_MAKER_HH_
#define _CML_CONNECTION_MAKER_HH_

#include <string>
#include <vector>

#include <sdf/sdf.hh>

#include "gazebo/common/MouseEvent.hh"
#include "gazebo/common/CommonTypes.hh"
#include "gazebo/math/Vector3.hh"
#include "gazebo/rendering/RenderTypes.hh"
#include <gazebo/gui/qt.h>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>

namespace gazebo
{
  namespace gui
  {
    class ConnectionData;

    /// \class CMLConnectionMaker CMLConnectionMaker.hh
    /// \brief Connection maker
    class CMLConnectionMaker : public QObject,
                               public SingletonT<CMLConnectionMaker>
    {
      Q_OBJECT

      /// \enum Connection types
      /// \brief Unique identifiers for connection types that can be created.
      public: enum ConnectType
      {
        /// \brief none
        CONNECT_NONE,

        /// \brief Electrical
        CONNECT_ELECTRICAL,

        /// \brief Mechanical
        CONNECT_MECHANICAL
      };

      /// \brief Constructor
      public: CMLConnectionMaker();

      /// \brief Destructor
      public: virtual ~CMLConnectionMaker();

      /// \brief Reset the connection maker;
      public: void Reset();

      /// \brief Add a connection
      /// \param[in] _type Type of connection to be added in string.
      public: void AddConnection(const std::string &_type);

      /// \brief Add a connection
      /// \param[in] _type Type of connection to be added
      public: void AddConnection(ConnectType _type);

      /// \brief Create a connection
      /// \param[in] _parent Parent of the connection.
      /// \param[in] _child Child of the connection.
      public: ConnectionData *CreateConnection(rendering::VisualPtr _parent,
          rendering::VisualPtr _child);

      /// \brief Update callback on PreRender.
      public: void Update();

      /// \brief Update callback on PreRender.
      public: void OnFinish();

      /// \brief Remove connection by name
      /// \param[in] _connectionName Name of connection to be removed.
      public: void RemoveConnection(const std::string &_connectionName);

      /// \brief Remove all connections of an entity
      /// \param[in] _entityName Name of entity.
      public: void RemoveConnectionsByEntity(const std::string &_entityName);

      /// \brief Get state
      /// \return State of ConnectType if connection creation is in process,
      /// otherwise CONNECT_NONE
      public: CMLConnectionMaker::ConnectType GetState() const;

      /// \brief Stop the process of creating connection.
      public: void Stop();

      /// \brief Queue a connection to be created.
      /// \param[in] _connectionElem Connection SDF element.
      /// \param[in] _modelName Name of the model that contains this connection.
      public: void CreateConnection(sdf::ElementPtr _connectionElem,
          const std::string &_modelName = "");

      /// \brief Parse and create connections from sdf.
      /// \param[in] _connectionElem Connection SDF element.
      /// \param[in] _modelName Name of the model that contains this connection.
      public: void CreateConnectionFromSDF(sdf::ElementPtr _connectionElem,
          const std::string &_modelName = "");

      /// \brief Create SDF from a connection.
      /// \param[in] _connection Connection data.
      /// \return An SDF element.
      public: sdf::ElementPtr CreateConnectionSDF(ConnectionData *_connection);

      /// \brief Enable the mouse and key event handlers.
      public: void EnableEventHandlers();

      /// \brief Disable the mouse and key event handlers.
      public: void DisableEventHandlers();

      /// \brief Deselect all currently selected connection visuals.
      public: void DeselectAll();

      /// \brief Set the select state of a connection.
      /// \param[in] _name Name of the connection.
      /// \param[in] _selected True to select the connection.
      public: void SetSelected(const std::string &_name, const bool selected);

      /// \brief Set the select state of a connection visual.
      /// \param[in] _jointVis Pointer to the connection visual.
      /// \param[in] _selected True to select the connection.
      public: void SetSelected(rendering::VisualPtr _connectionVis,
          const bool selected);

      /// \brief Creates a name for a connection.
      /// \param[in] _parent Name of parent part of connection.
      /// \param[in] _parentPort Name of parent port.
      /// \param[in] _child Name of child part of connection.
      /// \param[in] _childPort Name of child port.
      public: std::string CreateConnectionName(const std::string &_parent,
          const std::string &_parentPort, const std::string &_child,
          const std::string &_childPort) const;

      /// \brief Creates a name for a connection.
      /// \param[in] _connection Connection data.
      public: std::string CreateConnectionName(
          const ConnectionData &_connection) const;

      /// \brief Insert the connection sdf element to the model's plugin sdf.
      /// \param[in] _connection Connection data.
      private: void InsertConnectionElement(ConnectionData *_connection);

      /// \brief Remove the connection sdf element from the model's plugin sdf.
      /// \param[in] _connection Connection data.
      private: void RemoveConnectionElement(ConnectionData *_connection);

      /// \brief Mouse event filter callback when mouse button is pressed.
      /// \param[in] _event The mouse event.
      /// \return True if the event was handled
      private: bool OnMousePress(const common::MouseEvent &_event);

      /// \brief Mouse event filter callback when mouse button is released.
      /// \param[in] _event The mouse event.
      /// \return True if the event was handled
      private: bool OnMouseRelease(const common::MouseEvent &_event);

      /// \brief Mouse event filter callback when mouse is moved.
      /// \param[in] _event The mouse event.
      /// \return True if the event was handled
      private: bool OnMouseMove(const common::MouseEvent &_event);

      /// \brief Key event filter callback when key is pressed.
      /// \param[in] _event The key event.
      /// \return True if the event was handled
      private: bool OnKeyPress(const common::KeyEvent &_event);

      /// \brief Callback to show the connection's context menu
      /// \param[in] _connectionId Name of the connection.
      private: void OnShowConnectionContextMenu(
          const std::string &_connectionId);

      /// \brief Qt Callback when a connection is to be deleted.
      private slots: void OnDelete();

      /// \brief Callback for when a joint is removed.
      /// \param[in] _id Name of the joint.
      public: void OnJointRemoved(const std::string &_id);

      /// \brief Helper method to create hotspot visual for mouse interaction.
      //// \param[in] _connect Create a hotspot from the connect data.
      private: void CreateHotSpot(ConnectionData *_connect);

      /// \brief Select a port on the model.
      /// return Name of port selected, empty string otherwise.
      private: std::string SelectPort();

      /// \brief Check if a visual is a component
      /// \param[in] _name Visual name
      /// return True if the visual is a component.
      private: bool IsComponent(const std::string &_name);

      /// \brief Get lowest level visual that represents a component
      /// \param[in] _visual Leaf visual.
      /// return Visual of type component.
      private: rendering::VisualPtr GetLowestLevelComponentVisual(
          rendering::VisualPtr _visual);

      /// \brief Callback when an entity is selected.
      /// \param[in] _name Name of entity.
      /// \param[in] _mode Select mode
      private: void OnSetSelectedEntity(const std::string &_name,
          const std::string &_mode);

      /// \brief Callback when a link is selected.
      /// \param[in] _name Name of link.
      /// \param[in] _selected True if the link is selected, false if
      /// deselected.
      private: void OnSetSelectedLink(const std::string &_name, bool _selected);

      /// \brief Callback when a connection is selected.
      /// \param[in] _name Name of joint.
      /// \param[in] _selected True if the connection is selected, false if
      /// deselected.
      private: void OnSetSelectedConnection(const std::string &_name,
          const bool _selected);

      /// \brief Qt signal when the connection creation process has ended.
      //Q_SIGNALS: void ConnectionAdded();

      /// \brief Type of connection to create
      private: CMLConnectionMaker::ConnectType connectType;

      /// \brief Visual that is currently hovered over by the mouse
      private: rendering::VisualPtr hoverVis;

      /// \brief Currently selected visual
      private: rendering::VisualPtr selectedVis;

      /// \brief Visual that is currently being inspected.
      private: rendering::VisualPtr inspectVis;

      /// \brief All connections created by connection maker.
      private: std::map<std::string, ConnectionData *> connects;

      /// \brief Connection currently being created.
      private: ConnectionData *mouseConnection;

      /// \brief All the event connections.
      private: std::vector<event::ConnectionPtr> connections;

      /// \brief Flag set to true when a connection is established.
      private: bool newConnectionCreated;

      /// \brief Counter for the number of connections.
      private: int connectCounter;

      /// \brief Mutex to protect the list of joints
      private: boost::recursive_mutex *updateMutex;

      /// \brief Selected joint.
      private: rendering::VisualPtr selectedConnection;

      /// \brief A map of connection type to its corresponding material.
      private: std::map<CMLConnectionMaker::ConnectType, std::string>
          connectionMaterials;

      private: common::MouseEvent mouseEvent;

      /// \brief Qt action for deleting a connection.
      private: QAction *deleteAct;

      /// \brief Name of connection to be deleted.
      private: std::string deleteName;

      private: std::list<std::pair<sdf::ElementPtr, std::string> >
          connectionsToAdd;

      /// \brief This is a singleton class.
      private: friend class SingletonT<CMLConnectionMaker>;
    };



    /// \class ConnectionData ConnectionData.hh
    /// \brief Helper class to store connection data
    class ConnectionData
    {
      /// \brief Visual of the dynamic line
      public: rendering::VisualPtr visual;

      /// \brieft Visual of the hotspot
      public: rendering::VisualPtr hotspot;

      /// \brief Parent visual of the connection.
      public: rendering::VisualPtr parent;

      /// \brief Child visual of the connection.
      public: rendering::VisualPtr child;

      /// \internal
      /// \brief Parent visual pose used to determine if updates are needed.
      public: math::Pose parentPose;

      /// \internal
      /// \brief Child visual pose used to determine if updates are needed.
      public: math::Pose childPose;

      /// \brief Parent port of the connection.
      public: std::string parentPort;

      /// \brief Child port of the connection.
      public: std::string childPort;

      /// \brief Visual line used to represent the connection
      public: rendering::DynamicLines *line;

      /// \brief Type of connection.
      public: CMLConnectionMaker::ConnectType type;

      /// \brief True if the connection visual needs update.
      public: bool dirty;
    };
  }
}
#endif
