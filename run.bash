#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

$DIR/stop.sh


./stop.bash

echo "Did you source?:  <your setup path>/share/simplegdk/setup.sh"
echo "Did you source?:  <your setup path>/share/gazebo/setup.sh"

echo "GAZEBO PLUGIN PATH=$GAZEBO_PLUGIN_PATH"
echo "MENTOR2 world [$1]"

#gzserver --verbose  -s libSimpleConnectionsPlugin.so $1 -u &
#gzclient --verbose -g libCMLEditorPlugin.so  &


gzserver --verbose -s libRestWebPlugin.so -u  $1 &
gzclient --verbose -g libRestUiPlugin.so menu="Mentor2" title="Leaderboard" label="Login" url="https://localhost:3000" &

