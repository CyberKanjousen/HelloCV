# 准备工作

根据[官方文档](https://docs.ros.org/en/jazzy/How-To-Guides/Migrating-from-ROS1/Migrating-CPP-Package-Example.html)，ROS2的构建工具最好是`colcon`，我运行以下命令安装：

```shell
sudo apt install colcon
```

然后，按照[此处](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html#build-and-run)的步骤，新建项目文件夹（这也就是相当于创建了一个工作空间。ROS2的工作空间通常命名为`ros2_ws`），在项目根目录下新建`src`文件夹，在`src`目录下运行该命令，生成名称为`cpp_pubsub`的功能包：

```shell
ros2 pkg create --build-type ament_cmake cpp_pubsub
```

定位到项目文件夹下的`src/cpp_pubsub/src`目录，然后运行：

```shell
wget -O publisher_lambda_function.cpp https://raw.githubusercontent.com/ros2/examples/jazzy/rclcpp/topics/minimal_publisher/lambda.cpp
wget -O subscriber_lambda_function.cpp https://raw.githubusercontent.com/ros2/examples/jazzy/rclcpp/topics/minimal_subscriber/lambda.cpp
```

这两个命令先后下载了`talker` `listener`两个节点的`.cpp`文件（`pub/sup`系统）。然后在`package.xml`文件中添加构建工具`ament_cmake`一行之后加入：

```xml
<depend>rclcpp</depend>
<depend>std_msgs</depend>
```

把CMake重写成：（改编自[此处](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html#build-and-run)）

```cmake
cmake_minimum_required(VERSION 3.5)
project(cpp_pubsub)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(talker src/publisher_lambda_function.cpp)
ament_target_dependencies(talker rclcpp std_msgs)
add_executable(listener src/subscriber_lambda_function.cpp)
ament_target_dependencies(listener rclcpp std_msgs)

install(TARGETS
  talker
  listener
  DESTINATION lib/${PROJECT_NAME})

ament_package()
```

然后就可以着手编译和运行了。首先定位到项目根目录下检查是否有缺省的依赖：

```shell
rosdep install -i --from-path src --rosdistro jazzy -y
```

（我实际运行的时候，发现如果是首次运行`rosdep`命令的话，需要运行`sudo rosdep init`初始化。）

之后，用`colcon`构建：

```shell
colcon build --packages-select cpp_pubsub
```

设置并运行`talker`：

```shell
. install/setup.bash
ros2 run cpp_pubsub talker
```

在另外一个终端窗口设置并运行`listener`：

```shell
. install/setup.bash
ros2 run cpp_pubsub listener
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761657563296-62956e65-f012-4808-a805-53bcbc7ae41c.png)

## CLion

我曾经尝试直接用CLion创建ROS2工作空间，但是并没有成功。于是按照[CLion官方文档](https://www.jetbrains.com/help/clion/ros2-tutorial.html)进行手动配置。

先前我已经创建好工作空间了，接下来的步骤是生成汇编数据库：

```shell
colcon build --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761659235173-25afd339-cbb7-42fb-bad1-426ee81285f3.png)

确认在`biuld`目录下生成`compile_commands.json`文件。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761659244931-4e35401f-1fd6-4f90-b9ad-d33f16c337ec.png)

用CLion以项目形式打开该文件：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761659547933-5b57d934-1a2d-4332-8320-5e1a9fd0ca6a.png)

编译数据库成功导入：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761659593269-de464b74-22b5-4992-bcea-1cc800d86ac5.png)

但是根目录设置成了`build`，需要修改一下项目根目录：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761660055484-1adfb486-b0f1-4f5a-a62f-7b294d942dd2.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761660092244-5b25a2ee-2ecd-4925-a81f-a9d100f52d1c.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761660151664-e11b144b-e3be-4f3a-9718-d4123df94d0f.png)

为了~~懒~~提高效率，我新建了一个命令行脚本类型的运行/调试配置：（这个是`talker`的配置，`listener`同理）

