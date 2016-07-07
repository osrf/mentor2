set (files
  chapter1.world
  chapter2.world
  chapter3.world
  chapter4.world
  chapter5.world
  tow.world
)

install(FILES ${files} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/worlds/)
