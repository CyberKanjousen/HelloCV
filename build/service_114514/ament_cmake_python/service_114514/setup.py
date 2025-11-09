from setuptools import find_packages
from setuptools import setup

setup(
    name='service_114514',
    version='0.0.0',
    packages=find_packages(
        include=('service_114514', 'service_114514.*')),
)
