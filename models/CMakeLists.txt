set (dirs
  battery_holder
  bucket_wheel
  motor
  power_switch
  wheel_ramps
  AA_battery
  backhoe
)

INSTALL(DIRECTORY ${dirs} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/models)
