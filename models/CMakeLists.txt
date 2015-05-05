set (dirs
  axle_wheels
  battery_holder
  battery_pack
  bucket_wheel
  car
  chassis
  concrete_slab
  concrete_slab_small
  motor
  motor_box
  power_switch
  ramp
  vehicle
  vehicle_no_motor
  wheel_ramps
  AA_battery
)

INSTALL(DIRECTORY ${dirs} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/models)
