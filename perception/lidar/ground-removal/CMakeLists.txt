cmake_minimum_required(VERSION 3.8)
project(ground_removal)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(pcl_ros REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(PCL REQUIRED)
find_package(sdc_msgs REQUIRED)

# TBB (required for your node)
find_package(TBB REQUIRED)

# Include PCL
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


add_executable(lu src/lu.cpp)

ament_target_dependencies(lu
  rclcpp
  sensor_msgs
  pcl_conversions
  pcl_ros
  visualization_msgs
  geometry_msgs
  std_msgs
  sdc_msgs
)

add_executable(simpleZ src/simpleZ.cpp)


ament_target_dependencies(simpleZ
  rclcpp
  sensor_msgs
  pcl_conversions
  pcl_ros
  std_msgs
)



# Link with PCL and TBB (important!)
target_link_libraries(lu
  ${PCL_LIBRARIES}
  TBB::tbb
)

target_link_libraries(simpleZ
  ${PCL_LIBRARIES}
  TBB::tbb
)


# Install both executables
install(TARGETS
  lu
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  simpleZ
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

install(PROGRAMS
  scripts/janeremover.py
  DESTINATION lib/${PROJECT_NAME}
)
install(PROGRAMS
  scripts/janeremoverhuh.py
  DESTINATION lib/${PROJECT_NAME}
)


# Linting
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