```shell
colcon build --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja && . install/setup.bash && ros2 run cpp_pubsub talker
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761662384503-8a64b6c6-0a22-496a-a5de-89ea4a3e49e7.png)

`talker` `listener`对应的配置设置完以后，运行测试一下：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761662493272-1767321b-cdec-4f74-ae63-d1df3444a63b.png)

# 理解`package.xml`与`CMakeLists.txt`的作用

> Package manifest file which marks the root of a package and contains meta information about the package including its name, version, description, maintainer, license, dependencies, and more.
> 
> 出自[doc.ros.org](https://docs.ros.org/en/jazzy/Concepts/Advanced/About-Build-System.html)

既然已经有了`CMakeLists.txt`来定义项目依赖之类的了，为什么还需要`package.xml`呢？从官方文档的表述中可以看出，`package.xml`相当于一个包的清单，里面列出的不只有依赖，还有包的名称、维护者、版本号等等信息。在我看来，`CMakeLists.txt`使用的是CMake语言（本质上是一种**脚本语言**），用于用于定义项目的构建规则、依赖关系、编译选项等；`package.xml`使用的是可拓展文本**标记语言**（XML），用来传输和存储数据。

# 常用命令

ROS2只有CLI，并没有提供GUI，因此掌握一些常用的ROS2命令是必须的。以下命令的用法来自ROS2帮助命令`ros2 <command> -h`输出的信息。

## 运行指定数据包中的指定节点

```shell
ros2 run [--prefix PREFIX] package_name executable_name ...
```

可用参数：

- `package_name`: ROS数据包的名称
- `executable_name`: 可执行文件（通常是节点）
- `argv`: 启动参数

可用选项：

- `--prefix PREFIX`: 前缀命令，也就是在运行可执行文件之前要执行的命令。命令应当包含在引号之内（比如`--prefix 'gdb -ex run --args'`）

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762046800116-c7df3a6a-1e8b-4741-9d59-c246f527677b.png)

## 运行启动文件

```shell
ros2 launch [-n] [-d] [-p | -s] [-a] [--launch-prefix LAUNCH_PREFIX] [--launch-prefix-filter LAUNCH_PREFIX_FILTER] package_name [launch_file_name] [launch_arguments ...]
```

可用参数：

- `package_name`: 包含启动文件的ROS数据包的名称
- `launch_file_name`: 启动文件的名称（通常是`.py` `.yaml` `.xml`文件）
- `launch_arguments`: 运行启动文件的参数

可用选项：

- `-n` `--noninteractive`: 无交互式地运行启动系统，也就是说运行的时候没有控制台
- `-d` `--debug`: 启用debug模式，提供更多输出信息
- `-p` `--print` `--print-description`: 在不启动程序的情况下，将启动说明打印到控制台
- `-s` `--show-args` `--show-arguments`: 展示传入启动文件的参数
- `-a` `--show-all-subprocesses-output`: 使用`OVERRIDE_LAUNCH_PROCESS_OUTPUT`环境变量覆盖所有已启动子进程的输出配置，从而显示所有已启动子进程的输出
- `--launch-prefix LAUNCH_PREFIX`: 前缀命令，也就是在运行启动文件之前要执行的命令。命令应当包含在引号之内（比如`--prefix 'gdb -ex run --args'`）
- `-launch-prefix-filter`: `LAUNCH_PREFIX_FILTER`正则表达式模式，用于通过匹配可执行文件名称来过滤`--launch-prefix`所应用的可执行文件。

## 主题相关操作

### 输出指定主题的消息

```shell
ros2 topic echo [--spin-time SPIN_TIME] [-s] [--no-daemon] [--csv] [--field FIELD] [--full-length] [--truncate-length TRUNCATE_LENGTH] [--no-arr] [--no-str] [--flow-style] [--no-lost-messages] [--raw] [--filter FILTER_EXPR] [--once] [--timeout N] [--include-message-info] topic_name [message_type]
```

可用参数：

- `topic_name`: 需要监听的ROS主题名称
- `message_type`: ROS消息类型（比如`std_msgs/msg/String`）

可用选项：
回显消息中选定的字段。使用“.”选择子字段。例如，要回显 nav_msgs/msg/Odometry 消息的位置字段，请执行以下命令：'ros2 topic echo /odom --field pose.pose.position'
- `--spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `--csv`: 输出所有递归字段，以逗号分隔（比如`for plotting`）。如果同时使用了选项`--include-message-info`则会在前面添加以下字段：`source_timestamp` `received_timestamp` `publication_sequence_number` `reception_sequence_number`
- `--field FIELD`: 返回消息中选定的字段。使用`.`选择子字段。例如，要返回`nav_msgs/msg/Odometry`消息的位置字段，则执行：`ros2 topic echo /odom --field pose.pose.position`
- `--full-length` `-f`: 输出数组、字节、字符串中长度大于`--truncate-length`的所有元素；默认情况下，长度超过`--truncate-length`的元素会被截断，并在末尾添加`...`
- `--truncate-length TRUNCATE_LENGTH` `-l TRUNCATE_LENGTH`: 数组、字节、字符串的截断长度（默认值：128）
- `--no-arr`: 不输出数组类型的消息
- `--no-str`: 不输出字符串类型的消息
- `--flow-style`: 以块状样式输出该集合（`csv`格式不支持此功能）
- `--no-lost-messages`: 消息丢失时不报告
- `--raw`: 以原始二进制输出
- `--filter FILTER_EXPR`: 用于过滤输出消息的 Python 表达式。该表达式可以使用 Python 内置函数以及 `m`（消息本身）
- `--once`: 输出收到的第一条消息，然后退出
- `--timeout N`: 设置等待超时时间（单位：秒）
- `--include-message-info` `-i`: 显示关联消息的信息

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762047126124-3c2ae316-f159-4a2c-b1fb-4a52eadfcb3f.png)

### 输出可用主题列表

```shell
ros2 topic list [--spin-time SPIN_TIME] [-s] [--no-daemon] [-t] [-c] [--include-hidden-topics] [-v]
```

可用选项：

- `--spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `-t` `--show-types`: 额外显示主题类型
- `-c` `--count-topics`: 仅显示已发现的主题数量
- `--include-hidden-topics`: 包含隐藏主题
- `-v` `--verbose`: 列出所有主题的详细信息

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762047301025-c6d0791b-e922-4a52-8772-e2851e803e49.png)

第一次执行该命令时，主题`topic`内没有`publisher`或`subscriber`，输出列表中无`topic`；第二次执行时，运行了一个在`topic`发布消息的`publisher`，输出列表中有`topic`。我从这里看出，一个主题必须至少拥有一个`publisher`或`subscriber`才是一个可用主题，才能输出到列表中。

### 按消息类型查找相关主题

```shell
ros2 topic find [--spin-time SPIN_TIME] [-s] [--no-daemon] [-c] [--include-hidden-topics] topic_type
```

可用参数：

- `topic_type`: 目标主题的消息类型（比如`std_msgs/msg/String`）

可用选项：

- `--spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `-c` `--count-topics`: 仅显示已发现的主题数量
- `--include-hidden-topics`: 包含隐藏主题

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762047815756-92e82b89-9d91-44f9-81f1-b08c56cb8a05.png)

### 输出指定主题的信息

```shell
ros2 topic info [--spin-time SPIN_TIME] [-s] [--no-daemon] [--verbose] topic_name
```

可用参数：

- `topic_name`: 要获取信息的 ROS 主题名称（例如`/chatter`）

可用选项：

