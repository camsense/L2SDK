cmake_minimum_required(VERSION 2.8.3)
project(lcm2ros)

set(BUILD_LOGPLAYER TRUE)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  geometry_msgs
)

###########
## Build ##
###########
#set(Thirdparty_DIR $ENV{HOME}/cleanRobot/thirdparty)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Thirdparty_DIR}/CMakeModules")
find_package(lcm REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -Wno-deprecated-declarations")

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

add_executable(lcm2ros
  src/main.cpp 
  src/RosPublisher.cpp
)
target_link_libraries(lcm2ros
  ${catkin_LIBRARIES}
  lcm
)






