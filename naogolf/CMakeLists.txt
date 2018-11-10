cmake_minimum_required(VERSION 2.8)
project(naogolf)
find_package(qibuild)
# main interface, exposed to naoqi
set(_src ${_src} main_interface.cpp)
# vision part
set(_src ${_src} vision.cpp)
# decision part
set(_src ${_src} decision.cpp)
# util
set(_src ${_src} util.cpp image_util.cpp)
# parameter
set(_src ${_src} parameter.cpp)
# main head
set(_src ${_src} naogolf.h)
message(STATUS "source files include ${_src}")
qi_create_lib(naogolf SHARED ${_src})
qi_use_lib(naogolf ALCOMMON ALPROXIES ALVISION OPENCV2_CORE OPENCV2_HIGHGUI)
