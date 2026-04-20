import rclpy
from rclpy.node import Node

from sensor_msgs.msg import Image, CameraInfo
from cv_bridge import CvBridge
from sdc_msgs.msg import CoordinateList
from yolo_msgs.msg import DetectionArray

import numpy as np
import cv2


class DebugOverlay(Node):

    def __init__(self):
        super().__init__('debug_overlay')

        self.bridge = CvBridge()

        # ---------- INTRINSICS ----------
        self.fx = 457.4294128417969
        self.fy = 457.38433837890625
        self.cx = 392.4784851074219
        self.cy = 247.3738250732422

        # ---------- EXTRINSICS ----------
        self.T_lidar_to_cam = np.array([
            [0.0185,  0.1362,  0.9905, -0.015539170612323691],
            [-0.9998, 0.0056,  0.0179, -0.010189313850158001],
            [-0.0031, -0.9907, 0.1363, -0.03247633076479044],
            [0.0,     0.0,     0.0,     1.0],
        ], dtype=np.float32)

        # ---------- STORAGE ----------
        self.image = None
        self.detections = None
        self.lidar_clusters = None
        self.fused_cones = None

        # ---------- SUBSCRIBERS ----------
        self.create_subscription(Image, '/oak/rgb/image_raw', self.image_callback, 10)
        self.create_subscription(CameraInfo, '/oak/rgb/camera_info', self.camera_info_callback, 10)
        self.create_subscription(DetectionArray, '/yolo/detections', self.detection_callback, 10)
        self.create_subscription(CoordinateList, '/Clusters', self.lidar_callback, 10)
        self.create_subscription(CoordinateList, '/fused_cones', self.fused_callback, 10)

        self.get_logger().info('Debug Overlay Node Started')

    # ---------------- CALLBACKS ----------------
    def image_callback(self, msg):
        self.image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        self.process()

    def camera_info_callback(self, msg):
        self.fx = msg.k[0]
        self.fy = msg.k[4]
        self.cx = msg.k[2]
        self.cy = msg.k[5]

    def detection_callback(self, msg):
        self.detections = msg
        self.process()

    def lidar_callback(self, msg):
        self.lidar_clusters = msg
        self.process()

    def fused_callback(self, msg):
        self.fused_cones = msg
        self.process()

    # ---------------- MAIN ----------------
    def process(self):
        if self.image is None:
            return

        frame = self.image.copy()

        # 1. Draw YOLO boxes
        if self.detections is not None:
            self.draw_boxes(frame)

        # 2. Draw raw projected LiDAR cluster points
        if self.lidar_clusters is not None:
            for cone in self.lidar_clusters.cone_coordinates:
                uv = self.project_lidar_point((cone.x, cone.y, cone.z))
                if uv is None:
                    continue

                u, v = uv
                if 0 <= u < frame.shape[1] and 0 <= v < frame.shape[0]:
                    # small white dots = all candidate lidar cluster points
                    cv2.circle(frame, (int(u), int(v)), 4, (255, 255, 255), -1)

        # 3. Draw fused cones more prominently
        if self.fused_cones is not None:
            for cone in self.fused_cones.cone_coordinates:
                uv = self.project_lidar_point((cone.x, cone.y, cone.z))
                if uv is None:
                    continue

                u, v = uv
                if 0 <= u < frame.shape[1] and 0 <= v < frame.shape[0]:
                    color = self.color_from_cone(cone.colour)

                    # bigger circle for fused cone
                    cv2.circle(frame, (int(u), int(v)), 8, color, 2)
                    cv2.circle(frame, (int(u), int(v)), 2, color, -1)

                    label = self.label_from_colour(cone.colour)
                    cv2.putText(
                        frame,
                        label,
                        (int(u) + 8, int(v) - 8),
                        cv2.FONT_HERSHEY_SIMPLEX,
                        0.5,
                        color,
                        1
                    )

        cv2.imshow('Debug Overlay', frame)
        cv2.waitKey(1)

    # ---------------- DRAW BOXES ----------------
    def draw_boxes(self, frame):
        for det in self.detections.detections:
            box_cx = det.bbox.center.position.x
            box_cy = det.bbox.center.position.y
            w = det.bbox.size.x
            h = det.bbox.size.y

            if w <= 0 or h <= 0:
                continue

            xmin = int(box_cx - w / 2.0)
            xmax = int(box_cx + w / 2.0)
            ymin = int(box_cy - h / 2.0)
            ymax = int(box_cy + h / 2.0)

            label = det.class_name
            color = self.color_from_label(label)

            cv2.rectangle(frame, (xmin, ymin), (xmax, ymax), color, 2)
            cv2.putText(
                frame,
                label,
                (xmin, max(0, ymin - 5)),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.5,
                color,
                1
            )

    # ---------------- GEOMETRY ----------------
    def project_lidar_point(self, p_l):
        p_h = np.array([p_l[0], p_l[1], p_l[2], 1.0], dtype=np.float32)

        # Match working fusion node
        T_inv = np.linalg.inv(self.T_lidar_to_cam)
        p_c = T_inv @ p_h
        x_c, y_c, z_c = p_c[:3]

        # Same empirical correction as fusion node
        rot_y_deg = 6.0
        rot_y = np.deg2rad(rot_y_deg)

        R_y = np.array([
            [ np.cos(rot_y), 0.0, np.sin(rot_y)],
            [ 0.0,           1.0, 0.0          ],
            [-np.sin(rot_y), 0.0, np.cos(rot_y)]
        ], dtype=np.float32)

        vec = np.array([x_c, y_c, z_c], dtype=np.float32)
        vec = R_y @ vec
        x_c, y_c, z_c = vec

        # Same empirical translation correction as fusion node
        x_c -= 0.20
        y_c -= 0.25

        if z_c <= 0:
            return None

        u = self.fx * (x_c / z_c) + self.cx
        v = self.fy * (y_c / z_c) + self.cy
        return (u, v)

    # ---------------- HELPERS ----------------
    def color_from_label(self, label):
        if label in ['yellow', 'yellow_cone']:
            return (0, 255, 255)   # yellow in BGR
        elif label in ['blue', 'blue_cone']:
            return (255, 0, 0)     # blue
        elif label in ['orange', 'orange_cone']:
            return (0, 165, 255)   # orange
        return (0, 0, 255)         # red fallback

    def color_from_cone(self, colour):
        if abs(colour - 0.0) < 1e-3:
            return (255, 0, 0)       # blue
        elif abs(colour - 1.0) < 1e-3:
            return (0, 255, 255)     # yellow
        elif abs(colour - 2.0) < 1e-3 or abs(colour - 3.0) < 1e-3:
            return (0, 165, 255)     # orange
        return (0, 0, 255)           # unknown / red

    def label_from_colour(self, colour):
        if abs(colour - 0.0) < 1e-3:
            return 'blue_fused'
        elif abs(colour - 1.0) < 1e-3:
            return 'yellow_fused'
        elif abs(colour - 2.0) < 1e-3:
            return 'orange_fused'
        return 'unknown_fused'


def main(args=None):
    rclpy.init(args=args)
    node = DebugOverlay()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()