- `--spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `--verbose` `-v`: 输出详细信息，例如此主题的发布者和订阅者的节点名称、节点命名空间、主题类型、主题类型哈希、GUID、QoS 配置文件。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762047981681-d6cab5db-1dc7-4383-b379-ab26ea028e8b.png)

### 向指定主题发布消息

```shell
ros2 topic pub [--stdin] [-r N] [-p N] [-1 | -t TIMES] [-w WAIT_MATCHING_SUBSCRIPTIONS] [--max-wait-time-secs MAX_WAIT_TIME_SECS] [--keep-alive N] [-n NODE_NAME] [--spin-time SPIN_TIME] [-s] topic_name message_type [values]
```

可用参数：

- `topic_name`: ROS 主题名称（例如`/chatter`）
- `message_type`: ROS消息类型（例如`std_msgs/String`）
- `values`: 消息内容，以`YAML`格式填写（例如`'data: Hello World'`），否则消息将使用默认值。使用`now`占位符获取当前时间，使用`auto`占位符获取包含当前时间和空`frame_id`的`std_msgs.msg.Header`

可用选项：

- `--stdin`: 从标准输入读取值
- `-r N, --rate N`: 发布频率（单位：赫兹）（默认值：1）
- `-p N` `--print N`: 仅输出每第 N 条已发布的消息（默认值：1）
- `-1` `--once`: 发布一条消息并退出
- `-t TIMES` `--times TIMES`: 发布若干次数后退出
- `-w WAIT_MATCHING_SUBSCRIPTIONS` `--wait-matching-subscriptions WAIT_MATCHING_SUBSCRIPTIONS`: 等待直到找到指定数量的匹配订阅。使用`-1`或`-- once`或`--times`时默认为`1`，否则默认为`0`
- `--max-wait-time-secs MAX_WAIT_TIME_SECS`: 如果启用了 `--wait-until-matching-subscriptions` 选项，则此参数设置最大等待时间（单位：秒）。默认情况下，此标志未设置，这意味着订阅者将无限期等待
- `--keep-alive N`: 在发出最后一条消息后，保持发布节点活动`N`秒（默认值：0.1）
- `-n NODE_NAME` `--node-name NODE_NAME`: 创建的发布节点的名称
- `--spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762048984570-fabe8b70-3ee8-4df3-b298-cfb5414bf333.png)

### 输出指定主题的类型

```shell
ros2 topic type [--spin-time SPIN_TIME] [-s] [--no-daemon] topic_name
```

可用参数：

- `topic_name`: ROS 主题名称

可用选项：

- `--spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程

## 服务相关操作

### 呼起服务

```shell
ros2 service call [--stdin] [-r N] service_name service_type [values]
```

可用参数：

- `service_name`: 呼起的 ROS 服务名称（例如：`/add_two_ints`）
- `service_type`: ROS 服务类型 （例如：`std_srvs/srv/Empty`）
- `values`: 服务请求的值，以`YAML`格式填写（例如：'{a: 1, b: 2}'），否则服务请求将使用默认值

可用选项：

- `--stdin`: 从标准输入`stdin`读取值
- `-r N` `--rate N`: 以指定频率（单位：赫兹）反复调用。

### 输出可用服务列表

```shell
ros2 service list [--spin-time SPIN_TIME] [-s] [--no-daemon] [-t] [-c] [--include-hidden-services]
```

可用选项：

- `--spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `-t` `--show-types`: 额外显示服务类型
- `-c` `--count-services`: 仅显示发现的服务数量
- `--include-hidden-services`: 包含隐藏服务

## 设置 ROS2 参数

### 获取参数

```shell
ros2 param get [--spin-time SPIN_TIME] [-s] [--no-daemon] [--include-hidden-nodes] [--hide-type] [--timeout N] node_name parameter_name
```

可用参数：

- `node_name`: ROS节点名称
- `parameter_name`: 参数名称

可用选项：

- `-spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `--include-hidden-nodes`: 包含隐藏节点
- `--hide-type`: 隐藏类型信息
- `--timeout N`: 等待`N`秒，直到节点可用（默认值为`1`秒）

### 输出参数列表

```shell
ros2 param list [--spin-time SPIN_TIME] [-s] [--no-daemon] [--filter FILTER] [--include-hidden-nodes] [--param-prefixes PARAM_PREFIXES [PARAM_PREFIXES ...]] [--param-type] [node_name]
```

可用参数：

- `node_name`: ROS节点名称

可用选项：

- `-spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程 
- `--filter FILTER`: 仅显示与正则表达式匹配的参数。支持`re`正则表达式。
- `--include-hidden-nodes`: 包含隐藏节点

### 设置指定参数为某值

```shell
ros2 param set [--spin-time SPIN_TIME] [-s] [--no-daemon] [--include-hidden-nodes] [--timeout N] node_name parameter_name value
```

可用参数：

- `node_name`: ROS节点名称
- `parameter_name`: 参数名称
- `value`: 参数的值

可用选项：

- `-spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `--include-hidden-nodes`: 包含隐藏节点
- `--timeout N`: 等待`N`秒，直到节点可用（默认值为`1`秒）

## 节点相关操作

### 输出可用节点列表

```shell
ros2 node list [--spin-time SPIN_TIME] [-s] [--no-daemon] [-a] [-c]
```

可用选项：

- `-spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `-a` `--all`: 显示包括隐藏节点在内的所有节点
- `-c` `--count-nodes`: 仅显示发现的节点数量

### 输出节点信息

```shell
ros2 node info [--spin-time SPIN_TIME] [-s] [--no-daemon] [--include-hidden] node_name
```

可用参数：

- `node_name`: ROS节点名称

可用选项：

- `-spin-time SPIN_TIME`: 运行等待时间（秒）（仅适用于未使用已运行的守护进程的情况）
- `-s` `--use-sim-time`: 启用 ROS 模拟时间
- `--no-daemon`: 既不启动也不使用已在运行的守护进程
- `--include-hidden`: 显示隐藏的主题、服务、动作

# 节点

