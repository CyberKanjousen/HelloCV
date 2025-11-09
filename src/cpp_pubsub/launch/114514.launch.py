from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    """launch内容描述函数，由ros2 launch 扫描调用"""
    publisher_114514 = Node(
        package="cpp_pubsub",
        namespace="rclcpp",
        executable="publisher_114514",
        parameters=[{'content': '1919810'}]
    )
    subscriber_114514 = Node(
        package="cpp_pubsub",
        namespace="rclcpp",
        executable="subscriber_114514"
    )
    # 创建LaunchDescription对象launch_description,用于描述launch文件
    launch_description = LaunchDescription(
        [publisher_114514, subscriber_114514])
    # 返回让ROS2根据launch描述执行节点
    return launch_description
