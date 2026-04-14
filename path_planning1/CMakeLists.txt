cmake_minimum_required(VERSION 3.8)
project(path_planning1)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(message_filters REQUIRED)
find_package(sdc_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(eufs_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(midPoint src/midPoint.cpp)
ament_target_dependencies(midPoint
  rclcpp
  nav_msgs
  tf2
  tf2_ros
  message_filters
  sdc_msgs
  eufs_msgs
)

add_executable(midPointColored src/midPointColored.cpp)
ament_target_dependencies(midPointColored
  rclcpp
  nav_msgs
  tf2
  tf2_ros
  message_filters
  sdc_msgs
)

add_executable(midPointErrorPropogator src/midPointErrorPropogator.cpp)
ament_target_dependencies(midPointErrorPropogator
  rclcpp
  nav_msgs
  tf2
  tf2_ros
  message_filters
  sdc_msgs
  std_msgs
  visualization_msgs
)

install(TARGETS
  midPoint
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  midPointColored
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  midPointErrorPropogator
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}/
)

add_executable(coneLineViz src/coneLineViz.cpp)
ament_target_dependencies(coneLineViz
  rclcpp
  visualization_msgs
  geometry_msgs
  sdc_msgs
)
install(TARGETS
  coneLineViz
  DESTINATION lib/${PROJECT_NAME}
)

install(PROGRAMS
  scripts/midPointOriginal.py
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
