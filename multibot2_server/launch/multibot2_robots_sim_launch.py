import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, RegisterEventHandler, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource


def generate_launch_description():
    
    multibot2_robot_dir = get_package_share_directory("multibot2_robot")
    
    robots = ["robot1", "robot2", "robot3", "robot4"]
    # robots = ["robot1", "robot2", "robot3", "robot4", "robot5", "robot6", "robot7", "robot8"]
    
    multibot2_robot_cmds = []        
    for robot in robots:
        multibot2_robot_launch = robot + '_sim_launch.py'
                
        multibot2_robot_cmds.append(
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(os.path.join(
                    multibot2_robot_dir, 'launch', multibot2_robot_launch)),
            )
        )

    ld = LaunchDescription()
        
    for multibot2_robot_cmd in multibot2_robot_cmds:
        ld.add_action(multibot2_robot_cmd)
    
    return ld