> A node is a participant in the ROS 2 graph, which uses a client library to communicate with other nodes. Nodes can communicate with other nodes within the same process, in a different process, or on a different machine. Nodes are typically the unit of computation in a ROS graph; each node should do one logical thing.
> 
> 出自[doc.ros.org](https://docs.ros.org/en/jazzy/Concepts/Basic/About-Nodes.html)

在我看来，节点是一个ROS2项目的基本单位。一个机器人的组件，比如摄像头之类的传感器，都是一个个节点。其在节点间的通信中可以充当`publisher` `subscriber` `service server` `service client` `action server` `action client`等角色（一个节点可以充当多种角色）。先前下载的两个`.cpp`文件就分别定义了`publisher` `subscriber`两个节点。

以下是先前下载的`publisher_lambda_function.cpp`文件的代码：（这是一个`publisher`）

```c++
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

// 创建一个继承自Node类的子类
class MinimalPublisher : public rclcpp::Node
{
public:
  // 构造函数，定义节点名，将0赋值给私有变量count_（也就是计数器）
  MinimalPublisher()
  : Node("minimal_publisher"), count_(0)
  {
    // 创建发布者，规定消息类型为std_msgs::msg::String，主题名称为topic，必要的队列大小为10
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    // 使用lambda表达式设置消息数据并发布消息
    auto timer_callback =
      [this]() -> void {
        auto message = std_msgs::msg::String();
        message.data = "Hello, world! " + std::to_string(this->count_++);
        // 将消息输出到控制台
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        // 将消息发布至主题
        this->publisher_->publish(message);
      };
    // 每500毫秒回调一次timer_callback
    timer_ = this->create_wall_timer(500ms, timer_callback);
  }

private:
  // 声明定时器、发布者的共享指针，声明计数器
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  // 初始化ROS2
  rclcpp::init(argc, argv);
  // 开始处理来自该节点的数据
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  // 关闭该节点的通信并清理资源
  rclcpp::shutdown();
  return 0;
}
```

显然，一个节点的生命周期是：
1. 初始化节点并运行
2. 执行构造函数中的代码
3. 结束运行，关闭该节点通信并清理资源

从上述文件来看，编写一个节点的核心步骤有两步：创建`Node`类的子类和编写主函数。实践部分的我放到了[创建话题发布与订阅节点](#创建话题发布与订阅节点)这一块。

# 话题`topic`

## Publisher-Subscriber

> 在 ROS2 中，话题（Topics）是一种用于不同节点之间进行异步通信的机制，话题通信的实现依赖于发布/订阅模式，通过底层的 DDS（数据分发服务）来确保消息的可靠传递。在话题通信中，发送数据的对象称为发布者（Publisher），接收数据的对象称为订阅者（Subscriber）。发布者将数据发送到指定的 Topic ，订阅 Topic 的订阅者都可以获取到数据，同时传输的数据也有固定的数据类型。
>
> 出自[foooor.com](https://foooor.com/ROS2/04-%E8%AF%9D%E9%A2%98%E9%80%9A%E4%BF%A1.html#_4-1-%E8%AF%9D%E9%A2%98%E7%AE%80%E4%BB%8B)

> DDS的全称为Data Distribution Service（数据分发服务)，是由OMG发布的分布式通信规范，采用发布/订阅模型，提供多种QoS服务质量策略。
> 
> DDS将分布式网络中传输的数据定义为“主题”，将数据的产生和接收对象分别定义为“发布者”和“订阅者”，从而构成数据的发布/订阅传输模型。各个节点在逻辑上无主从关系，点与点之间都是对等关系，通信方式可以是点对点、点对多、多对多等，在QoS的控制下建立连接，自动发现和配置网络参数。
> 
> 出自[zhiwango.com](https://zhiwango.com/2022/03/25/ros2-qos/)

在[官方文档](https://docs.ros.org/en/jazzy/Tutorials/Beginner-CLI-Tools/Understanding-ROS2-Nodes/Understanding-ROS2-Nodes.html)有一张动图形象地展示了节点间通信的原理：

![](https://docs.ros.org/en/jazzy/_images/Nodes-TopicandService.gif)

其中，`publisher`节点发布消息，该消息通过底层DDS，凭话题名称来分发到对应的`subscriber`节点。在我看来，从名字上就可以看出，之所以称这种消息分发的方式为"Publisher-Subscriber 模式"，应该是因为其过程和油管上订阅频道很相似吧。

## 创建话题发布与订阅节点

### `publisher`

`publisher`发布节点的编写方法在前面已经阐明，在这里直接来到我的实践部分：（相比之前那个`publisher`，这个缺省了计数器）（我一般按照创建源文件 -> 编辑`CMakeLists.txt` -> 重新构建 -> 正式编写代码这样的顺序，这样，在编写代码的时候源文件是包含于项目内的，因而可以使用语法检查器了。）

```c++
class Publisher_114514 : public rclcpp::Node {
public:
    Publisher_114514() : Node("publisher_114514") {
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
        auto time_back =
            [this] () -> void {
                auto message = std_msgs::msg::String();
                message.data = "114514";
                RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
                this->publisher_->publish(message);
            };
        time_ = this->create_wall_timer(500ms, time_back);
    }
private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr time_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Publisher_114514>());
    rclcpp::shutdown();
    return 0;
}
```

经我测试，主题名不得以数字开头，否则抛出`rclcpp::exceptions::InvalidTopicNameError`。

由于这不是CLion传统的C++项目，创建C++源文件的时候并不会自动将其包含在项目内，因而需要手动添加。首先，在`CmakeLists.txt`添加：

```cmake
add_executable(publisher_114514 src/publisher_114514.cpp)
ament_target_dependencies(publisher_114514 rclcpp std_msgs)
```

并在`install()`中添加一个名为`publisher_114514`的`TARGET`。然后，删除`build`文件夹下的内容，重新构建项目：

```shell
rm -rf build/* 
colcon build --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja
```

当然，这两行命令我也加进了CLion的运行/调试配置中。经我测试，如果不删除`build`内的文件的话，构建会失败。

然后，运行`publisher_114514`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761827982070-983f5c1f-ad98-4625-a67c-b60c5718817e.png)

运行先前下载的`listener`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1761828018525-289dcdd6-64bf-48d7-8530-1848788b0a4a.png)

由于`publisher_114514`发布的主题也是`topic`，因此即使`listener`没有经过任何改动，“听”到的消息却发生了改变。

### `subcriber`

从先前下载的`subscriber_lambda_function.cpp`探究`subcriber`节点的编写方法：

```c++
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class MinimalSubscriber : public rclcpp::Node
{
public:
  // 构造函数，定义节点名
  MinimalSubscriber()
  : Node("minimal_subscriber")
  {
    // 使用lambda表达式将消息输出到控制台
    auto topic_callback =
      [this](std_msgs::msg::String::UniquePtr msg) -> void {
        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
      };
    // 创建订阅者，订阅者从指定主题获取信息，并传入topic_callback
    subscription_ =
      this->create_subscription<std_msgs::msg::String>("topic", 10, topic_callback);
  }

private:
  // 声明订阅者的共享指针，接收类型为std_msgd::msg::Stirng的消息
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
```

和`publisher`不同的是，`subcriber`声明了一个`rclcpp::Subscription<typename>::SharedPtr`类型的订阅者，用于接受指定类型的消息。其他部分就和`publisher`大同小异了。尤其是`main()`方法，两者的格式几乎一样。

在这里我也动手创建一个`subscriber`节点：

```c++
class Subscriber_114514 : public rclcpp::Node {
public:
    Subscriber_114514() : Node("subscriber_114514") {
        auto topic_114514_callback = [this](std_msgs::msg::String::UniquePtr msg) -> void {
            RCLCPP_INFO(this->get_logger(), "%s", msg->data.c_str());
        };
        subscription_ = this->create_subscription<std_msgs::msg::String>("topic_114514", 10, topic_114514_callback);
    }
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Subscriber_114514>());
    rclcpp::shutdown();
    return 0;
}
```

配置好`CMakelists.txt`，测试一下（`publisher_114514`那边发布的主题已经改成了`topic_114514`）：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762352902673-1d22b54b-6165-4181-ad2b-d22b580205e3.png)

## QoS基础

在`Node`类中，`create_publisher()`是这样定义的

```c++
std::shared_ptr<PublisherT> create_publisher<MessageT, AllocatorT, PublisherT>(
    const std::string & topic_name, 
    const QoS & qos, 
    const PublisherOptionsWithAllocator<AllocatorT> & options = PublisherOptionsWithAllocator<AllocatorT>())
```

那么，QoS是什么呢？

> QoS是一种控制机制，它提供了针对不同用户或者不同数据流采用相应不同的优先级，或者是根据应用程序的要求，保证数据流的性能达到一定的水准。QoS的保证对于容量有限的网络来说是十分重要的，特别是对于流多媒体应用，例如VoIP和IPTV等，因为这些应用常常需要固定的传输率，对延迟也比较敏感。当网络面临带宽拥塞或需要设定流量优先级时（例如，让某个 VLAN 的流量优先于另一个 VLAN），服务质量 (QoS) 能优化网络资源分配，缓解间歇性流量问题。
> 
> 出自[Wikipedia](https://zh.wikipedia.org/wiki/%E6%9C%8D%E5%8A%A1%E8%B4%A8%E9%87%8F)

ROS2的`QoS`类有很多用于定义QoS预设配置的派生类，以方便适应于传输各种类型的数据：

- `SensorDataQoS`: 传感器数据
- `ClockQoS`: 时钟数据
- `ParametersQoS`: 参数
- `ServicesQoS`: 服务
- `ParameterEventsQoS`: 参数事件
- `RosoutQoS`: ROS输出数据
- `SystemDefaultsQoS`: 系统默认数据
- `BestAvailableQoS`: （该配置在保持最高服务水平的前提下，匹配当前绝大多数的可用端点。根据源代码的注释，应当慎用该配置）

如果要在项目中使用这些配置，只需将`qos`参数设为上述类的构造实例。比如，在`subscriber_114514`使用`SensorDataQoS`配置，需要把创建订阅者那一行改成：

```c++
subscription_ = this->create_subscription<std_msgs::msg::String>("topic_114514", rclcpp::SensorDataQoS(), topic_114514_callback);
```

# 服务`service`

##  Client-Server

服务通信也是ROS中一种极其常用的通信模式，但是和话题有很多不同。服务有点类似于方法的调用，客户端调用服务端，服务端返回结果给客户端。

服务分为服务端和客户端：

- 服务端：提供服务的节点，等待其他节点发出请求，并根据请求进行相应的处理，然后返回结果
- 客户端：发起请求的节点，将请求发送给服务端，等待服务端处理并返回响应

每个服务都需要定义一个服务类型，它包含请求和响应的消息结构。服务的类型定义通常包含两个部分：

- 请求：客户端发送给服务端的内容
- 响应：服务端返回给客户端的结果

在 ROS2 中，服务类型定义使用`.srv`文件，该文件包含请求和响应两部分。典型的`.srv`文件分为两部分，中间通过`---`分隔。比如：

```
# Request
int64 a
int64 b
---
# Response
int64 sum
```

在这个例子中，客户端请求两个整数的和，服务端返回求和结果。

服务和话题的区别：

| 特性      | 服务（Service）           | 话题（Topic）          |
|:--------|:----------------------|:-------------------|
| 通信模型    | 请求-响应，类似函数调用	         | 发布-订阅，异步数据流        |
| 典型场景    | 一次性任务，如执行动作、路径规划等     | 持续数据流，如传感器数据、状态更新  |
| 消息传递方式  | 同步，双向通信，一发一收          | 异步，单向通信，一发可多收      |
| 消息持续性   | 一次性通信，发出请求得到响应后结束     | 持续发布与接收消息          |
| 实现难易度	  | 简单，类似函数调用	            | 复杂，适合实时性要求高的数据流    |
| 实时性	    | 一般，服务需要等待请求处理完毕才能响应	  | 高，适合实时性要求较高的场景     |
| 可靠性	    | 可靠，请求必有响应	            | 依赖QOS配置，可能丢失消息     |

> 使用建议：
> 
> 服务：适用于需要执行某个明确的、有限时长的任务，并且需要返回结果的场景，如远程调用某个动作或获取某个特定信息。
> 
> 话题：适用于持续数据流动的场景，如定期传递传感器数据、机器人位置信息等，不需要明确的请求和响应。
> 
> 出自[foooor.com](https://foooor.com/ROS2/05-%E6%9C%8D%E5%8A%A1%E9%80%9A%E4%BF%A1.html#_5-1-%E6%9C%8D%E5%8A%A1%E7%AE%80%E4%BB%8B)

## 创建简单的服务与客户端节点

我一边参考着[官方文档](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Service-And-Client.html)的教程，一边动手写了一下一对简单的服务与客户端节点。客户端会向服务端发送请求，其中包含两个整数；服务端响应，将两数相加，将结果发回客户端。

### 自定义通信接口

我另外新建了一个数据包`service_114514`，用于自定义通信接口。

```shell
ros2 pkg create --build-type ament_cmake service_114514r
```

在数据包根目录下，创建一个`srv`目录，在`srv`目录下创建`AddTwoInts.srv`文件，内容如下：

```
# AddTwoInts.srv
int64 a
int64 b
---
int64 sum
```

内容包括两个部分，请求部分和响应部分，中间使用`---`分隔；

- `int64 a`和`int64 b`是客户端请求的两个整数
- `int64 sum`是服务端返回的结果（即两个整数的和）

然后，修改`package.xml`，添加以下依赖项：

```xml
<build_depend>rosidl_default_generators</build_depend>
<exec_depend>rosidl_default_runtime</exec_depend>
<member_of_group>rosidl_interface_packages</member_of_group>
```

这些依赖可以让 ROS2 在构建时生成对应的服务代码。

除了`package.xml`，`CMakelists.txt`也要修改，添加：

```cmake
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/AddTwoInts.srv"
  
  # 如果有多个srv或msg文件，在这里继续配置
  
  # DEPENDENCIES action_msgs std_msgs   # 如果用到其他依赖，可以在这里配置
)
```

这个时候可以执行`colcon build`来构建项目，会根据`srv`文件生成供 Python 和 C++ 调用的数据结构。

之后，要使用这个接口的话，就在`CMakeLists.txt`里写入：

```cmake
find_package(service_114514 REQUIRED)
```

## `server`

新建一个数据包用于存放服务通信相关的代码（注意要在工作区中的`src`目录下创建数据包）：

```shell
ros2 pkg create --build-type ament_cmake --license Apache-2.0 cpp_srvcli --dependencies rclcpp service_114514
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762438055280-253ee6c3-cd7c-42ae-bec8-124c4b0cfe06.png)

新建`server_114514.cpp`。在`CMakeLists.txt`里写入：

```cmake
add_executable(server_114514 src/server_114514.cpp)
ament_target_dependencies(server_114514 rclcpp service_114514)

install(TARGETS
        server_114514
        DESTINATION lib/${PROJECT_NAME})
```

由于在这个例子中不需要传递字符串信息，因而不需要添加`std_msgs`依赖。

重新构建以后，开始编写服务端节点。首先，包含头文件和库：

```c++
#include "rclcpp/rclcpp.hpp"
#include "service_114514/srv/add_two_ints.hpp" // 根据.srv文件自动生成的头文件
#include <memory>
```

然后编写节点类，和之前差不多，这里创建了一个`rclcpp::Service`类型的对象，使节点发挥服务端的作用。

```c++
class Server_114514 : public rclcpp::Node {
public:
    Server_114514() : Node("server_114514") {
        auto add =
            [] (const std::shared_ptr<service_114514::srv::AddTwoInts::Request> request,
         std::shared_ptr<service_114514::srv::AddTwoInts::Response> response) -> void {
             // 两数相加
                response->sum = request->a + request->b;
                RCLCPP_INFO (rclcpp::get_logger("rclcpp"),
                    "Incoming request\na: %ld" " b: %ld",
                    request->a, request->b);
                RCLCPP_INFO (rclcpp::get_logger("rclcpp"),
                    "Sending back response: [%ld]",
                    static_cast<long int>(response->sum));
        };
        service = this->create_service<service_114514::srv::AddTwoInts>("add_two_ints", add);
    }
private:
    // 声明服务端的共享指针
    rclcpp::Service<service_114514::srv::AddTwoInts>::SharedPtr service;
};
```

接下来编写主方法，也是和之前几乎一样：

```c++
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to add two ints.");
    rclcpp::spin(std::make_shared<Server_114514>());
    rclcpp::shutdown();
    return 0;
}
```

### `client`

包含头文件和库：

```c++
#include "rclcpp/rclcpp.hpp"
#include "service_114514/srv/add_two_ints.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
```

编写主方法。这里试着用面向过程的方式编写了一个客户端节点：

```c++
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    // 确保传入的参数数量准确
    if (argc != 3) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "usage: add_two_ints_client X Y");
        return 1;
    }
    // 声明节点类型的对象和客户端共享指针
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("add_two_ints_client");
    rclcpp::Client<service_114514::srv::AddTwoInts>::SharedPtr client =
      node->create_client<service_114514::srv::AddTwoInts>("add_two_ints");
    // 声明请求
    auto request = std::make_shared<service_114514::srv::AddTwoInts::Request>();
    request->a = atoll(argv[1]);
    request->b = atoll(argv[2]);
    // 给予客户端1秒时间寻找服务端节点
    while (!client->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
            return 0;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }
    // 向服务端发送请求
    auto result = client->async_send_request(request);
    // 处理响应
    if (rclcpp::spin_until_future_complete(node, result) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Sum: %ld", result.get()->sum);
    } else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service add_two_ints");
    }

    rclcpp::shutdown();
    return 0;
}
```
测试一下，首先启动服务端：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762511176041-079d7a8d-80fa-490a-89bb-9e635a50f6f5.png)

然后启动客户端，并附加两个整型参数：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762511243240-3be1704e-0303-4caf-91be-a267d0f7d703.png)

回到服务端所在的终端窗口，控制台上多了几行日志：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762511255327-e7a1b2da-662c-40b6-9c15-6df0d6d926d0.png)

# 参数

> 在 ROS2 中，每个节点都有自己的参数管理系统。参数直接与节点绑定，节点可以声明、获取和更新自己的参数。这种设计使得参数管理更加分散和灵活，适应了分布式系统的需求。
> 
> 出自 [foooor.com](https://foooor.com/ROS2/08-%E5%8F%82%E6%95%B0.html)

## 参数的声明、读取、设置

参数在节点内声明、读取、设置：

```c++
// 节点类
// 声明
this->declare_parameter<std::string>("my_param", "Hello, ROS2!");
// 读取
this->get_parameter("my_param").as_string();
// 设置
this->set_parameters({rclcpp::Parameter("my_param", "Hello, ROS2!")});
```

第一行代码声明了一个名称为`my_param`，默认值为`Hello, ROS2!`的参数，第二行则读取该参数并将读取结果转化成字符串，第三行则是把该参数设置为`Hello, ROS2!`。

比如，在之前的Pub/Sub实例中，可以在`publisher`中声明一个确定发布消息内容的参数：

```c++
this->declare_parameter<std::string>("content", "114514");

