# Autonomous Stack SDC

ROS 2-based autonomous driving stack for a Formula Student / driverless vehicle.

This repository contains the main software pipeline for:
- perception
- sensor fusion
- mapping / SLAM
- path planning
- control

This README is written for the **real-car runtime path**, not just the older sim-only flow.

---

## Current Real-World Runtime Flow

1. OAK-D LR camera driver
2. RS-LiDAR M1P driver
3. Ground removal
4. Clustering
5. YOLO
6. Fusion
7. Debug overlay *(recommended during setup / calibration)*
8. `coord_to_arr`
9. Colored mapping
10. Planning
11. Control

The key real-world perception chain is:

```text
/oak/rgb/image_raw + /oak/rgb/camera_info
                    │
                    └──> YOLO → /yolo/detections

/rslidar_points
    └──> ground removal → clustering → /Clusters (CoordinateList)

 /Clusters + /yolo/detections + /oak/rgb/camera_info
                    └──> fusion → /fused_cones (CoordinateList, non-destructive)

 /fused_cones
    └──> coord_to_arr → /Clusters (Arrofarr for legacy mapping input)

 /Clusters + /odom
    └──> coloredMapping → /map_arr

 /map_arr + /odom
    └──> midPointColored → /cone_pair

 /cone_pair + /odom
    └──> midPointErrorPropogator → /path_error

 /path_error
    └──> control
```

> Important: the fusion node is now **non-destructive**.  
> Every LiDAR cone is published forward. Matched cones get semantic color; unmatched cones are still published as `unknown`.  
> This prevents mapping from dropping valid far cones just because vision failed to match them.

---

## Repository Structure

```text
autonomous-stack-sdc/
├── control/
├── path_planning1/
├── perception/
│   ├── camera/
│   │   └── yolo_ros/
│   ├── lidar/
│   │   ├── clustering/
│   │   └── ground-removal/
│   └── sensorfusion/
├── sdc_msgs/
└── slam/
    ├── core/
    │   ├── mapping1/
    │   └── odometry/
    └── interface/
        └── velocity_transform_sim/
```

---

## Main Packages

### Perception

#### `perception/lidar/ground-removal`
Removes ground points from raw LiDAR clouds.

#### `perception/lidar/clustering`
Clusters non-ground LiDAR points into cone candidates.

#### `perception/camera/yolo_ros`
Runs YOLO-based cone detection on the camera stream.

#### `perception/sensorfusion`
Contains:
- `sensor_fusion_node`
- `debug_overlay`
- `coord_to_arr`

This package:
- projects LiDAR cluster centers into the image plane
- associates projected LiDAR cones with YOLO bounding boxes
- publishes `/fused_cones`
- visualizes boxes + projected cones for debugging
- converts fused output into the legacy array format expected by mapping

---

### Messages

#### `sdc_msgs`
Custom message definitions used across the stack, including:
- `Coordinate`
- `CoordinateList`
- `Arr`
- `Arrofarr`

---

### SLAM / Mapping

#### `slam/core/mapping1`
Builds a colored cone map from fused perception output and odometry.

Current mapping is **2D colored cone mapping**, not full SLAM in the probabilistic sense.

---

### Planning

#### `path_planning1`
Midpoint-based planning stack, including:
- `midPoint`
- `midPointColored`
- `midPointErrorPropogator`
- `coneLineViz`

---

### Control

#### `control`
PID-based downstream controller.

---

## Hardware Driver Requirements

This repository depends on external driver repositories that are **not included** here.

### Camera — Luxonis OAK-D LR

Required external repository:
- `depthai_ros`

Clone:
```bash
git clone https://github.com/luxonis/depthai-ros.git
```

Expected camera topics used by this stack:
```text
/oak/rgb/image_raw
/oak/rgb/camera_info
```

---

### LiDAR — RoboSense RS-LiDAR M1P

Required external repository:
- `rslidar_sdk`

Clone:
```bash
git clone https://github.com/RoboSense-LiDAR/rslidar_sdk.git
```

Expected LiDAR input topic for the perception chain:
```text
/rslidar_points
```

---

### Example Workspace Layout

```text
~/autonomous_ws/src/
├── autonomous-stack-sdc/
├── depthai-ros/
└── rslidar_sdk/
```

