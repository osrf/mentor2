set (dirs
  battery_holder
  motor
  power_switch
  AA_battery
  wheel_ramps
)

INSTALL(DIRECTORY ${dirs} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/models)