// 在 lambda 表达式内，获取该参数并将其赋给 message.data
message.data = this->get_parameter("content").as_string();
```

运行该节点：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762513853065-085b0813-4801-455e-8d06-9e79ab7fc2b2.png)

输出节点参数的列表：

```shell
ros2 param list
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762514021945-bfda17d8-ebdb-44a5-8d41-20b32f4c77e9.png)

查看该参数的描述信息：

```shell
ros2 param describe /publisher_114514 content
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762514197038-87447464-f1b5-4012-8b30-c2aa7956d0c5.png)

查看参数的值：

```shell
ros2 param get /publisher_114514 content
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762514394013-7d8c6f8e-0fed-466f-b3e3-6e0c20087d58.png)

修改参数：

```shell
ros2 param set /publisher_114514 content "'1919810'"
```

由于`1919810`或`"1919810"`或`'1919810'`会被识别成整型而非字符串，进而导致修改参数失败，因而写成`"'1919810'"`。这时再看`publisher_114514`所在的终端窗口，发布的信息由`114514`变为了`1919810`。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762514827310-98081347-1bc6-4cc7-a751-1e10d27ebd03.png)

参数也可以在launch文件中设置，详见[在launch文件中设置参数](#在launch文件中设置参数)

# `rviz`可视化

`rviz`是 ROS 中提供的一个强大的数据可视化的工具，用于显示机器人和传感器数据，帮助开发者调试和开发机器人应用。

在 ROS2 中，在终端运行如下命令可以打开`rviz`：

```shell
# 打开rviz
rviz2

# 或者
ros2 run rviz2 rviz2

# 或者指定配置文件
ros2 run rviz2 rviz2 -d config_file.rviz
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762515521190-350e4198-10b8-4643-bd18-6ab3b700b7be.png)

## TF树（`tf2`）

> TF(TransForm)，就是坐标转换，包括了位置和姿态两个方面的变换。注意区分坐标转换和坐标系转换。
> 
> 坐标转换是一个坐标在不同坐标系下的表示，而坐标系转换不同坐标系的相对位姿关系。
> 
> ROS中机器人模型包含大量的部件，每一个部件统称之为link（比如手部、头部、某个关节、某个连杆）,每一个link上面对应着一个frame（坐标系）, 用frame表示该部件的坐标系，frame和link是绑定在一起的。
> 
> TF是一个通俗的名称，实际上它有很多含义：
> 
> - 可以被当做是一种标准规范，这套标准定义了坐标转换的数据格式和数据结构．tf本质是树状的数据结构，即"tf tree"。
> - tf也可以看成是一个话题/tf，话题中的消息保存的就是tf tree的数据结构格式。维护了整个机器人的甚至是地图的坐标转换关系。维持并更新机器人整个坐标系的话题是/tf，/tf话题表示的内容是整个机器人的tf树，而非仅仅是某两个坐标系的转换关系，这样的话，/tf话题是需要很多的节点来维护的，每一个节点维护两个frame之间的关系。
> - tf还可以看成是一个package,它当中包含了很多的工具．比如可视化，查看关节间的tf,debug tf等等．
> - tf含有一部分的API接口，用来节点程序中的编程。TF对发布器与订阅器进行了封装，使开发者通过TF的接口更加简单地建立对TF树中某些坐标系转换关系的维护与订阅。
> - tf是一个树状结构，维护坐标系之间的关系，靠话题通信机制来持续地发布不同link之间的坐标关系。作为树状结构，要保证父子坐标系都有某个节点在持续地发布他们之间的位姿关系，才能使树状结构保持完整。只有父子坐标系的位姿关系能被正确的发布，才能保证任意两个frame之间的连通。
> 
> 如果出现某一环节的断裂，就会引发error系统报错．所以完整的tf tree不能有任何断层的地方，这样我们才能查清楚任意两个frame之间的关系。
>
> 每两个相邻frame之间靠节点发布它们之间的位姿关系，这种节点称为broadcaster。broadcaster就是一个发布器publisher,如果两个frame之间发生了相对运动，broadcaster就会发布相关消息。
> 
> 出自[cnblogs.com](https://www.cnblogs.com/sxy370921/p/11726691.html)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762519346839-526d1f26-963d-4ffc-9aa2-7504574e6422.png)

点击`rviz`页面左下角的`Add`，然后选择`TF`并添加，然后就可以在`rviz`查看TF数据了：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762517502422-1433664f-358b-4004-9295-7eb95e06f32c.png)

这里除了`world`以外暂且还没有东西，于是我按照[官方文档](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Tf2/Writing-A-Tf2-Static-Broadcaster-Cpp.html)，下载了一个“小海龟”静态广播节点：

```c++
#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/static_transform_broadcaster.h"

class StaticFramePublisher : public rclcpp::Node
{
public:
  explicit StaticFramePublisher(char * transformation[])
  : Node("static_turtle_tf2_broadcaster")
  {
    tf_static_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

    // Publish static transforms once at startup
    this->make_transforms(transformation);
  }

private:
  void make_transforms(char * transformation[])
  {
    geometry_msgs::msg::TransformStamped t;

    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "world";
    t.child_frame_id = transformation[1];

    t.transform.translation.x = atof(transformation[2]);
    t.transform.translation.y = atof(transformation[3]);
    t.transform.translation.z = atof(transformation[4]);
    tf2::Quaternion q;
    q.setRPY(
      atof(transformation[5]),
      atof(transformation[6]),
      atof(transformation[7]));
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    tf_static_broadcaster_->sendTransform(t);
  }

  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_static_broadcaster_;
};

int main(int argc, char * argv[])
{
  auto logger = rclcpp::get_logger("logger");

  // Obtain parameters from command line arguments
  if (argc != 8) {
    RCLCPP_INFO(
      logger, "Invalid number of parameters\nusage: "
      "$ ros2 run learning_tf2_cpp static_turtle_tf2_broadcaster "
      "child_frame_name x y z roll pitch yaw");
    return 1;
  }

  // As the parent frame of the transform is `world`, it is
  // necessary to check that the frame name passed is different
  if (strcmp(argv[1], "world") == 0) {
    RCLCPP_INFO(logger, "Your static turtle name cannot be 'world'");
    return 2;
  }

  // Pass parameters and initialize node
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StaticFramePublisher>(argv));
  rclcpp::shutdown();
  return 0;
}
```

编辑一下`CMakeLists.txt`：

```cmake
add_executable(static_turtle_tf2_broadcaster src/static_turtle_tf2_broadcaster.cpp)
ament_target_dependencies(
        static_turtle_tf2_broadcaster
        geometry_msgs
        rclcpp
        tf2
        tf2_ros
)

