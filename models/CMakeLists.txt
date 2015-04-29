set (dirs
  axle_wheels
  battery_holder
  battery_pack
  bucket_wheel
  car
  chassis
  motor
  motor_box
  power_switch
  wheel_ramps
  AA_battery
)

INSTALL(DIRECTORY ${dirs} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/models)
