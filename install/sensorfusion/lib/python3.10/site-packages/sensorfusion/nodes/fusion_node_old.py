# import rclpy
# from rclpy.node import Node

# from sensor_msgs.msg import CameraInfo
# from yolo_msgs.msg import DetectionArray
# from sdc_msgs.msg import CoordinateList, Coordinate

# import numpy as np


# class FusionNode(Node):

#     def __init__(self):
#         super().__init__('fusion_node')

#         # ---------- INTRINSICS ----------
#         self.fx = 457.4294
#         self.fy = 457.3843
#         self.cx = 392.4785
#         self.cy = 247.3738

#         # ---------- EXTRINSICS ----------
#         self.T_lidar_to_cam = np.array([
#             [0.0185, 0.1362, 0.9905, -0.0155391706],
#             [-0.9998, 0.0056, 0.0179, -0.0101893138],
#             [-0.0031, -0.9907, 0.1363, -0.0324763307],
#             [0.0, 0.0, 0.0, 1.0],
#         ], dtype=np.float32)

#         self.detections = None
#         self.lidar_clusters = None

#         self.create_subscription(CameraInfo, '/oak/rgb/camera_info', self.camera_info_callback, 10)
#         self.create_subscription(DetectionArray, '/yolo/detections', self.detection_callback, 10)
#         self.create_subscription(CoordinateList, '/Clusters', self.lidar_callback, 10)

#         self.fused_pub = self.create_publisher(CoordinateList, '/fused_cones', 10)

#         self.get_logger().info("Fusion Node Started")

#     # ---------------- CALLBACKS ----------------
#     def camera_info_callback(self, msg):
#         self.fx = msg.k[0]
#         self.fy = msg.k[4]
#         self.cx = msg.k[2]
#         self.cy = msg.k[5]

#     def detection_callback(self, msg):
#         self.detections = msg
#         self.try_fuse()

#     def lidar_callback(self, msg):
#         self.lidar_clusters = msg
#         self.try_fuse()

#     # ---------------- MAIN ----------------
#     def try_fuse(self):
#         if self.detections is None or self.lidar_clusters is None:
#             return

#         fused = []

#         lidar_points = [(c.x, c.y, c.z) for c in self.lidar_clusters.cone_coordinates]
#         lidar_points.sort(key=lambda p: p[0]**2 + p[1]**2)

#         used_boxes = set()

#         self.get_logger().info(f"\n===== NEW FRAME =====")
#         self.get_logger().info(f"Detections: {len(self.detections.detections)}")
#         self.get_logger().info(f"LiDAR clusters: {len(lidar_points)}")

#         # Print all boxes once
#         for i, det in enumerate(self.detections.detections):
#             xmin, ymin, xmax, ymax = self.bbox_to_corners(det)
#             self.get_logger().info(f"[BBOX {i}] {xmin:.1f}, {xmax:.1f}, {ymin:.1f}, {ymax:.1f}")

#         for idx, p_l in enumerate(lidar_points):

#             self.get_logger().info(f"\n[LIDAR {idx}] raw: {p_l}")
#             if idx in used_clusters:
#                 continue
#             uv = self.project_lidar_point(p_l)
#             if uv is None:
#                 self.get_logger().info("[SKIP] Behind camera")
#                 continue

#             u, v = uv
#             self.get_logger().info(f"[PROJ] u={u:.1f}, v={v:.1f}")

#             matched = False

#             for i, det in enumerate(self.detections.detections):
#                 if i in used_boxes:
#                     continue

#                 xmin, ymin, xmax, ymax = self.bbox_to_corners(det)

#                 bbox_u = (xmin + xmax) / 2.0
#                 bbox_v = (ymin + ymax) / 2.0
#                 du = u - bbox_u
#                 dv = v - bbox_v

#                 self.get_logger().info(f"[DELTA vs BBOX {i}] du={du:.1f}, dv={dv:.1f}") # du, dv values for debugging projected points bcs of transforms

#                 if xmin <= u <= xmax and ymin <= v <= ymax:
#                     self.get_logger().info(f"[MATCH] with box {i}")

#                     used_boxes.add(i)
#                     fused.append({
#                         "x": p_l[0],
#                         "y": p_l[1],
#                         "z": p_l[2],
#                         "label": det.class_name
#                     })
#                     matched = True
#                     break

#             if not matched:
#                 self.get_logger().info("[MISS] No bbox contains this point")

#         self.get_logger().info(f"\n[FUSED COUNT] {len(fused)}")

#         self.publish_fused_cones(fused)

#     # ---------------- GEOMETRY ----------------
#     def project_lidar_point(self, p_l):
#         p_h = np.array([p_l[0], p_l[1], p_l[2], 1.0], dtype=np.float32)

