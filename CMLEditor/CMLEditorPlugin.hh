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

#include <gazebo/gazebo.hh>

namespace gazebo
{
  namespace gui
  {
    class CMLRender;
  }

  class CMLEditorPlugin : public SystemPlugin
  {
    /// \brief Destructor
    public: virtual ~CMLEditorPlugin();

    /// \brief Load the plugin.
    public: virtual void Load(int /*_argc*/, char ** /*_argv*/);

    /// \brief Initialize the plugin.
    private: virtual void Init();

    /// \brief Event callback when the gazebo main window has been intialized.
    private: void OnMainWindowReady();

    /// \brief A list of connections.
    private: std::vector<event::ConnectionPtr> connections;
  };
}
