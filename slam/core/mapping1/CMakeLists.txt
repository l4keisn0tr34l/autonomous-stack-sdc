cmake_minimum_required(VERSION 3.8)
project(mapping1)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(message_filters REQUIRED)
find_package(sdc_msgs REQUIRED)
find_package(eufs_msgs REQUIRED)

# Executables


add_executable(coloredMapping src/coloredMapping.cpp)
ament_target_dependencies(coloredMapping
  rclcpp
  nav_msgs
  visualization_msgs
  tf2
  tf2_ros
  message_filters
  sdc_msgs
  eufs_msgs
)

# --- NEW EXECUTABLE ADDED HERE ---
add_executable(colorMapping src/colorMapping.cpp)
ament_target_dependencies(colorMapping
  rclcpp
  nav_msgs
  visualization_msgs
  tf2
  tf2_ros
  message_filters
  sdc_msgs
  eufs_msgs
)
# ---------------------------------

# Install

install(TARGETS
  coloredMapping
  DESTINATION lib/${PROJECT_NAME}
)

# --- NEW INSTALL RULE ADDED HERE ---
install(TARGETS
  colorMapping
  DESTINATION lib/${PROJECT_NAME}
)
# -----------------------------------

install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