#     # Base transform
#         T_inv = np.linalg.inv(self.T_lidar_to_cam)
#         p_c = T_inv @ p_h
#         x_c, y_c, z_c = p_c[:3]

#     # ----- TEMP EMPIRICAL ROTATION CORRECTION -----
#     # This is a Y-axis rotation (keep because it gave the closest behavior so far)
#         rot_y_deg = 6.0
#         rot_y = np.deg2rad(rot_y_deg)

#         R_y = np.array([
#             [ np.cos(rot_y), 0.0, np.sin(rot_y)],
#             [ 0.0,           1.0, 0.0          ],
#             [-np.sin(rot_y), 0.0, np.cos(rot_y)]
#             ], dtype=np.float32)

#         vec = np.array([x_c, y_c, z_c], dtype=np.float32)
#         vec = R_y @ vec
#         x_c, y_c, z_c = vec

#     # ----- TEMP EMPIRICAL TRANSLATION CORRECTION -----
#     # Move projected points left and up
#         x_c -= 0.15 # (was 0.20, adjusted for better performance. try calibrating this if bad results)
#         y_c -= 0.14

#         self.get_logger().info(f"[CAM] x={x_c:.3f}, y={y_c:.3f}, z={z_c:.3f}")

#         if z_c <= 0:
#             return None

#         u = self.fx * (x_c / z_c) + self.cx
#         v = self.fy * (y_c / z_c) + self.cy

#         return u, v

#     def bbox_to_corners(self, det):
#         cx = det.bbox.center.position.x
#         cy = det.bbox.center.position.y
#         w = det.bbox.size.x
#         h = det.bbox.size.y

#         return cx - w/2, cy - h/2, cx + w/2, cy + h/2

#     # ---------------- PUBLISH ----------------
#     def publish_fused_cones(self, fused):
#         msg = CoordinateList()
#         msg.header.stamp = self.get_clock().now().to_msg()
#         msg.header.frame_id = "rslidar"

#         for cone in fused:
#             c = Coordinate()
#             c.x = float(cone["x"])
#             c.y = float(cone["y"])
#             c.z = float(cone["z"])

#             if cone["label"] == "blue_cone":
#                 c.colour = 0.0
#             elif cone["label"] == "yellow_cone":
#                 c.colour = 1.0
#             else:
#                 c.colour = -1.0

#             msg.cone_coordinates.append(c)

#         self.fused_pub.publish(msg)


# def main(args=None):
#     rclpy.init(args=args)
#     node = FusionNode()
#     rclpy.spin(node)
#     node.destroy_node()
#     rclpy.shutdown()

import rclpy
from rclpy.node import Node

from sensor_msgs.msg import CameraInfo
from yolo_msgs.msg import DetectionArray
from sdc_msgs.msg import CoordinateList, Coordinate

import numpy as np


