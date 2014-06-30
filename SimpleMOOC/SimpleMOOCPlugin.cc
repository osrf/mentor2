//#include "gazebo/gui/GuiIface.hh"
//#include "gazebo/rendering/rendering.hh"
#include "gazebo/gazebo.hh"

namespace gazebo
{
  class SimpleMOOCPlugin : public SystemPlugin
  {
    public: virtual ~SimpleMOOCPlugin()
    {
      //if (this->userCam)
      //  this->userCam->EnableSaveFrame(false);
    }

    public: virtual void Load(int /*_argc*/, char ** /*_argv*/)
    {
        std::cerr << "Simple MOOC server plugin Load()" <<  std::endl;
    }

    private: virtual void Init()
    {
      // Get a pointer to the active user camera
      rendering::UserCameraPtr userCam;
      //userCam = gui::get_active_camera();

      std::cerr << "Simple MOOC server plugin Init()" <<  std::endl;

    }

    private: std::vector<event::ConnectionPtr> connections;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_SYSTEM_PLUGIN(SimpleMOOCPlugin)
}

