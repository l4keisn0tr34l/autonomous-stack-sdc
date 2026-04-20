from setuptools import find_packages
from setuptools import setup

setup(
    name='sdc_msgs',
    version='0.0.0',
    packages=find_packages(
        include=('sdc_msgs', 'sdc_msgs.*')),
)
