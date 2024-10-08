################################################################################
# Set minimum required version of cmake, project name and compile options
################################################################################
cmake_minimum_required(VERSION 3.5)
project(multibot2_msgs)

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

################################################################################
# Find dependencies
################################################################################
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED) 
find_package(costmap_converter_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)

set(dependencies
    std_msgs
    geometry_msgs    
    costmap_converter_msgs
    builtin_interfaces
)

################################################################################
# Declare ROS messages, services and actions
################################################################################
set(msg_files
  "msg/DriverStatusStamped.msg"
  "msg/FeedbackMsg.msg"
  "msg/Neighbor.msg"
  "msg/Neighbors.msg"
  "msg/ObstacleArrayMsg.msg"
  "msg/RobotConfig.msg"
  "msg/RobotFootprint.msg"
  "msg/RobotWithTrajectory.msg"
  "msg/RobotWithTrajectoryArray.msg"
  "msg/RobotState.msg"
  "msg/Task.msg"
  "msg/TrajectoryMsg.msg"
  "msg/TrajectoryPointMsg.msg"
  "msg/TrajectoryPointSE2.msg"
)

set(srv_files
  "srv/Connection.srv"
  "srv/Disconnection.srv"
  "srv/DriverCommand.srv"
  "srv/ModeSelection.srv"
  "srv/QueueRivision.srv"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${srv_files}
  DEPENDENCIES ${dependencies}
)

################################################################################
# Macro for ament package
################################################################################
ament_export_dependencies(rosidl_default_runtime)
ament_package()