---

## Additional Visual / YOLO Requirements

### YOLO package notes
This stack expects the visual detection stage to be available and properly configured.

If you are also using the separate weights / visual setup repository, keep that consistent with the YOLO node configuration.

### NumPy note
Use:

```bash
pip install "numpy<2"
```

This is important because some parts of the older visual / ONNX / ROS tooling are still safer with NumPy 1.x.

---

## Prerequisites

Assumes:
- Ubuntu 22.04
- ROS 2 Humble
- `colcon`
- `rosdep`
- Python 3
- OpenCV
- PCL
- TBB
- working OAK-D LR driver via `depthai_ros`
- working RS-LiDAR M1P driver via `rslidar_sdk`

Optional but strongly recommended:
- NVIDIA GPU / CUDA if real-time YOLO is expected

---

## Core Dependencies

### ROS / apt dependencies

```bash
sudo apt update
sudo apt install -y \
  git \
  python3-colcon-common-extensions \
  python3-rosdep \
  python3-pip \
  libopencv-dev \
  libpcl-dev \
  libtbb-dev \
  ros-humble-cv-bridge \
  ros-humble-image-transport \
  ros-humble-message-filters \
  ros-humble-pcl-conversions \
  ros-humble-pcl-msgs \
  ros-humble-pcl-ros \
  ros-humble-rclpy \
  ros-humble-rviz2 \
  ros-humble-sensor-msgs \
  ros-humble-geometry-msgs \
  ros-humble-std-msgs \
  ros-humble-nav-msgs \
  ros-humble-visualization-msgs \
  ros-humble-tf2 \
  ros-humble-tf2-ros \
  ros-humble-tf2-geometry-msgs \
  ros-humble-ackermann-msgs \
  ros-humble-eufs-msgs
```

### Python dependencies

```bash
pip install "numpy<2" opencv-python typing-extensions
```

YOLO / vision-related:
```bash
pip install ultralytics==8.3.168 lap>=0.5.12
```

If ONNX is used:
```bash
pip install onnx onnxruntime
```

If GPU ONNX runtime is used:
```bash
pip install onnxruntime-gpu
```

If PyTorch inference is used:
```bash
pip install torch torchvision
```

---

## CUDA / GPU Notes

Useful checks:

```bash
nvidia-smi
nvcc --version
python3 -c "import torch; print(torch.cuda.is_available())"
```

The rest of the stack is not strictly CUDA-dependent, but the visual stage may be too slow on CPU for practical real-time use.

---

## Workspace Setup

```bash
mkdir -p ~/autonomous_ws/src
cd ~/autonomous_ws/src

git clone https://github.com/l4keisn0tr34l/autonomous-stack-sdc
git clone https://github.com/luxonis/depthai-ros.git
git clone https://github.com/RoboSense-LiDAR/rslidar_sdk.git

cd ~/autonomous_ws
```

Initialize rosdep if needed:

```bash
sudo rosdep init
rosdep update
```

Install dependencies from manifests:

```bash
rosdep install --from-paths src --ignore-src -r -y
```

Build:

```bash
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash
```

If custom messages fail to resolve, make sure:
- `sdc_msgs` is present
- ROS 2 is sourced before build
- the workspace is rebuilt after message changes

---

## Launch / Run Order

### 1. Camera driver
Bring up `depthai_ros` first.

Expected topics:
```text
/oak/rgb/image_raw
/oak/rgb/camera_info
```

---

### 2. LiDAR driver
Bring up `rslidar_sdk` so `/rslidar_points` is live.

---

### 3. Ground removal
Package:
```text
perception/lidar/ground-removal
```

Example:
```bash
ros2 launch ground_removal simpleZ.launch.py
```

---

### 4. Clustering
Package:
```text
perception/lidar/clustering
```

Example:
```bash
ros2 run clustering boundaryscan
```

This produces LiDAR cone candidates.

---

### 5. YOLO
Package:
```text
perception/camera/yolo_ros/yolo_bringup
```

Example:
```bash
ros2 launch yolo_bringup yolov9.launch.py
```

Expected detection topic:
```text
/yolo/detections
```

---

### 6. Fusion
Package:
```text
perception/sensorfusion
```

