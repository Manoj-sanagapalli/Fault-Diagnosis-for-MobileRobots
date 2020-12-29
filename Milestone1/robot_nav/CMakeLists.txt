cmake_minimum_required(VERSION 3.5)
project(robot_nav)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(ros2_msg REQUIRED)
find_package(sensor_msgs REQUIRED)


add_executable(navigation_node src/navigation.cpp)
ament_target_dependencies(navigation_node rclcpp std_msgs geometry_msgs nav_msgs)

add_executable(float_array src/float_array.cpp)
ament_target_dependencies(float_array rclcpp std_msgs geometry_msgs nav_msgs)

add_executable(csv_extractor_node src/csv_extractor.cpp)
ament_target_dependencies(csv_extractor_node rclcpp std_msgs geometry_msgs nav_msgs ros2_msg sensor_msgs)

add_executable(validation_node src/validation.cpp)
ament_target_dependencies(validation_node rclcpp std_msgs geometry_msgs nav_msgs ros2_msg sensor_msgs)

add_executable(current_monitor_node src/current_monitor.cpp)
ament_target_dependencies(current_monitor_node rclcpp std_msgs geometry_msgs nav_msgs ros2_msg)

add_executable(vel_publisher src/vel_publisher.cpp)
ament_target_dependencies(vel_publisher rclcpp std_msgs geometry_msgs nav_msgs ros2_msg)

add_executable(fake_odom_node src/fake_odom.cpp)
ament_target_dependencies(fake_odom_node rclcpp std_msgs geometry_msgs nav_msgs tf2_ros)



install(TARGETS
  navigation_node
  float_array
  current_monitor_node
  vel_publisher
  csv_extractor_node
  validation_node
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}
)



ament_package()