class FusionNode(Node):

    def __init__(self):
        super().__init__('fusion_node')

        # ---------- INTRINSICS ----------
        self.fx = 457.4294
        self.fy = 457.3843
        self.cx = 392.4785
        self.cy = 247.3738

        # ---------- EXTRINSICS ----------
        self.T_lidar_to_cam = np.array([
            [0.0185, 0.1362, 0.9905, -0.0155391706],
            [-0.9998, 0.0056, 0.0179, -0.0101893138],
            [-0.0031, -0.9907, 0.1363, -0.0324763307],
            [0.0, 0.0, 0.0, 1.0],
        ], dtype=np.float32)

        self.detections = None
        self.lidar_clusters = None

        self.create_subscription(CameraInfo, '/oak/rgb/camera_info', self.camera_info_callback, 10)
        self.create_subscription(DetectionArray, '/yolo/detections', self.detection_callback, 10)
        self.create_subscription(CoordinateList, '/Clusters', self.lidar_callback, 10)

        self.fused_pub = self.create_publisher(CoordinateList, '/fused_cones', 10)

        self.get_logger().info("Fusion Node Started")

    # ---------------- CALLBACKS ----------------
    def camera_info_callback(self, msg):
        self.fx = msg.k[0]
        self.fy = msg.k[4]
        self.cx = msg.k[2]
        self.cy = msg.k[5]

    def detection_callback(self, msg):
        self.detections = msg
        self.try_fuse()

    def lidar_callback(self, msg):
        self.lidar_clusters = msg
        self.try_fuse()

    # ---------------- MAIN ----------------
    def try_fuse(self):
        if self.detections is None or self.lidar_clusters is None:
            return

        # FIX ③ — staleness guard: skip if detection and LiDAR frames are too far apart
        lidar_stamp = self.lidar_clusters.header.stamp
        det_stamp   = self.detections.header.stamp
        dt = abs(
            (lidar_stamp.sec - det_stamp.sec) +
            (lidar_stamp.nanosec - det_stamp.nanosec) * 1e-9
        )
        if dt > 0.1:
            self.get_logger().warn(f"[STALE] Frame pair too old: dt={dt:.3f}s — skipping fusion")
            return

        fused = []

        lidar_points = [(c.x, c.y, c.z) for c in self.lidar_clusters.cone_coordinates]
        lidar_points.sort(key=lambda p: p[0]**2 + p[1]**2)

        # FIX ② — track used clusters (not boxes) so each cluster is consumed once
        used_clusters = set()

        self.get_logger().info(f"\n===== NEW FRAME =====")
        self.get_logger().info(f"Detections: {len(self.detections.detections)}")
        self.get_logger().info(f"LiDAR clusters: {len(lidar_points)}")

        # FIX ② — outer loop over detections, inner loop over clusters
        # Rationale: detections are the confirmed ground truth (camera saw a cone).
        #            We want the best LiDAR cluster that explains each detection.
        #            The old order (outer=cluster, inner=bbox) let stray clusters
        #            greedily consume a bbox before the real cone cluster reached it.
        for i, det in enumerate(self.detections.detections):
            xmin, ymin, xmax, ymax = self.bbox_to_corners(det)
            self.get_logger().info(f"\n[BBOX {i}] cls={det.class_name} "
                                   f"x=[{xmin:.1f},{xmax:.1f}] y=[{ymin:.1f},{ymax:.1f}]")

            best_cluster_idx = None
            best_dist        = float('inf')

            for idx, p_l in enumerate(lidar_points):
                if idx in used_clusters:
                    continue

                uv = self.project_lidar_point(p_l)
                if uv is None:
                    continue

                u, v = uv
                bbox_u = (xmin + xmax) / 2.0
                bbox_v = (ymin + ymax) / 2.0
                du = u - bbox_u
                dv = v - bbox_v
                self.get_logger().info(f"  [CLUSTER {idx}] proj u={u:.1f} v={v:.1f} "
                                       f"du={du:.1f} dv={dv:.1f}")

                if xmin <= u <= xmax and ymin <= v <= ymax:
                    dist = p_l[0]**2 + p_l[1]**2
                    if dist < best_dist:
                        best_dist        = dist
                        best_cluster_idx = idx

            if best_cluster_idx is not None:
                used_clusters.add(best_cluster_idx)
                p_l = lidar_points[best_cluster_idx]
                self.get_logger().info(f"[MATCH] BBOX {i} <-> CLUSTER {best_cluster_idx} "
                                       f"dist={best_dist**0.5:.2f}m")
                fused.append({
                    "x":     p_l[0],
                    "y":     p_l[1],
                    "z":     p_l[2],
                    "label": det.class_name
                })
            else:
                self.get_logger().info(f"[MISS] No cluster projects into BBOX {i}")

        self.get_logger().info(f"\n[FUSED COUNT] {len(fused)}")
        self.publish_fused_cones(fused)

    # ---------------- GEOMETRY ----------------
    def project_lidar_point(self, p_l):
        p_h = np.array([p_l[0], p_l[1], p_l[2], 1.0], dtype=np.float32)

        # Base transform
        T_inv = np.linalg.inv(self.T_lidar_to_cam)
        p_c = T_inv @ p_h
        x_c, y_c, z_c = p_c[:3]

        # ----- TEMP EMPIRICAL ROTATION CORRECTION -----
        # Y-axis rotation (keep because it gave the closest behavior so far)
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

        # ----- TEMP EMPIRICAL TRANSLATION CORRECTION -----
        x_c -= 0.15  # (was 0.20, adjusted for better performance)
        y_c -= 0.14

        self.get_logger().info(f"[CAM] x={x_c:.3f}, y={y_c:.3f}, z={z_c:.3f}")

        if z_c <= 0:
            return None

        u = self.fx * (x_c / z_c) + self.cx
        v = self.fy * (y_c / z_c) + self.cy

        return u, v

    def bbox_to_corners(self, det):
        cx = det.bbox.center.position.x
        cy = det.bbox.center.position.y
        w  = det.bbox.size.x
        h  = det.bbox.size.y

        return cx - w/2, cy - h/2, cx + w/2, cy + h/2

    # ---------------- PUBLISH ----------------
    def publish_fused_cones(self, fused):
        msg = CoordinateList()
        msg.header.stamp    = self.get_clock().now().to_msg()
        msg.header.frame_id = "rslidar"

        for cone in fused:
            c = Coordinate()
            c.x = float(cone["x"])
            c.y = float(cone["y"])
            c.z = float(cone["z"])

            if cone["label"] == "blue_cone":
                c.colour = 0.0
            elif cone["label"] == "yellow_cone":
                c.colour = 1.0
            else:
                c.colour = -1.0

            msg.cone_coordinates.append(c)

        self.fused_pub.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = FusionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()