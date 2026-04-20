from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    param_file = os.path.join(
        get_package_share_directory('mapping'),
        'config',
        'simpleMapping_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='mapping',
            executable='simpleMapping',
            name='simpleMapping',
            output='screen',
            parameters=[param_file]
        )
    ])