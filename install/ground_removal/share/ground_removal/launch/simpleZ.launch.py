from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Path to YAML parameter file
    param_file = os.path.join(
        get_package_share_directory('ground_removal'),
        'config',
        'simpleZ_sim_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='ground_removal',
            executable='simpleZ',
            name='simpleZ',
            output='screen',
            parameters=[param_file]
        )
    ])