from setuptools import find_packages, setup

package_name = 'sensorfusion'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='beam',
    maintainer_email='beam@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'sensor_fusion_node = sensorfusion.nodes.fusion_node:main',
            'debug_overlay = sensorfusion.nodes.debug_overlay:main',
            'coord_to_arr = sensorfusion.nodes.coord_to_arr:main',
        ],

    },
)
