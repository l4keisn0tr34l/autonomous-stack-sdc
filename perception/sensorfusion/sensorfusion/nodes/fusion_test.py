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
        # These are placeholder values from camera_info.
        # They get overwritten once /oak/rgb/camera_info starts publishing.
        self.fx = 457.4294
        self.fy = 457.3843
        self.cx = 392.4785
        self.cy = 247.3738

        # ---------- EXTRINSICS ----------
        # This is the calibrated transform matrix you got.
        # We empirically found that using its inverse gives the correct direction
        # for LiDAR point -> camera-frame projection in this stack.
        self.T_lidar_to_cam = np.array([
            [0.0185,  0.1362,  0.9905, -0.015539170612323691],
            [-0.9998, 0.0056,  0.0179, -0.010189313850158001],
            [-0.0031, -0.9907, 0.1363, -0.03247633076479044],
            [0.0,     0.0,     0.0,     1.0],
        ], dtype=np.float32)

        # ---------- STORAGE ----------
        # Latest messages are stored here.
        self.detections = None
        self.lidar_clusters = None

        # ---------- SUBSCRIBERS ----------
        self.create_subscription(
            CameraInfo,
            '/oak/rgb/camera_info',
            self.camera_info_callback,
            10
        )
        self.create_subscription(
            DetectionArray,
            '/yolo/detections',
            self.detection_callback,
            10
        )
        self.create_subscription(
            CoordinateList,
            '/Clusters',
            self.lidar_callback,
            10
        )

        # ---------- PUBLISHER ----------
        # Output topic used downstream by converter/mapping/planning.
        self.fused_pub = self.create_publisher(
            CoordinateList,
            '/fused_cones',
            10
        )

        self.get_logger().info("Fusion Node Started (non-destructive version)")

    # ---------------- CALLBACKS ----------------
    def camera_info_callback(self, msg):
        # Overwrite placeholder intrinsics with live values.
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
        # Need both detections and lidar clusters before fusion can happen.
        if self.detections is None or self.lidar_clusters is None:
            return

        output_cones = []

        # Extract all LiDAR cone centers from CoordinateList.
        lidar_points = [(c.x, c.y, c.z) for c in self.lidar_clusters.cone_coordinates]

        # Sort by distance so nearer cones are matched first.
        # This helps prevent dumb box assignment when multiple cones are present.
        lidar_points.sort(key=lambda p: p[0]**2 + p[1]**2)

        # Once a YOLO box is matched, do not reuse it for another LiDAR cone.
        used_boxes = set()

        # ---------- DEBUG FRAME HEADER ----------
        self.get_logger().info("\n===== NEW FRAME =====")
        self.get_logger().info(f"Detections: {len(self.detections.detections)}")
        self.get_logger().info(f"LiDAR clusters: {len(lidar_points)}")

        # Print all box bounds once for debugging.
        for i, det in enumerate(self.detections.detections):
            xmin, ymin, xmax, ymax = self.bbox_to_corners(det)
            self.get_logger().info(
                f"[BBOX {i}] {xmin:.1f}, {xmax:.1f}, {ymin:.1f}, {ymax:.1f}"
            )

        # ---------- FUSION LOOP ----------
        for idx, p_l in enumerate(lidar_points):
            self.get_logger().info(f"\n[LIDAR {idx}] raw: {p_l}")

            uv = self.project_lidar_point(p_l)
            matched_label = None

            # Only attempt matching if projection landed in front of camera.
            if uv is not None:
                u, v = uv
                self.get_logger().info(f"[PROJ] u={u:.1f}, v={v:.1f}")

                for i, det in enumerate(self.detections.detections):
                    if i in used_boxes:
                        continue

                    xmin, ymin, xmax, ymax = self.bbox_to_corners(det)

                    # Compute bbox center and delta for debugging/tuning.
                    bbox_u = (xmin + xmax) / 2.0
                    bbox_v = (ymin + ymax) / 2.0
                    du = u - bbox_u
                    dv = v - bbox_v

                    self.get_logger().info(
                        f"[DELTA vs BBOX {i}] du={du:.1f}, dv={dv:.1f}" # du, dv values for debugging projected points bcs of transforms
                    )

                    # Matching rule:
                    # projected LiDAR point center must fall inside YOLO bbox.
                    if xmin <= u <= xmax and ymin <= v <= ymax:
                        matched_label = det.class_name
                        used_boxes.add(i)
                        self.get_logger().info(
                            f"[MATCH] with box {i} ({matched_label})"
                        )
                        break
            else:
                self.get_logger().info("[SKIP] Behind camera")

            # ---------- NON-DESTRUCTIVE OUTPUT ----------
            # Always publish the LiDAR cone.
            # If matched, attach semantic colour from YOLO.
            # If unmatched, keep the LiDAR geometry and mark colour as unknown.
            output_cones.append({
                "x": p_l[0],
                "y": p_l[1],
                "z": p_l[2],
                "label": matched_label
            })

            if matched_label is None:
                pass
        matched_count = sum(1 for c in output_cones if c["label"] is not None)
        self.get_logger().info(f"\n[MATCHED COUNT] {matched_count}")
        self.get_logger().info(f"[OUTPUT COUNT] {len(output_cones)}")

        self.publish_fused_cones(output_cones)

    # ---------------- GEOMETRY ----------------
    def project_lidar_point(self, p_l):
        # Convert LiDAR point into homogeneous coordinates.
        p_h = np.array([p_l[0], p_l[1], p_l[2], 1.0], dtype=np.float32)

        # Inverted transform was empirically the correct direction
        # for this stack and this calibration.
        T_inv = np.linalg.inv(self.T_lidar_to_cam) 
        p_c = T_inv @ p_h
        x_c, y_c, z_c = p_c[:3]

        # ---------- EMPIRICAL CORRECTION ----------
        # This is the extra correction that gave working matches in practice.
        # It is still a hack, but right now it is the working hack.
        rot_y_deg = 6.0     # This is a Y-axis rotation (keep because it gave the closest behavior so far)  
        rot_y = np.deg2rad(rot_y_deg)

        R_y = np.array([
            [ np.cos(rot_y), 0.0, np.sin(rot_y)],
            [ 0.0,           1.0, 0.0          ],
            [-np.sin(rot_y), 0.0, np.cos(rot_y)]
        ], dtype=np.float32)

        vec = np.array([x_c, y_c, z_c], dtype=np.float32)
        vec = R_y @ vec
        x_c, y_c, z_c = vec

        # Empirical camera-frame translation correction.
        # These values were tuned so projected LiDAR points land in YOLO boxes.

        x_c -= 0.273 # (was 0.20, adjusted for better performance. try calibrating this if bad results)

        y_c -= 0.25

        self.get_logger().info(f"[CAM] x={x_c:.3f}, y={y_c:.3f}, z={z_c:.3f}")

        # Points behind camera cannot be projected meaningfully.
        if z_c <= 0:
            return None

        # Standard pinhole projection.
        u = self.fx * (x_c / z_c) + self.cx
        v = self.fy * (y_c / z_c) + self.cy

        return u, v

    def bbox_to_corners(self, det):
        # Convert YOLO center-width-height box to corner form.
        cx = det.bbox.center.position.x
        cy = det.bbox.center.position.y
        w = det.bbox.size.x
        h = det.bbox.size.y

        return (
            cx - w / 2.0,
            cy - h / 2.0,
            cx + w / 2.0,
            cy + h / 2.0
        )

    # ---------------- PUBLISH ----------------
    def publish_fused_cones(self, cones):
        msg = CoordinateList()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "rslidar"

        for cone in cones:
            c = Coordinate()
            c.x = float(cone["x"])
            c.y = float(cone["y"])
            c.z = float(cone["z"])

            # Use mapping-compatible colour IDs.
            # 0 = blue
            # 1 = yellow
            # 2 = orange
            # 4 = unknown
            if cone["label"] in ["blue", "blue_cone"]:
                c.colour = 0.0
            elif cone["label"] in ["yellow", "yellow_cone"]:
                c.colour = 1.0
            elif cone["label"] in ["orange", "orange_cone"]:
                c.colour = 2.0
            else:
                c.colour = 4.0

            msg.cone_coordinates.append(c)

        self.fused_pub.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = FusionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()