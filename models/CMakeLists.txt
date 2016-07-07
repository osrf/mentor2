set (dirs
  axle_wheels
  battery_holder
  battery_9v
  battery_pack
  bucket_wheel
  car
  caster_wheel
  chassis
  chassis_v2
  concrete_slab
  concrete_slab_small
  gearbox
  motor
  motor_gearbox
  power_switch
  ramp
  region_event_box
  vehicle
  vehicle_no_motor
  vehicle_weight_rear
  vrc_driving_terrain
  vrc_driving_terrain_v2
  wheel_ramps
  AA_battery
)

INSTALL(DIRECTORY ${dirs} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/models)
