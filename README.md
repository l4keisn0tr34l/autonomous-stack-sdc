# Autonomous Stack SDC

ROS 2-based autonomous driving stack for a Formula Student / driverless vehicle.

This repository contains the main software pipeline for:
- perception
- sensor fusion
- mapping / SLAM
- path planning
- control

The current real-world runtime flow is:

1. Camera node  
2. LiDAR node  
3. Ground removal  
4. Clustering  
5. YOLO  
6. Fusion  
7. Debug overlay (optional but strongly recommended)  
8. `coord_to_arr`  
9. Colored mapping  
10. Planning / control nodes

This repository is intended for a ROS 2 workspace and is organized around modular packages for each stage of the stack.

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
- `perception/lidar/ground-removal`  
  Removes ground points from raw LiDAR point clouds.

- `perception/lidar/clustering`  
  Clusters non-ground LiDAR points into cone candidates.

- `perception/camera/yolo_ros`  
  Runs YOLO-based visual detection on the camera stream.

- `perception/sensorfusion`  
  Fuses LiDAR clusters and camera detections, provides debug visualization, and converts fused cones into mapping input format.

### Messages
- `sdc_msgs`  
  Custom message definitions used across the stack.

### SLAM / Mapping
- `slam/core/mapping1`  
  Builds a colored cone map from fused perception output and odometry.

- `slam/core/odometry`  
  Odometry support package.

### Planning
- `path_planning1`  
  Midpoint-based path planning and related utilities.

### Control
- `control`  
  PID-based control node.

---

## Hardware Driver Requirements

This stack depends on external sensor driver repositories in addition to the code in this repo.

### Camera: Luxonis OAK-D LR
Required external repository:
- `depthai_ros`

Clone link:
```bash
git clone https://github.com/luxonis/depthai-ros.git
```

### LiDAR: RoboSense RS-LiDAR M1P
Required external repository:
- `rslidar_sdk`

Clone link:
```bash
git clone https://github.com/RoboSense-LiDAR/rslidar_sdk.git
```

> These driver repositories are not part of this repo by default. Clone them into the same ROS 2 workspace `src/` directory alongside this repository before building.

Example workspace layout:

```text
~/autonomous_ws/src/
├── autonomous-stack-sdc/
├── depthai-ros/
└── rslidar_sdk/
```

---

## Prerequisites

This repository assumes:

- **Ubuntu 22.04**
- **ROS 2 Humble**
- `colcon`
- `rosdep`
- Python 3
- OpenCV
- PCL
- TBB
- a working OAK-D LR driver via `depthai_ros`
- a working RS-LiDAR M1P driver via `rslidar_sdk`
- NVIDIA CUDA support if GPU-accelerated YOLO inference is desired

> This stack may still build without CUDA, but real-time YOLO performance usually requires GPU acceleration.

---

## Core Dependencies

Some dependencies are declared in `package.xml`, but users may still need to install system dependencies manually.

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

For YOLO:

```bash
pip install ultralytics==8.3.168 lap>=0.5.12
```

If ONNX inference is used:

```bash
pip install onnx onnxruntime
```

If GPU ONNX inference is used:

```bash
pip install onnxruntime-gpu
```

If PyTorch GPU inference is used anywhere in the visual stack:

```bash
pip install torch torchvision
```

---

## CUDA / GPU Notes

YOLO inference may require additional NVIDIA dependencies for usable real-time performance.

Recommended checks:

```bash
nvidia-smi
nvcc --version
python3 -c "import torch; print(torch.cuda.is_available())"
```

You may need:
- NVIDIA GPU driver
- CUDA Toolkit
- cuDNN
- CUDA-compatible PyTorch or ONNX Runtime

> The rest of the stack is not inherently CUDA-dependent, but the camera detection pipeline may be too slow on CPU for practical use.

---

## Workspace Setup

Clone everything into a ROS 2 workspace:

```bash
mkdir -p ~/autonomous_ws/src
cd ~/autonomous_ws/src

git clone <your-repo-url>
git clone https://github.com/luxonis/depthai-ros.git
git clone https://github.com/RoboSense-LiDAR/rslidar_sdk.git

cd ~/autonomous_ws
```

Initialize rosdep if needed:

```bash
sudo rosdep init
rosdep update
```

Install dependencies from package manifests:

```bash
rosdep install --from-paths src --ignore-src -r -y
```

Build:

```bash
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash
```

> If custom messages fail to resolve, make sure `sdc_msgs` is included in the workspace and rebuild the workspace after sourcing ROS 2 correctly.

---

## Launch / Run Order

The stack should be run in the following order.

### 1. Camera node
Sensor:
- **Luxonis OAK-D LR**

Driver repository:
- `depthai_ros`

Expected camera topics for the perception stack include:

```text
/oak/rgb/image_raw
/oak/rgb/camera_info
```

Bring up the camera driver from `depthai_ros` first.

### 2. LiDAR node
Sensor:
- **RoboSense RS-LiDAR M1P**

Driver repository:
- `rslidar_sdk`

Bring up the LiDAR driver first so that raw point clouds are being published before ground removal starts.

### 3. Ground removal
Package:
```text
perception/lidar/ground-removal
```

Example launch:
```bash
ros2 launch ground_removal simpleZ.launch.py
```

### 4. Clustering
Package:
```text
perception/lidar/clustering
```

Example run:
```bash
ros2 run clustering boundaryscan
```

### 5. YOLO
Package:
```text
perception/camera/yolo_ros/yolo_bringup
```

