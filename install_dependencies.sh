#!/usr/bin/env bash
set -e

echo "[1/6] Updating apt..."
sudo apt update

echo "[2/6] Installing ROS / system dependencies..."
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

echo "[3/6] Installing Python dependencies..."
python3 -m pip install --upgrade pip
python3 -m pip install "numpy<2" opencv-python typing-extensions
python3 -m pip install ultralytics==8.3.168 "lap>=0.5.12"
python3 -m pip install onnx onnxruntime

echo "[4/6] Updating rosdep..."
if ! command -v rosdep >/dev/null 2>&1; then
  echo "rosdep not found after install. Exiting."
  exit 1
fi

if [ ! -f /etc/ros/rosdep/sources.list.d/20-default.list ]; then
  echo "Initializing rosdep..."
  sudo rosdep init || true
fi

rosdep update

echo "[5/6] External sensor driver repositories required for this stack:"
echo "  - OAK-D LR camera: https://github.com/luxonis/depthai-ros.git"
echo "  - RS-LiDAR M1P:    https://github.com/RoboSense-LiDAR/rslidar_sdk.git"
echo ""
echo "Clone both into your ROS 2 workspace src/ directory before building."
echo "Example:"
echo "  cd ~/autonomous_ws/src"
echo "  git clone https://github.com/luxonis/depthai-ros.git"
echo "  git clone https://github.com/RoboSense-LiDAR/rslidar_sdk.git"

echo "[6/6] Done."
echo "If you want GPU acceleration for YOLO, also make sure NVIDIA drivers, CUDA, and cuDNN are installed correctly."
