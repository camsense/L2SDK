# CMakeList.txt: L2_SDK_Demo 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

INCLUDE_DIRECTORIES(${CAMSENSE_SOURCE_DIR}/l2_sdk)
LINK_DIRECTORIES(${CAMSENSE_SOURCE_DIR}/l2_sdk)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/l2_sdk)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

#opencv
# INCLUDE_DIRECTORIES(include()
#     ${Boost_INCLUDE_DIRS}
#     ${catkin_INCLUDE_DIRS}
#     /usr/local/include/opencv4
#     )
# file(GLOB OpenCV_LIBS "/usr/local/lib/*.so")


# 将源代码添加到此项目的可执行文件。
add_executable (L2_SDK_Demo "L2_SDK_Demo.cpp" "L2_SDK_Demo.h")
TARGET_LINK_LIBRARIES (L2_SDK_Demo l2_sdk)
#target_link_libraries(L2_SDK_Demo ${OpenCV_LIBS})

#TARGET_LINK_LIBRARIES(l2_sdk.lib)

# TODO: 如有需要，请添加测试并安装目标。
