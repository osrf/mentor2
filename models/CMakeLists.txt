set (dirs
  AA_battery
  battery_holder
  bucket_wheel
  car
  concrete_slab
  motor
  power_switch
  wheel_ramps
)

INSTALL(DIRECTORY ${dirs} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/models)
