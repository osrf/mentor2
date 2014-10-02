./stop.sh

echo "SOURCE before you buy ~/local/share/simplegdk/setup.sh"

echo "GAZEBO PLUGIN PATH=$GAZEBO_PLUGIN_PATH"
echo "MENTOR2 world [$1]"


gzserver --verbose -s libSimpleMOOCPlugin.so $1 -u &
gzclient --verbose -g libMOOCUIPlugin.so -g libCMLEditorPlugin.so  &

