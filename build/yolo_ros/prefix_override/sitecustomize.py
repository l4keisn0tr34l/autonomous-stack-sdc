import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/beam/autonomous-stack-sdc/install/yolo_ros'
