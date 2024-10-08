import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PythonExpression

from launch_ros.actions import Node
from nav2_common.launch import RewrittenYaml
import launch_ros.actions

import yaml

def generate_launch_description():
    # Get the launch directory
    multibot2_server_dir = get_package_share_directory('multibot2_server')

    # Launch argument setting
    lifecycle_nodes = ['map_server']
    use_sim_time = False
    autostart = True

    # rviz
    rviz_config_dir = os.path.join(
        multibot2_server_dir,
        'rviz',
        'multibot2_server.rviz'
    )

    start_rviz_cmd = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', rviz_config_dir],
        parameters=[{'use_sim_time': use_sim_time}],
        output='screen'
    )

    # Map server
    map_server_param_path = os.path.join(
        multibot2_server_dir,
        'params',
        'map_server_params.yaml'
    )

    with open(map_server_param_path) as map_server_params:
        map_server_params = yaml.load(map_server_params, Loader=yaml.Loader)
        map = map_server_params['map_server']['ros__parameters']['map']
        yaml_filename = os.path.join(
            multibot2_server_dir, 'maps', map, 'map.yaml')

        with open(yaml_filename) as map_params:
            map_params = yaml.load(map_params, Loader=yaml.Loader)
            map_origin = map_params['origin']

    configured_params = RewrittenYaml(
        source_file=map_server_param_path,
        root_key='',
        param_rewrites={
            'yaml_filename': yaml_filename
        },
        convert_types=True
    )

    map_server_cmd = Node(
        package='nav2_map_server',
        executable='map_server',
        output='screen',
        parameters=[configured_params,
                    {'autostart': autostart}],
        remappings=[('/tf', 'tf'),
                    ('/tf_static', 'tf_static')]
    )

    world_map_cmd = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        output='screen',
        arguments=[str(map_origin[0]), str(map_origin[0]), '0', '0', '0', '0', 'world', 'map']
    )

    # lifecycle manager
    start_lifecycle_manager_cmd = launch_ros.actions.Node(
            package='nav2_lifecycle_manager',
            executable='lifecycle_manager',
            name='lifecycle_manager',
            output='screen',
            emulate_tty=True,  # https://github.com/ros2/launch/issues/188
            parameters=[{'use_sim_time': use_sim_time},
                        {'autostart': autostart},
                        {'node_names': lifecycle_nodes}]
    )

    # Server Node
    fake_map_to_base = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        namespace='',
        output='screen',
        arguments=['0','0','0','0','0','0',
                   'map', 'server/base_link']
    )
    
    global_costmap_Config = RewrittenYaml(
        source_file = os.path.join(multibot2_server_dir, 'params', 'global_costmap.yaml'),
        root_key = '',
        param_rewrites={
            'robot_base_frame': 'server/base_link',
            'use_sim_time': str(use_sim_time)
        },
        convert_types=True
    )
    
    multibot2_server_cmd = Node(
        package='multibot2_server',
        executable='server',
        name='server',
        output='screen',
        parameters=[
            global_costmap_Config,
            os.path.join(multibot2_server_dir, 'params', 'server_params.yaml')
        ]
    )

    # Create the launch description and populate
    ld = LaunchDescription()

    # Add any conditioned actions
    ld.add_action(start_rviz_cmd)
    ld.add_action(map_server_cmd)
    ld.add_action(world_map_cmd)
    ld.add_action(start_lifecycle_manager_cmd)

    ld.add_action(fake_map_to_base)
    ld.add_action(multibot2_server_cmd)

    return ld