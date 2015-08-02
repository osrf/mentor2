
SOURCE_PATH=${HOME}
INSTALL_PATH=${HOME}/local

echo "=====  Updating SDFormat ====="
cd ${SOURCE_PATH}/sdformat/build/
hg pull 
hg up -C
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH}
make -j 4 install


echo "=====  Updating Gazebo ====="
cd ${SOURCE_PATH}/gazebo/build/
hg pull
hg up -C
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH}
make -j 4 install


echo "=====  Updating MENTOR2 ====="
cd ${SOURCE_PATH}/mentor2/build/
hg pull 
hg up -C
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH}
make -j 4 install


echo "===== Done ====="
$SHELL
