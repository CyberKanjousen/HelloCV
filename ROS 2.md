1. 输入以下命令检查编码。

```plain
locale
```

编码应为UTF-8。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760097599004-cb58291b-b024-4e49-a5cd-fda2739619c7.png)

2. 安装Ubuntu Universe repository。

```plain
sudo apt install software-properties-common
sudo add-apt-repository universe
```

3. 添加源。

```plain
sudo apt update && sudo apt install curl -y
export ROS_APT_SOURCE_VERSION=$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F "tag_name" | awk -F\" '{print $4}')
curl -L -o /tmp/ros2-apt-source.deb "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.$(. /etc/os-release && echo ${UBUNTU_CODENAME:-${VERSION_CODENAME}})_all.deb"
sudo dpkg -i /tmp/ros2-apt-source.deb
```

4. 安装ROS2。

```plain
sudo apt update
sudo apt upgrade
sudo apt install ros-dev-tools
sudo apt install ros-humble-desktop
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760097862625-f3dbc30a-a2ad-441b-9264-ec7aa56069d7.png)

5. 配置运行环境。  
直接运行ROS 2内置的配置环境的可执行文件即可。

```plain
source /opt/ros/humble/setup.bash
```

验证安装：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760098061119-a813ab28-d4df-4c9a-9bc1-b9c760053bca.png)

安装过程中未遇到问题。

