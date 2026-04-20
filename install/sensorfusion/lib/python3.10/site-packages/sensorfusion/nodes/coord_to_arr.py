import rclpy
from rclpy.node import Node

from sdc_msgs.msg import CoordinateList
from sdc_msgs.msg import Arrofarr, Arr  # assuming Arr exists inside msg

class CoordToArrConverter(Node):

    def __init__(self):
        super().__init__('coord_to_arr_converter')

        # ---- SUBSCRIBE: fusion output ----
        self.sub = self.create_subscription(
            CoordinateList,
            '/fused_cones',
            self.callback,
            10
        )

        # ---- PUBLISH: what mapping expects ----
        self.pub = self.create_publisher(
            Arrofarr,
            '/mapping_input',
            10
        )

        self.get_logger().info("CoordinateList → Arrofarr converter started")

    def callback(self, msg):
        out_msg = Arrofarr()

        for cone in msg.cone_coordinates:
            arr = Arr()
            arr.data = [float(cone.x), float(cone.y), float(cone.colour)]
            out_msg.data.append(arr)

        self.pub.publish(out_msg)


def main(args=None):
    rclpy.init(args=args)
    node = CoordToArrConverter()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()