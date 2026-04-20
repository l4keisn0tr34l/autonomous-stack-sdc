from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    param_file = os.path.join(
        get_package_share_directory('odometry'),
        'config',
        'deadReckoning_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='odometry',
            executable='deadReckoning',
            name='deadReckoning',
            output='screen',
            parameters=[param_file]
        )
    ])