cmake_minimum_required(VERSION 2.8)
project(naogolf)
find_package(qibuild)
add_definitions(-DBOOST_SIGNALS_NO_DEPRECATION_WARNING)
# main interface, exposed to naoqi
set(_src ${_src} main_interface.cpp)
# vision part
set(_src ${_src} vision.cpp constraint_limit.cpp)
# decision part
set(_src ${_src} decision.cpp fsm.cpp)
# control part
set(_src ${_src} control.cpp)
# util
set(_src ${_src} util.cpp image_util.cpp)
# parameter
set(_src ${_src} parameter.cpp)
# algorithm base
set(_src ${_src} algorithm_base.cpp)
# main head
set(_src ${_src} naogolf.h detection_base.h decision_base.h)
message(STATUS "source files include ${_src}")
qi_create_lib(naogolf SHARED ${_src})
qi_use_lib(naogolf ALCOMMON ALPROXIES ALVISION OPENCV2_CORE OPENCV2_HIGHGUI)
