rosbuild_add_executable(stereo_node StereoNode.cpp)
target_link_libraries(stereo_node prosilica)
rosbuild_link_boost(stereo_node thread)
