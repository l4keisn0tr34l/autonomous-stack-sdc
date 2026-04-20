from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    param_file = os.path.join(
        get_package_share_directory('path_planning1'),
        'config',
        'midPointColored_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='path_planning1',
            executable='midPointColored',
            name='midPointColored',
            output='screen',
            parameters=[param_file]
        )
    ])