cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(redballdetect)

find_package(qibuild)
set(_srcs
  main.cpp
  main.hpp
  njunaoModule.cpp
  njunaoModule.hpp
  Detect.cpp
  Detect.hpp
)
#include_directories(".","/usr/local/include")  
#qi_create_bin(redballdetect ${_srcs})
qi_create_lib(redballdetect SHARED ${_srcs} SUBFOLDER naoqi)

qi_use_lib(redballdetect ALCOMMON ALPROXIES ALVISION OPENCV2_CORE OPENCV2_HIGHGUI OPENCV2_IMGPROC)

