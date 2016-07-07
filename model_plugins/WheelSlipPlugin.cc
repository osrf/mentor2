#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Angle.hh>
#include <ignition/math/Helpers.hh>
#include <stdio.h>
#include <math.h>
#include <gazebo/msgs/msgs.hh>
#include <ctime>
#include <sstream>

namespace gazebo
{
  class WheelSlipPlugin : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
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
    	static int n = 0;

    	if (!this->axle)
    		return;

    	math::Vector3 Vl_model = this->modelLink->GetRelativeLinearVel();
    	math::Vector3 Va_axle = this->axle->GetRelativeAngularVel();

    	double vmg = this->CalculateExpectedVMG(Va_axle);
    	double diff = fabs(fabs(Vl_model.y) - vmg);

//    	if ((++n % 5) == 0) {
//			std::cout << "axle: x=" << std::setw(6) << axle_av.x << " y=" << std::setw(6) << axle_av.y << " z=" << std::setw(6) << axle_av.z << " "
//					  << "model: x=" << std::setw(6) << model_wlv.x << " y=" << std::setw(6) << model_wlv.y << " z=" << std::setw(6) << model_wlv.z << " "
//					  << "vmg: " << vmg << " m/s diff: " << diff << std::endl;
//    	}

    	bool slipping = (diff > 0.001);

    	if (slipping) {
    		std::time_t now = time(NULL);

    		if (abs(this->lastSlipMsgSent - now) > this->maxSlipMsgIntervalSecs) {

    			std::stringstream postMsgStream;

    			postMsgStream << "\"type\": \"traction\","
    					      << "\"name\": \"wheel_slip\","
						      << "\"data\": {"
							  << "\"state\": \"slipping\","
							  << "\"vehicle\": {"
							  << "\"expected_velocity\": \"" << vmg << "\","
							  << "\"actual_velocity\": \"" << Vl_model.y << "\""
							  << "}"
							  << "}";

				gazebo::msgs::RestPost restPostMsg;
				restPostMsg.set_route("/events/new");

				restPostMsg.set_json(postMsgStream.str());
				this->restPub->Publish(restPostMsg);

				gzdbg << postMsgStream.str() << std::endl;

    			this->lastSlipMsgSent = now;
    		}
    		this->wasSlipping = true;

    	} else if (this->wasSlipping) {

			std::stringstream postMsgStream;

			postMsgStream << "\"type\": \"traction\","
					   	  << "\"name\": \"wheel_slip\","
					      << "\"data\": {"
					      << "\"state\": \"gripping\""
			              << "}";

			gazebo::msgs::RestPost restPostMsg;
			restPostMsg.set_route("/events/new");

			restPostMsg.set_json(postMsgStream.str());
			this->restPub->Publish(restPostMsg);

			gzdbg << postMsgStream.str() << std::endl;

			this->wasSlipping = false;
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

    /// \brief Time when the last slip message was sent
    private: std::time_t lastSlipMsgSent = 0;

    /// \brief Max slip message interval in seconds
    private: int maxSlipMsgIntervalSecs = 3;

    /// \brief Whether or not the vehicle was slipping
    private: bool wasSlipping = false;

  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(WheelSlipPlugin)
}