Example launch:
```bash
ros2 launch yolo_bringup yolov9.launch.py
```

### 6. Fusion
Package:
```text
perception/sensorfusion
```

Example run:
```bash
ros2 run sensorfusion sensor_fusion_node
```

### 7. Debug overlay (optional but recommended)
Package:
```text
perception/sensorfusion
```

Example run:
```bash
ros2 run sensorfusion debug_overlay
```

This node is optional for operation, but strongly recommended because it helps verify:
- camera intrinsics
- projection consistency
- cluster-to-bounding-box association
- overall fusion sanity

### 8. Fused cone conversion
Package:
```text
perception/sensorfusion
```

Example run:
```bash
ros2 run sensorfusion coord_to_arr
```

This converts fused cone output into the array format expected by the mapping stage.

### 9. Colored mapping
Package:
```text
slam/core/mapping1
```

Example launch:
```bash
ros2 launch mapping1 colorMapping.launch.py
```

### 10. Planning / control
Planning and control are downstream of the mapping output.

Examples:
```bash
ros2 launch path_planning1 midPointColored.launch.py
ros2 launch control pid.launch.py
```

---

## Topic Flow Overview

The effective data flow is:

```text
Camera image + camera info
            │
            ├──> YOLO detections
            │
LiDAR raw point cloud
            │
            └──> Ground removal ──> Clustering
                                     │
                                     └──> LiDAR cone candidates
YOLO detections + LiDAR cone candidates + camera info
                                     │
                                     └──> Sensor fusion
                                              │
                                              ├──> /fused_cones
                                              ├──> debug overlay
                                              └──> coord_to_arr
                                                       │
                                                       └──> /mapping_input
                                                                │
                                                                └──> Colored mapping
                                                                         │
                                                                         ├──> map output
                                                                         └──> planning / control
```

---

## Important Internal Packages / Executables

### `sensorfusion`
Available console entry points:
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

## Common Failure Points

### 1. Camera node is running but YOLO gets no image
Possible causes:
- wrong image topic
- wrong camera info topic
- `depthai_ros` driver not running
- topic remap mismatch

Check:
```bash
ros2 topic list
ros2 topic echo /oak/rgb/camera_info
```

---

### 2. LiDAR node runs but ground removal shows nothing
Possible causes:
- incorrect LiDAR topic name
- no point cloud being published
- `rslidar_sdk` driver misconfiguration
- Ethernet / device connectivity issue

Check:
```bash
ros2 topic list
ros2 topic hz <lidar_topic>
```

---

### 3. Ground removal publishes empty output
Possible causes:
- aggressive filtering parameters
- wrong input topic
- coordinate / frame assumptions
- invalid point cloud content

---

### 4. Clustering publishes no cones
Possible causes:
- ground removal removed too much
- cluster thresholds are too strict
- point cloud density too low
- wrong input topic

---

### 5. YOLO does not start
Possible causes:
- missing weights
- missing Python packages
- broken model path
- CUDA / ONNX / torch mismatch
- wrong input image topic

Check:
```bash
python3 -c "import torch; print(torch.cuda.is_available())"
python3 -c "import cv2; print(cv2.__version__)"
nvidia-smi
```

---

### 6. Fusion runs but publishes nothing
Possible causes:
- no detections from YOLO
- no clusters from LiDAR branch
- topic mismatch
- timing / synchronization issue
- bad frame assumptions

Check:
```bash
ros2 topic echo /fused_cones
ros2 topic hz /fused_cones
```

Also verify all upstream topics exist.

---

### 7. Debug overlay shows nonsense or misalignment
Possible causes:
- wrong camera intrinsics
- wrong projection assumptions
- stale detections
- wrong frame convention
- extrinsics not matching real setup

This is exactly why the debug overlay is recommended.

---

### 8. `coord_to_arr` runs but mapping receives nothing
Possible causes:
- wrong fused cone message format
- wrong mapping input topic
- converter not subscribed to the real fusion output topic

---

### 9. Mapping launches but behaves incorrectly
Possible causes:
- no `/mapping_input`
- no odometry
- parameter mismatch
- launch/config mismatch
- topic assumptions from sim vs real stack

---

### 10. Planning/control nodes do not work downstream
Possible causes:
- invalid map output
- missing odometry
- wrong planner input topic
- control expecting path data that is not being published

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

This stack uses custom messages from `sdc_msgs`, including:
- `Coordinate`
- `CoordinateList`
- `Arr`
- `Arrofarr`

Make sure:
- `sdc_msgs` is in the same workspace
- the workspace is built successfully
- `install/setup.bash` is sourced before launching nodes that depend on these messages

---

## Recommended Cleanup / Future Improvements

This repository is currently runnable as a modular stack, but a cleaner public-facing setup would benefit from:

- a unified root-level launch file for the perception stage
- a unified launch file for full stack startup
- a dedicated `docs/` folder for:
  - architecture
  - topic map
  - troubleshooting
- removal of purely experimental or legacy files
- standardization of package descriptions / licenses / maintainer fields

Suggested future launch files:
- `perception.launch.py`
- `slam.launch.py`
- `full_stack.launch.py`

---

## Final Reminder

This repository contains the stack logic, but successful deployment also depends on:
- correct hardware drivers
- correct sensor topics
- correct calibration / transforms
- correct workspace sourcing
- correct GPU / inference environment for YOLO

If something “runs” but the output is wrong, the issue is often not compilation — it is usually:
- a topic mismatch
- a frame mismatch
- a calibration mismatch
- or missing upstream data
