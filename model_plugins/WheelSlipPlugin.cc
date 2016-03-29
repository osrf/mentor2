#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Angle.hh>
#include <ignition/math/Helpers.hh>
#include <stdio.h>
#include <math.h>
#include <gazebo/msgs/msgs.hh>

namespace gazebo
{
  class WheelSlipPlugin : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
    	std::cout << "Load()...." << std::endl << std::flush;

      // Store the pointer to the model
      this->model = _parent;

      this->modelLink = this->model->GetLink("link");
      if (!this->modelLink)
      {
    	  gzerr << "Missing model link element" << std::endl;
    	  return;
      }
      physics::ModelPtr axleWheels = this->model->NestedModel("axle_wheels");
      if (!axleWheels)
      {
    	  gzerr << "Missing nested axle_wheels model element" << std::endl;
    	  return;
      }
	  this->axle = axleWheels->GetLink("link");
	  if (!this->axle)
	  {
		  gzerr << "Missing axle_wheels link element" << std::endl;
		  return;
      }
      physics::CollisionPtr wheelColl = axleWheels->GetChildCollision("wheel_0_collision");
      if (!wheelColl)
      {
    	  gzerr << "Missing wheel_0_collision collision element" << std::endl;
      }
      if ((wheelColl->GetShapeType() & (~physics::Entity::SHAPE)) == physics::Entity::CYLINDER_SHAPE)
      {
  		  boost::shared_ptr<physics::CylinderShape> cyl = boost::dynamic_pointer_cast<physics::CylinderShape>(wheelColl->GetShape());
  		  if (cyl)
  		  {
  			  this->wheelCirc = 2 * IGN_PI * cyl->GetRadius();
  			  std::cout << "Wheel: r=" << cyl->GetRadius() << " C=" << this->wheelCirc << std::endl;
  		  }
      }

      this->node = transport::NodePtr(new transport::Node());
      this->node->Init();

      // Create a publisher on the Rest plugin topic
      this->restPub = this->node->Advertise<gazebo::msgs::RestPost>("/gazebo/rest/rest_post");

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&WheelSlipPlugin::OnUpdate, this, _1));
    }

    /////////////////////////////////////////////////
    public: void Init()
    {
    	ModelPlugin::Init();
    	std::cout << "Init()..." << std::endl << std::flush;

    	std::cout.setf(std::ios::fixed, std::ios::floatfield);
    	std::cout.precision(3);
    }

    private: double CalculateExpectedVMG(math::Vector3 axleAngularVel)
    {
    	// 2 pi radians = 360 degrees = 1 revolution
    	double revsPerSec = fabs(axleAngularVel.x) / (2 * IGN_PI);

    	// Velocity made good in meters/second
    	return revsPerSec * this->wheelCirc;
    }


    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
    	static int i = 0;
    	static int n = 0;
    	//    	std::cout << "OnUpdate()..." << std::endl << std::flush;

    	if (!this->axle)
    		return;

    	math::Vector3 model_wlv = this->modelLink->GetWorldLinearVel();
    	math::Vector3 axle_av = this->axle->GetRelativeAngularVel();

    	double vmg = this->CalculateExpectedVMG(axle_av);
    	double diff = fabs(fabs(model_wlv.y) - vmg);


//    	if ((++i % 5) == 0) {
//			std::cout << "axle: x=" << std::setw(6) << axle_av.x << " y=" << std::setw(6) << axle_av.y << " z=" << std::setw(6) << axle_av.z << " "
//					  << "model: x=" << std::setw(6) << model_wlv.x << " y=" << std::setw(6) << model_wlv.y << " z=" << std::setw(6) << model_wlv.z << " "
//					  << "vmg: " << vmg << " m/s diff: " << diff << std::endl;
//    	}

    	if (n++ >= 500) {

			std::string parentScopedName("HOMEBOY");
			std::string _parentPort("123");
			std::string childScopedName("PUNKIE");
			std::string _childPort("456");

			std::string postStr;

			postStr = "\"type\": \"connection\",";
			postStr += "\"name\": \"simple_connection\",";
			postStr += "\"data\": {";
			postStr += "\"parent\": \"" + parentScopedName + "\",";
			postStr += "\"parent_port\": \"" + _parentPort + "\",";
			postStr += "\"child\": \"" + childScopedName + "\",";
			postStr += "\"child_port\": \"" + _childPort + "\"";
			postStr += "}";

			gazebo::msgs::RestPost restMsg;
			restMsg.set_route("/events/new");

			std::cout << ">>> publishing msg " << postStr << std::endl;

			restMsg.set_json(postStr);
			this->restPub->Publish(restMsg);

    	    n = 0;

    	}

    }

    // Pointer to the model
    private: physics::ModelPtr model;

    private: physics::LinkPtr modelLink;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;

    // Pointer to the axle link
    private: physics::LinkPtr axle;

    // Circumference of wheel
    private: double wheelCirc;

    // Pointer to the node
    private: transport::NodePtr node;

    /// \brief a way to send messages to the MOOC topic (to the REST)
    private: transport::PublisherPtr restPub;

  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(WheelSlipPlugin)
}
