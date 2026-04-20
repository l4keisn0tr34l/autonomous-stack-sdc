#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2
import sensor_msgs_py.point_cloud2 as pc2
import ros2_numpy as rnp


class JaneRemover(Node):
    def __init__(self):
        super().__init__("simple_ground_remover")
        self.subscriber = self.create_subscription(
            PointCloud2, "/velodyne_points", self.listener_callback, 10)
        self.publisher = self.create_publisher(
            PointCloud2, "/noground", 10)
        
        self.get_logger().info("Simple ground remover started")
        self.min_height = -0.7

    def listener_callback(self, msg):
        points=rnp.numpify(msg)
        z= points['xyz'][:,2]
        temp = z > self.min_height
        clean_points = {}
        for key in points:
            clean_points[key] = points[key][temp]
        out = pc2.create_cloud_xyz32(msg.header, clean_points['xyz'])
        self.publisher.publish(out)
        
def main(args=None):
    rclpy.init(args=args)
    node = JaneRemover()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
