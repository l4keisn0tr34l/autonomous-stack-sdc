#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2
from rclpy.qos import qos_profile_sensor_data
import sensor_msgs_py.point_cloud2 as pc2 # Use the standard library

class JaneRemover(Node):
    def __init__(self):
        super().__init__("simple_ground_remover")
        
        self.subscriber = self.create_subscription(
            PointCloud2, 
            "/velodyne_points", 
            self.listener_callback, 
            qos_profile_sensor_data
        )
            
        self.publisher = self.create_publisher(
            PointCloud2, 
            "/noground", 
            10)
        
        self.min_height = -0.7 
        self.get_logger().info("Simple ground remover started (Standard Mode)")

    def listener_callback(self, msg):
        # 1. Read points using standard library (safer than numpify)
        # 'skip_nans=True' filters out errors automatically
        gen = pc2.read_points(msg, field_names=("x", "y", "z"), skip_nans=True)
        
        clean_points = []
        
        # 2. Iterate and filter
        for p in gen:
            x, y, z = p
            if z > self.min_height:
                clean_points.append([x, y, z])

        # 3. Publish
        out_msg = pc2.create_cloud_xyz32(msg.header, clean_points)
        self.publisher.publish(out_msg)

def main(args=None):
    rclpy.init(args=args)
    node = JaneRemover()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()