install(TARGETS
        static_turtle_tf2_broadcaster
        DESTINATION lib/${PROJECT_NAME})
```

构建完成后，运行一下：

```shell
. install/setup.bash
ros2 run learning_tf2_cpp static_turtle_tf2_broadcaster mystaticturtle 0 0 1 0 0 0
```

这里设置了一个在(0, 0, 1)发送广播的“小海龟”。

这个节点会将TF信息发布在`/tf_static`主题上：

```shell
ros2 topic echo /tf_static
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762519117273-75d57cef-9c82-4932-be30-2d3802c84429.png)

另一边，`rviz`也有了一些变化：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762520309976-8863bc9b-6ce7-496d-a87a-762c18081a3f.png)

不再只有`world`，新增了`mystaticturtle`。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762520669783-a5fef5d2-e6c8-4057-9435-c91ad76e6431.png)

在左侧菜单栏中，`mystaticturtle`的几个坐标从上到下依次为：位置、方向、相对位置、相对方向。

## 生成坐标系图

执行以下命令，即可生成坐标系图：

```shell
ros2 run tf2_tools view_frames
```

如果此时上文中的`mystaticturtle`处于运行状态，则输出的`.pdf`文件如下：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762521443940-31cdd9bd-dd2f-45ff-ac49-b933d8b9a8c8.png)

