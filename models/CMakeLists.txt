set (dirs
  battery_holder
  power_switch
  AA_battery
  wheel_ramps
  bucket_wheel
)

INSTALL(DIRECTORY ${dirs} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/models)
