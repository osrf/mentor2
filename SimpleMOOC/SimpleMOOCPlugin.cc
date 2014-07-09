
#include <gazebo/gazebo.hh>

#include "SimpleMOOC.pb.h"


typedef const boost::shared_ptr<const SimpleMOOC_msgs::msgs::SimpleMOOCLoginRequest> ConstSimpleMOOCLoginRequestPtr;

namespace gazebo
{

  class SimpleMOOCPlugin : public SystemPlugin
  {

    public: SimpleMOOCPlugin()
      :node(new gazebo::transport::Node())
    {
      
    }

    public: virtual ~SimpleMOOCPlugin()
    {
      //if (this->userCam)
      //  this->userCam->EnableSaveFrame(false);
      node.reset();
      sub.reset();
    }

    public: virtual void Load(int /*_argc*/, char ** /*_argv*/)
    {
      std::cerr << "Simple MOOC server plugin Load()" <<  std::endl;
    }

    /////////////////////////////////////////////////
    // Function is called everytime a message is received.
    public: void OnLoginRequest(ConstSimpleMOOCLoginRequestPtr &_msg )
    {
      std::cout << "MESSAGE RECEIVED!!" << std::endl;
    }


    private: virtual void Init()
    {
      std::cerr << "Simple MOOC server plugin Init()" <<  std::endl;
      // setup our node for communication
      node->Init();
      sub = node->Subscribe("~/MOOC", &SimpleMOOCPlugin::OnLoginRequest, this);  
 
    }

    private: gazebo::transport::NodePtr node;
    private: gazebo::transport::SubscriberPtr sub;
    private: std::vector<event::ConnectionPtr> connections;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_SYSTEM_PLUGIN(SimpleMOOCPlugin)
}