从这里可以看出，`mystaticturtle`是`world`的子项。

# `launch`文件

如果有多个节点需要运行，一个个输入命令来运行节点实在繁琐，而在CLion编写一长串命令并创建一个Shell类型的运行/调试配置又实在\*问候用语\*。有没有什么高效一点的方法呢？有的，这就是`launch`文件。

launch文件允许同时启动和配置多个包含ROS2节点的可执行文件。

ROS2的launch文件有三种格式，python、xml、yaml。其中ROS2官方推荐的是python方式编写launch文件。 原因在于，相较于XML和YAML，Python是一个编程语言，更加的灵活，可以利用Python的很多库来做一些其他工作（比如创建一些初始化的目录等）。

首先，在数据包根目录下新建`launch`目录：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762522603234-c60613de-a076-4b4e-973b-8057701e2f07.png)

在该目录下新建`114514.launch.py`文件：

```python
from launch import LaunchDescription # launch文件的描述类
from launch_ros.actions import Node # 节点启动的描述类


def generate_launch_description():
    """launch内容描述函数，由ros2 launch 扫描调用"""
    publisher_114514 = Node(
        package="cpp_pubsub",
        namespace="rclcpp", # 定义命名空间
        executable="publisher_114514",
        parameters=[{'content': '1919810'}] # 定义参数
    )
    subscriber_114514 = Node(
        package="cpp_pubsub",
        namespace="rclcpp",
        executable="subscriber_114514"
    )
    # 创建LaunchDescription对象launch_description,用于描述launch文件
    launch_description = LaunchDescription(
        [publisher_114514, subscriber_114514])
    # 返回launch文件的描述信息，让ROS2据此执行节点
    return launch_description

```