Example:
```bash
ros2 run sensorfusion sensor_fusion_node
```

Fusion currently:
- uses LiDAR cluster geometry as base truth
- assigns color if a YOLO bbox match exists
- marks unmatched cones as `unknown`
- publishes all cones forward on `/fused_cones`

Expected output:
```text
/fused_cones
```

---

### 7. Debug overlay *(optional but strongly recommended during setup)*
Package:
```text
perception/sensorfusion
```

Example:
```bash
ros2 run sensorfusion debug_overlay
```

This helps verify:
- camera intrinsics
- cluster projection into the image
- YOLO / LiDAR alignment
- current empirical fusion correction quality

Overlay currently visualizes:
- YOLO boxes
- projected LiDAR candidate points
- fused cones

---

### 8. Fused cone conversion
Package:
```text
perception/sensorfusion
```

Example:
```bash
ros2 run sensorfusion coord_to_arr
```

This converts:

```text
/fused_cones   (CoordinateList)
      ↓
/Clusters      (Arrofarr)
```

This step exists because mapping currently expects legacy array-of-arrays input.

---

### 9. Intermediate TF command before mapping
Before mapping, run the required TF/static-transform command used in your current integration workflow.

This is the step you mentioned is needed **after `coord_to_arr` and before mapping**.

If your team already has a fixed command for this, place it here in your local setup docs / scripts.

Example placeholder form:

```bash
# replace with the actual tf2/static transform command your stack uses
<your_tf2_command_here>
```

> Keep this step exactly where your current runtime expects it:
> after fusion/conversion is running, before mapping is launched.

---

### 10. Colored mapping
Package:
```text
slam/core/mapping1
```

Example:
```bash
ros2 launch mapping1 colorMapping.launch.py
```

Expected inputs:
- `/Clusters` (Arrofarr)
- `/odom`

Expected outputs:
- `/map_arr`
- map visualization markers

---

### 11. Planning
Examples:
```bash
ros2 launch path_planning1 midPointColored.launch.py
```

---

### 12. Control
Examples:
```bash
ros2 launch control pid.launch.py
```

---

## Topic Flow Overview

```text
/oak/rgb/image_raw + /oak/rgb/camera_info
        │
        └──> YOLO → /yolo/detections

/rslidar_points
        │
        └──> ground removal → clustering → /Clusters (CoordinateList)

 /Clusters + /yolo/detections + /oak/rgb/camera_info
        │
        └──> fusion → /fused_cones (CoordinateList, all cones preserved)

 /fused_cones
        │
        └──> coord_to_arr → /Clusters (Arrofarr)

 /Clusters + /odom
        │
        └──> coloredMapping → /map_arr

 /map_arr + /odom
        │
        └──> midPointColored → /cone_pair

 /cone_pair + /odom
        │
        └──> midPointErrorPropogator → /path_error

 /path_error
        │
        └──> control
```

---

## Important Internal Packages / Executables

### `sensorfusion`
Console entry points:
- `sensor_fusion_node`
- `debug_overlay`
- `coord_to_arr`

### `ground_removal`
Executables:
- `simpleZ`
- `lu`

### `clustering`
Executable:
- `boundaryscan`

### `mapping1`
Executables:
- `coloredMapping`
- `colorMapping`

### `path_planning1`
Executables:
- `midPoint`
- `midPointColored`
- `midPointErrorPropogator`
- `coneLineViz`

### `control`
Executable:
- `pid`

---

## Important Notes on Fusion

### Non-destructive fusion
This is one of the biggest changes in the real-world stack.

Old brittle behavior:
```text
LiDAR cone
→ if matched with YOLO: keep
→ else: drop
```

Current behavior:
```text
LiDAR cone
→ if matched with YOLO: keep with semantic colour
→ else: keep as unknown
```

This matters because mapping should not lose geometric cones just because far-box matching failed.

### Why this was necessary
As cones get farther:
- YOLO boxes get smaller
- LiDAR clusters get sparser
- tiny projection errors matter more

So making fusion non-destructive prevents valid cones from disappearing before mapping.

---

## Mapping Notes

Current mapping is simple and usable, but not especially robust:
- nearest-neighbor style association
- odometry-based global projection
- 2D colored cone map
- no advanced confidence / covariance / temporal filtering

