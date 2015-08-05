#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

$DIR/stop.bash


#./stop.bash

echo "Did you source?:  <your setup path>/share/simplegdk/setup.sh"
echo "Did you source?:  <your setup path>/share/gazebo/setup.sh"

# get simplegdk configuration.
if [ -f ${HOME}/local/share/simplegdk/setup.sh ]; then
  . ${HOME}/local/share/simplegdk/setup.sh
elif [ -f /usr/local/share/simplegdk/setup.sh ]; then
  . /usr/local/share/simplegdk/setup.sh
else
  echo "Warning: failed to find simplegdk's setup.sh.  You will need to source it manually."
fi


echo "GAZEBO PLUGIN PATH=$GAZEBO_PLUGIN_PATH"
echo "MENTOR2 world [$1]"

#gzserver --verbose  -s libSimpleConnectionsPlugin.so $1 -u &
#gzclient --verbose -g libCMLEditorPlugin.so  &


echo "GAZEBO PREFIX=$GAZEBO_PREFIX"

gzserver --verbose -s libRestWebPlugin.so -u  $1 &

gzclient --verbose -g libCMLEditorPlugin.so -g libRestUiPlugin.so menu="Mentor2" title="Leaderboard" label="Login" url="http://mentor2-local.srirobotics.com" &


sleep 10
