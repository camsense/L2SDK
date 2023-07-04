# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("camsense_sdk")

add_definitions(-std=c++11)

#设置执行程序输出路径
SET(EXECUTABLE_OUTPUT_PATH ${camsense_sdk_SOURCE_DIR}/bin/lib CACHE PATH "Build directory" FORCE)
#设置lib输出路径
SET(LIBRARY_OUTPUT_PATH ${camsense_sdk_SOURCE_DIR}/bin/lib CACHE PATH "Build directory" FORCE)

# 包含子项目。
add_subdirectory ("L2_SDK_Demo")

add_subdirectory("l2_sdk")