如果要重命名的话，就在构造函数`Node()`中加上`name`一项，比如：

```python
name="publisher_1919810"
```

如果编写完成后直接编译，就会发现install目录下根本没有launch文件，后续launch自然也找不到这个launch文件。

因为这里用的是ament_cmake类型功能包，所以要使用cmake命令进行文件的拷贝：

```cmake
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})
```

重新构建以后，执行：

```shell
ros2 launch cpp_pubsub 114514.launch.py
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762524109648-18795ee6-c59f-4264-a6c1-c63397d1fd20.png)

## 话题重映射

在构造函数`Node()`中加上`remapping`一项，比如：

```python
remappings=[                  # 重映射列表
    ('/input/pose', '/turtlesim1/turtle1/pose'),         # 将/input/pose话题名修改为/turtlesim1/turtle1/pose
    ('/output/cmd_vel', '/turtlesim2/turtle1/cmd_vel'),  # 将/output/cmd_vel话题名修改为/turtlesim2/turtle1/cmd_vel
]
```

## 在launch文件中设置参数

如果前文中`publisher_114514.cpp`中设置的参数改在launch文件中设置的话，则如下所示：

```python
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument       # 声明launch文件内使用的Argument类
from launch.substitutions import LaunchConfiguration, TextSubstitution

from launch_ros.actions import Node


def generate_launch_description():
    content_arg = DeclareLaunchArgument(
        'content', default_value=TextSubstitution(text='0') # 创建一个Launch文件内参数 content
    )
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
    launch_description = LaunchDescription(
        [content_arg, publisher_114514, subscriber_114514])
    return launch_description
```

[语雀笔记页面](https://www.yuque.com/u29112212/ucgrla/pkb51e8poq8hb2fg)