cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(njunaoModule)

find_package(qibuild)
set(_srcs
  main.cpp
  main.hpp
  njunaoModule.cpp
  njunaoModule.hpp
  Detect.cpp
  Detect.hpp
  pole.cpp
  pole.hpp
)
#qi_create_bin(njunaoModule ${_srcs})
qi_create_lib(njunaoModule SHARED ${_srcs} SUBFOLDER naoqi)

qi_use_lib(njunaoModule ALCOMMON ALPROXIES ALVISION OPENCV2_CORE OPENCV2_HIGHGUI OPENCV2_IMGPROC)

