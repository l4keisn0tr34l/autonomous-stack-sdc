cmake_minimum_required(VERSION 3.16)
project(clustering)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(pcl_ros REQUIRED)
find_package(PCL REQUIRED)
find_package(sdc_msgs REQUIRED)

include_directories(
  include
  ${PCL_INCLUDE_DIRS}
)

# 1. Define both executables with unique target names
add_executable(boundaryscan src/boundaryscan.cpp)

# 2. Add dependencies for both targets
ament_target_dependencies(boundaryscan
  rclcpp
  sensor_msgs
  pcl_conversions
  pcl_ros
  PCL
  sdc_msgs
)


# 3. Link libraries for both targets
target_link_libraries(boundaryscan
  ${PCL_LIBRARIES}
)


# 4. Install both targets (you can list them together!)
install(TARGETS 
  boundaryscan 
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()