It works as a baseline, but bad fusion or bad odometry will show up quickly in map quality.

This is exactly why fusion quality and debug overlay sanity matter.

---

## Common Failure Points

### 1. Camera node runs but YOLO gets no image
Possible causes:
- wrong image topic
- wrong camera info topic
- `depthai_ros` not running
- remap mismatch

Checks:
```bash
ros2 topic list
ros2 topic echo /oak/rgb/camera_info
```

---

### 2. LiDAR node runs but ground removal sees nothing
Possible causes:
- wrong LiDAR topic
- no point cloud
- `rslidar_sdk` config issue
- hardware / Ethernet issue

Checks:
```bash
ros2 topic list
ros2 topic hz /rslidar_points
```

---

### 3. Ground removal publishes empty output
Possible causes:
- aggressive filtering
- wrong input topic
- bad frame assumptions
- invalid point cloud

---

### 4. Clustering publishes no cones
Possible causes:
- too much ground removed
- thresholds too strict
- too little point density
- wrong input topic

---

### 5. YOLO does not start
Possible causes:
- missing weights
- missing Python packages
- broken model path
- ONNX / torch mismatch
- wrong image topic
- NumPy version mismatch

Checks:
```bash
python3 -c "import torch; print(torch.cuda.is_available())"
python3 -c "import cv2; print(cv2.__version__)"
nvidia-smi
```

---

### 6. Fusion runs but outputs nonsense
Possible causes:
- wrong intrinsics
- wrong image stream
- bad transform direction
- stale detections
- poor empirical correction

Checks:
```bash
ros2 topic echo /fused_cones
ros2 topic hz /fused_cones
```

Also use `debug_overlay`.

---

### 7. Debug overlay looks wrong
Possible causes:
- wrong camera topic
- wrong camera info
- projection mismatch
- stale asynchronous overlay data
- bad correction values

---

### 8. `coord_to_arr` runs but mapping gets nothing
Possible causes:
- wrong `/fused_cones` topic
- wrong message conversion
- mapping subscribed to wrong topic

---

### 9. Mapping behaves poorly
Possible causes:
- bad `/Clusters`
- missing `/odom`
- parameter mismatch
- sim-era assumptions still present
- fusion too sparse

---

### 10. Planning/control fail downstream
Possible causes:
- bad map
- bad odometry
- wrong planner input topic
- missing path/error topics

---

## Useful Debug Commands

### Nodes
```bash
ros2 node list
```

### Topics
```bash
ros2 topic list
ros2 topic type /topic_name
ros2 topic echo /topic_name
ros2 topic hz /topic_name
```

### Graph inspection
```bash
rqt_graph
```

### TF inspection
```bash
ros2 run tf2_tools view_frames
ros2 run tf2_ros tf2_echo <source_frame> <target_frame>
```

### Parameters
```bash
ros2 param list
ros2 param get /node_name parameter_name
```

---

## Notes on Custom Messages

This stack uses custom `sdc_msgs`, including:
- `Coordinate`
- `CoordinateList`
- `Arr`
- `Arrofarr`

Make sure:
- `sdc_msgs` is in the same workspace
- workspace builds cleanly
- `install/setup.bash` is sourced before launch

---

## Current Known Rough Edges

- Fusion still uses an empirical correction on top of calibration
- Mapping still expects legacy `Arrofarr`
- Debug overlay is useful but not perfectly synchronized
- Some packages still reflect older naming / sim-era assumptions

The stack is usable, but not fully cleaned architecturally yet.

---

## Recommended Future Improvements

- replace legacy `Arrofarr` mapping input with direct `CoordinateList`
- centralize projection logic so fusion and overlay cannot drift apart
- add unified launch files:
  - `perception.launch.py`
  - `mapping.launch.py`
  - `full_stack.launch.py`
- move real-car calibration / TF commands into a dedicated docs section
- improve mapping association / confidence handling

---

## Final Reminder

If something builds but behaves wrong, the issue is usually **not compilation**.  
It is usually one of:

- topic mismatch
- frame mismatch
- bad camera / LiDAR alignment
- stale upstream data
- bad calibration / correction
- missing driver
- missing odometry
