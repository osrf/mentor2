#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Angle.hh>
#include <ignition/math/Helpers.hh>
#include <stdio.h>
#include <math.h>

namespace gazebo
{
  class ModelPush : public ModelPlugin
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

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&ModelPush::OnUpdate, this, _1));
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
//    	double rotationsPerSec = 2 * IGN_PI * fabs(axleAngularVel.x);
    	double rotationsPerSec = fabs(axleAngularVel.x) / 360;

    	// Velocity made good in meters/second
    	return rotationsPerSec * this->wheelCirc;
    }


    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
//    	std::cout << "OnUpdate()..." << std::endl << std::flush;

    	if (!this->axle)
    		return;

    	math::Vector3 model_wlv = this->modelLink->GetWorldLinearVel();
    	math::Vector3 axle_av = this->axle->GetRelativeAngularVel();

    	math::Vector3 model_wla = this->modelLink->GetRelativeLinearAccel();


    	double vmg = this->CalculateExpectedVMG(axle_av);

/*    	std::cout << "axle: x=" << std::setw(6) << axle_av.x << " y=" << std::setw(6) << axle_av.y << " z=" << std::setw(6) << axle_av.z << " "
    			  << "model: x=" << std::setw(6) << model_wlv.x << " y=" << std::setw(6) << model_wlv.y << " z=" << std::setw(6) << model_wlv.z << " "
				  << "vmg: " << vmg << " m/s" << std::endl;*/

//  static double c = 0;
//  c this->modelLink->GetWorldPose().pos.y;

  if (math::equal(std::fmod(this->model->GetWorld()->GetSimTime().Double(), 1.0), 0.0, 1e-4))
  {
    	std::cerr << "time: " << this->model->GetWorld()->GetSimTime().Double() << std::endl;
    	std::cout << " model: v " << std::setw(6) << model_wlv.x << " y=" << std::setw(6) << model_wlv.y << " z=" << std::setw(6) << model_wlv.z << std::endl;
    	std::cout << " model: a " << model_wla.x << " y=" << std::setw(6) << model_wla.y << " z=" << std::setw(6) << model_wla.z << std::endl;
    	std::cerr<< " pose: " << this->modelLink->GetWorldPose().pos << std::endl;
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

  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
