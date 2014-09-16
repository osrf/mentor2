Gazebo Design Kit (GDK) for MENTOR2
===


Installation
===

    mkdir build
    cd build
    cmake ..
    make install


Dependencies
===

SDFormat >= 2.0.1
Gazebo >= 4.0.1
    branch: expose_mainwindow

Running
===

you should setup the environment by sourcing the setup.sh file. The location of the setup file depends on CMAKE_INSTALLATION_PREFIX.
 
     . /usr/local/share/simplegdk/setup.sh
     
run a Mentor2  world
    run.sh ./test_scoring.world