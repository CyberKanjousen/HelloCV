# 基本操作

首先认识一下SDF文件：

> SDFormat (Simulation Description Format), sometimes abbreviated as SDF, is an XML format that describes objects and environments for robot simulators, visualization, and control.
> 
> 出自[gazebosim.org](https://gazebosim.org/docs/harmonic/building_robot/)

## 加载世界

用命令加载某个世界：

```shell
gz sim world.sdf
```

如果要加载一个空白世界的话，那就执行`gz sim`，然后在弹出的图形界面选择`Empty`并运行。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762569723877-d798d427-100b-4acb-adea-1be083927b83.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762569764383-0680f057-0b6e-4cd5-9df8-cefbd55068eb.png)

## 插入模型

左上角已经预设了几种简单的模型，可以快速插入到世界中。比如，插入一个正方体（箱子）：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762570179014-1f0741b4-44c7-4ad0-bf3f-1d3468bd8b4d.png)

右键这个箱子，点击`Remove`即可移除。

如果要插入复杂一些的模型，比如一个小车的话，则需要在`.sdf`文件中写入：（代码出自[此处](https://github.com/gazebosim/docs/blob/master/harmonic/tutorials/building_robot/building_robot.sdf)）

```xml
<model name='vehicle_blue' canonical_link='chassis'>
    <pose relative_to='world'>0 0 0 0 0 0</pose>   <!--the pose is relative to the world by default-->

    <!--chassis-->
    <link name='chassis'>
        <pose relative_to='__model__'>0.5 0 0.4 0 0 0</pose>
        <inertial> <!--inertial properties of the link mass, inertia matix-->
            <mass>1.14395</mass>
            <inertia>
                <ixx>0.095329</ixx>
                <ixy>0</ixy>
                <ixz>0</ixz>
                <iyy>0.381317</iyy>
                <iyz>0</iyz>
                <izz>0.476646</izz>
            </inertia>
        </inertial>
        <visual name='visual'>
            <geometry>
                <box>
                    <size>2.0 1.0 0.5</size>
                </box>
            </geometry>
            <!--Color of the link-->
            <material>
                <ambient>0.0 0.0 1.0 1</ambient>
                <diffuse>0.0 0.0 1.0 1</diffuse>
                <specular>0.0 0.0 1.0 1</specular>
            </material>
        </visual>
        <collision name='collision'>
            <geometry>
                <box>
                    <size>2.0 1.0 0.5</size>
                </box>
            </geometry>
        </collision>
    </link>

    <!--Left wheel-->
    <link name='left_wheel'>
        <pose relative_to="chassis">-0.5 0.6 0 -1.5707 0 0</pose> <!--angles are in radian-->
        <inertial>
            <mass>1</mass>
            <inertia>
                <ixx>0.043333</ixx>
                <ixy>0</ixy>
                <ixz>0</ixz>
                <iyy>0.043333</iyy>
                <iyz>0</iyz>
                <izz>0.08</izz>
            </inertia>
        </inertial>
        <visual name='visual'>
            <geometry>
                <cylinder>
                    <radius>0.4</radius>
                    <length>0.2</length>
                </cylinder>
            </geometry>
            <material>
                <ambient>1.0 0.0 0.0 1</ambient>
                <diffuse>1.0 0.0 0.0 1</diffuse>
                <specular>1.0 0.0 0.0 1</specular>
            </material>
        </visual>
        <collision name='collision'>
            <geometry>
                <cylinder>
                    <radius>0.4</radius>
                    <length>0.2</length>
                </cylinder>
            </geometry>
        </collision>
    </link>

    <!--The same as left wheel but with different position-->
    <link name='right_wheel'>
        <pose relative_to="chassis">-0.5 -0.6 0 -1.5707 0 0</pose> <!--angles are in radian-->
        <inertial>
            <mass>1</mass>
            <inertia>
                <ixx>0.043333</ixx>
                <ixy>0</ixy>
                <ixz>0</ixz>
                <iyy>0.043333</iyy>
                <iyz>0</iyz>
                <izz>0.08</izz>
            </inertia>
        </inertial>
        <visual name='visual'>
            <geometry>
                <cylinder>
                    <radius>0.4</radius>
                    <length>0.2</length>
                </cylinder>
            </geometry>
            <material>
                <ambient>1.0 0.0 0.0 1</ambient>
                <diffuse>1.0 0.0 0.0 1</diffuse>
                <specular>1.0 0.0 0.0 1</specular>
            </material>
        </visual>
        <collision name='collision'>
            <geometry>
                <cylinder>
                    <radius>0.4</radius>
                    <length>0.2</length>
                </cylinder>
            </geometry>
        </collision>
    </link>

    <!--arbitrary frame-->
    <frame name="caster_frame" attached_to='chassis'>
        <pose>0.8 0 -0.2 0 0 0</pose>
    </frame>

    <!--caster wheel-->
    <link name='caster'>
        <pose relative_to='caster_frame'/>
        <inertial>
            <mass>1</mass>
            <inertia>
                <ixx>0.016</ixx>
                <ixy>0</ixy>
                <ixz>0</ixz>
                <iyy>0.016</iyy>
                <iyz>0</iyz>
                <izz>0.016</izz>
            </inertia>
        </inertial>
        <visual name='visual'>
            <geometry>
                <sphere>
                    <radius>0.2</radius>
                </sphere>
            </geometry>
            <material>
                <ambient>0.0 1 0.0 1</ambient>
                <diffuse>0.0 1 0.0 1</diffuse>
                <specular>0.0 1 0.0 1</specular>
            </material>
        </visual>
        <collision name='collision'>
            <geometry>
                <sphere>
                    <radius>0.2</radius>
                </sphere>
            </geometry>
        </collision>
    </link>

    <!--left wheel joint-->
    <joint name='left_wheel_joint' type='revolute'>
        <pose relative_to='left_wheel'/>
        <parent>chassis</parent>
        <child>left_wheel</child>
        <axis>
            <xyz expressed_in='__model__'>0 1 0</xyz> <!--can be descired to any frame or even arbitrary frames-->
            <limit>
                <lower>-1.79769e+308</lower>    <!--negative infinity-->
                <upper>1.79769e+308</upper>     <!--positive infinity-->
            </limit>
        </axis>
    </joint>

    <!--right wheel joint-->
    <joint name='right_wheel_joint' type='revolute'>
        <pose relative_to='right_wheel'/>
        <parent>chassis</parent>
        <child>right_wheel</child>
        <axis>
            <xyz expressed_in='__model__'>0 1 0</xyz>
            <limit>
                <lower>-1.79769e+308</lower>    <!--negative infinity-->
                <upper>1.79769e+308</upper>     <!--positive infinity-->
            </limit>
        </axis>
    </joint>

    <!--caster wheel joint--> <!--pose defult value is the child-->
    <joint name='caster_wheel' type='ball'>
        <parent>chassis</parent>
        <child>caster</child>
    </joint>
</model>
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762570633891-ad26936e-8efe-4295-bc8e-7ba17e191352.png)

## 启用仿真

点击左下角按钮开启仿真。比如这个模拟一串珠子在风的作用下漂浮的例子：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762581956165-d79a1b82-3efb-4996-a56d-604075f816fa.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762582029423-2331cab9-57e8-4a92-b7a7-5ee453331eb8.png)

# 理解`.sdf`文件结构

在目前Gazebo仍受支持的版本中，世界的数据跟模型都存储在同一个`.sdf`文件中，而不再存储于`.world`文件。因而在这里理解了`.sdf`文件的结构，就相当于理解了旧版本中`.world`和`.sdf`文件的结构。

我通过借助[此处](https://github.com/gazebosim/docs/blob/master/harmonic/tutorials/moving_robot/moving_robot.sdf)的内容来理解`.sdf`文件结构，顺便学习以下通过键盘输入或向主题发布消息以令小车移动的方法：

```xml
<?xml version="1.0" ?>
<!--规定.sdf文件的版本-->
<sdf version="1.8">
    <!--定义世界名称-->
    <world name="Moving_robot">
        
        <!--世界的物理信息-->
        <physics name="1ms" type="ignored">
            <max_step_size>0.001</max_step_size>
            <real_time_factor>1.0</real_time_factor>
        </physics>
        
        <!--插件注入-->
        <plugin
                filename="gz-sim-physics-system"
                name="gz::sim::systems::Physics">
        </plugin>
        <plugin
                filename="gz-sim-user-commands-system"
                name="gz::sim::systems::UserCommands">
        </plugin>
        <plugin
                filename="gz-sim-scene-broadcaster-system"
                name="gz::sim::systems::SceneBroadcaster">
        </plugin>
        
        <!--光源-->
        <light type="directional" name="sun">
            <cast_shadows>true</cast_shadows>
            <pose>0 0 10 0 0 0</pose>
            <diffuse>0.8 0.8 0.8 1</diffuse>
            <specular>0.2 0.2 0.2 1</specular>
            <attenuation>
                <range>1000</range>
                <constant>0.9</constant>
                <linear>0.01</linear>
                <quadratic>0.001</quadratic>
            </attenuation>
            <direction>-0.5 0.1 -0.9</direction>
        </light>
        
        <!--插入平地模型-->
        <model name="ground_plane">
            <static>true</static>
            <link name="link">
                <collision name="collision">
                    <geometry>
                        <plane>
                            <normal>0 0 1</normal>
                        </plane>
                    </geometry>
                </collision>
                <visual name="visual">
                    <geometry>
                        <plane>
                            <normal>0 0 1</normal>
                            <size>100 100</size>
                        </plane>
                    </geometry>
                    <material>
                        <ambient>0.8 0.8 0.8 1</ambient>
                        <diffuse>0.8 0.8 0.8 1</diffuse>
                        <specular>0.8 0.8 0.8 1</specular>
                    </material>
                </visual>
            </link>
        </model>
        
        <!--插入小车模型-->
        <model name='vehicle_blue' canonical_link='chassis'>
            <pose relative_to='world'>0 0 0 0 0 0</pose>   <!--the pose is relative to the world by default-->

            <!--以下分别插入小车各个部分的模型链接-->
            <link name='chassis'>
                <pose relative_to='__model__'>0.5 0 0.4 0 0 0</pose>
                <inertial> <!--inertial properties of the link mass, inertia matix-->
                    <mass>1.14395</mass>
                    <inertia>
                        <ixx>0.126164</ixx>
                        <ixy>0</ixy>
                        <ixz>0</ixz>
                        <iyy>0.416519</iyy>
                        <iyz>0</iyz>
                        <izz>0.481014</izz>
                    </inertia>
                </inertial>
                <visual name='visual'>
                    <geometry>
                        <box>
                            <size>2.0 1.0 0.5</size> <!--question: this size is in meter-->
                        </box>
                    </geometry>
                    <!--let's add color to our link-->
                    <material>
                        <ambient>0.0 0.0 1.0 1</ambient>
                        <diffuse>0.0 0.0 1.0 1</diffuse>
                        <specular>0.0 0.0 1.0 1</specular>
                    </material>
                </visual>
                <collision name='collision'> <!--todo: describe why we need the collision-->
                    <geometry>
                        <box>
                            <size>2.0 1.0 0.5</size>
                        </box>
                    </geometry>
                </collision>
            </link>

            <!--let's build the left wheel-->
            <link name='left_wheel'>
                <pose relative_to="chassis">-0.5 0.6 0 -1.5707 0 0</pose> <!--angles are in radian-->
                <inertial>
                    <mass>2</mass>
                    <inertia>
                        <ixx>0.145833</ixx>
                        <ixy>0</ixy>
                        <ixz>0</ixz>
                        <iyy>0.145833</iyy>
                        <iyz>0</iyz>
                        <izz>0.125</izz>
                    </inertia>
                </inertial>
                <visual name='visual'>
                    <geometry>
                        <cylinder>
                            <radius>0.4</radius>
                            <length>0.2</length>
                        </cylinder>
                    </geometry>
                    <material>
                        <ambient>1.0 0.0 0.0 1</ambient>
                        <diffuse>1.0 0.0 0.0 1</diffuse>
                        <specular>1.0 0.0 0.0 1</specular>
                    </material>
                </visual>
                <collision name='collision'>
                    <geometry>
                        <cylinder>
                            <radius>0.4</radius>
                            <length>0.2</length>
                        </cylinder>
                    </geometry>
                </collision>
            </link>

            <!--copy and paste for right wheel but change position-->
            <link name='right_wheel'>
                <pose relative_to="chassis">-0.5 -0.6 0 -1.5707 0 0</pose> <!--angles are in radian-->
                <inertial>
                    <mass>1</mass>
                    <inertia>
                        <ixx>0.145833</ixx>
                        <ixy>0</ixy>
                        <ixz>0</ixz>
                        <iyy>0.145833</iyy>
                        <iyz>0</iyz>
                        <izz>0.125</izz>
                    </inertia>
                </inertial>
                <visual name='visual'>
                    <geometry>
                        <cylinder>
                            <radius>0.4</radius>
                            <length>0.2</length>
                        </cylinder>
                    </geometry>
                    <material>
                        <ambient>1.0 0.0 0.0 1</ambient>
                        <diffuse>1.0 0.0 0.0 1</diffuse>
                        <specular>1.0 0.0 0.0 1</specular>
                    </material>
                </visual>
                <collision name='collision'>
                    <geometry>
                        <cylinder>
                            <radius>0.4</radius>
                            <length>0.2</length>
                        </cylinder>
                    </geometry>
                </collision>
            </link>

            <!--插入框架-->
            <frame name="caster_frame" attached_to='chassis'>
                <pose>0.8 0 -0.2 0 0 0</pose>
            </frame>

            <!--caster wheel-->
            <link name='caster'>
                <pose relative_to='caster_frame'/>
                <inertial>
                    <mass>1</mass>
                    <inertia>
                        <ixx>0.1</ixx>
                        <ixy>0</ixy>
                        <ixz>0</ixz>
                        <iyy>0.1</iyy>
                        <iyz>0</iyz>
                        <izz>0.1</izz>
                    </inertia>
                </inertial>
                <visual name='visual'>
                    <geometry>
                        <sphere>
                            <radius>0.2</radius>
                        </sphere>
                    </geometry>
                    <material>
                        <ambient>0.0 1 0.0 1</ambient>
                        <diffuse>0.0 1 0.0 1</diffuse>
                        <specular>0.0 1 0.0 1</specular>
                    </material>
                </visual>
                <collision name='collision'>
                    <geometry>
                        <sphere>
                            <radius>0.2</radius>
                        </sphere>
                    </geometry>
                </collision>
            </link>


            <!--connecting these links together using joints-->
            <joint name='left_wheel_joint' type='revolute'> <!--continous joint is not supported yet-->
                <pose relative_to='left_wheel'/>
                <parent>chassis</parent>
                <child>left_wheel</child>
                <axis>
                    <xyz expressed_in='__model__'>0 1 0</xyz> <!--can be defined as any frame or even arbitrary frames-->
                    <limit>
                        <lower>-1.79769e+308</lower>    <!--negative infinity-->
                        <upper>1.79769e+308</upper>     <!--positive infinity-->
                    </limit>
                </axis>
            </joint>

            <joint name='right_wheel_joint' type='revolute'>
                <pose relative_to='right_wheel'/>
                <parent>chassis</parent>
                <child>right_wheel</child>
                <axis>
                    <xyz expressed_in='__model__'>0 1 0</xyz>
                    <limit>
                        <lower>-1.79769e+308</lower>    <!--negative infinity-->
                        <upper>1.79769e+308</upper>     <!--positive infinity-->
                    </limit>
                </axis>
            </joint>

            <!--different type of joints ball joint--> <!--defult value is the child-->
            <joint name='caster_wheel' type='ball'>
                <parent>chassis</parent>
                <child>caster</child>
            </joint>

            <!--diff drive plugin-->
            <plugin
                    filename="gz-sim-diff-drive-system"
                    name="gz::sim::systems::DiffDrive">
                <left_joint>left_wheel_joint</left_joint>
                <right_joint>right_wheel_joint</right_joint>
                <wheel_separation>1.2</wheel_separation>
                <wheel_radius>0.4</wheel_radius>
                <odom_publish_frequency>1</odom_publish_frequency>
                <topic>cmd_vel</topic>
            </plugin>
        </model>

        <!--使小车可以用键盘操作移动    -->
        <!-- Moving Left-->
        <plugin filename="gz-sim-triggered-publisher-system"
                name="gz::sim::systems::TriggeredPublisher">
            <input type="gz.msgs.Int32" topic="/keyboard/keypress">
                <match field="data">16777234</match>
            </input>
            <output type="gz.msgs.Twist" topic="/cmd_vel">
                linear: {x: 0.0}, angular: {z: 0.5}
            </output>
        </plugin>
        <!-- Moving Forward-->
        <plugin filename="gz-sim-triggered-publisher-system"
                name="gz::sim::systems::TriggeredPublisher">
            <input type="gz.msgs.Int32" topic="/keyboard/keypress">
                <match field="data">16777235</match>
            </input>
            <output type="gz.msgs.Twist" topic="/cmd_vel">
                linear: {x: 0.5}, angular: {z: 0.0}
            </output>
        </plugin>
        <!-- Moving Right-->
        <plugin filename="gz-sim-triggered-publisher-system"
                name="gz::sim::systems::TriggeredPublisher">
            <input type="gz.msgs.Int32" topic="/keyboard/keypress">
                <match field="data">16777236</match>
            </input>
            <output type="gz.msgs.Twist" topic="/cmd_vel">
                linear: {x: 0.0}, angular: {z: -0.5}
            </output>
        </plugin>
        <!-- Moving Backward-->
        <plugin filename="gz-sim-triggered-publisher-system"
                name="gz::sim::systems::TriggeredPublisher">
            <input type="gz.msgs.Int32" topic="/keyboard/keypress">
                <match field="data">16777237</match>
            </input>
            <output type="gz.msgs.Twist" topic="/cmd_vel">
                linear: {x: -0.5}, angular: {z: 0.0}
            </output>
        </plugin>
    </world>
</sdf>
```

将其载入到 Gazebo 中，然后执行：

```shell
gz topic -t "/cmd_vel" -m gz.msgs.Twist -p "linear: {x: 0.5}, angular: {z: 0.05}"
```

> `/cmd_vel`是 Gazebo 中用于向移动机器人发送速度指令的标准主题，它通常使用`gz.msgs.Twist`类型的消息来传递机器人的线速度和角速度。ROS 的`/cmd_vel`作用和 Gazebo 的相同。
>
> 文件中有一个`<odom_publish_frequency>`标签，用于定义`odom`数据的发布频率。

> `odom`，里程计坐标系，这里主要区分，`odom topic`，这是两个概念，一个是坐标系，一个是根据编码里程计计算得到的里程计信息。但是两者也有关系，`odom topic`转化得到的位姿矩阵是`odom->base_link`的`tf`关系。这时可能会有疑问：
> 
> ①位姿矩阵得到的不应该是`map->base_link`的`tf`关系吗？
> 
> ②map坐标系与`odom`坐标系又有什么关系？
> 
> 其实机器人刚开始运动时，`odom`坐标系与`map`坐标系是重合的，即`odom->base_link`与`map->base_link`的`tf`是一致的。但是随着时间的推移，出现了偏差，出现的偏差也就是里程计的累计误差，如果里程计精确度很高，没有计算误差，那么`map`坐标系与`odom`坐标系会一直是重合的。
> 
> 然而实际情况中，里程计是有偏差的，所以我们计算里程计得到的位姿矩阵是`odom->base_link`，与`map->base_link`还是有偏差的。
>
> ③那么`map->odom`的`tf`是怎么得到的呢？
> 
> 除去里程计还有一些合作校正的传感器例如imu，这些传感器可以估计出机器人在地图中的位置也就是`map->base_link`的`tf`，我们估计机器人在地图中的位置和解算里程计得到的位置的偏差也就是`map->odom`的`tf`。
> 
> 出自[此处](https://blog.csdn.net/weixin_46181372/article/details/109694300)

然后，小车就会在平地上缓慢地转圈：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762585092871-db8c89ac-ac06-409f-9f6b-6ba2cd2055e3.png)

如果在小车前面放一个箱子的话，小车会推开箱子：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762585717929-3ed04881-d051-47f6-af4b-1c89ae6a9ce1.png)

在左上角菜单栏里可以保存世界：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762585830265-602c664a-33ed-4afb-a840-3a3cdee0f789.png)

退出之后重新载入`dwb.sdf`，小车和箱子的位置不变，但小车不会转圈了，再次输入上面的命令才能让小车继续运动。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762586111880-c8e402e5-07bb-4abc-88f8-fe8f634be386.png)

# 插入传感器

## 图像传感器

图像传感器（IMU）以四元数的形式输出机器人的方向`orientation`，以(X, Y, Z)的形式输出角速度`angular_velocity`，以以(X, Y, Z)的形式输出线速度`linear_acceleration`。我把`moving_robot.sdf`的代码复制到新建的`sensor_robot.sdf`，在这个新文件内进行添加传感器的实践。在添加图像传感器之前，先注入一个插件：

```xml
<plugin filename="gz-sim-imu-system"
name="gz::sim::systems::Imu">
</plugin>
```

然后，添加图像传感器：

```xml
<sensor name="imu_sensor" type="imu">
    <always_on>1</always_on>
    <update_rate>1</update_rate>
    <visualize>true</visualize>
    <topic>imu</topic>
</sensor>
```

这个传感器通常添加到模型的链接，我将其添加到小车机壳`chassis`链接标签下。

该传感器的各个标签如下：

- `<always_on>`: 如果为真（`1`），传感器将根据更新速率进行持续性的更新
- `<update_rate>`: 更新速率，生成传感器数据的频率
- `<visualize>`: 如果为真，传感器将在图形界面显示
- `<topic>`: 数据发布的主题名称

> 注意：不是所有传感器都支持所有标签。

# 从图像传感器读取数据

载入世界并启用仿真：

```shell
gz sim sensor_robot.sdf
```

在另一个终端页面运行：
```shell
gz topic -e -t /imu
```

后一个命令监听发送到`/imu`主题的消息。图像传感器的数据是四元数，`angular_velocity` `linear_acceleration`是(X, Y, Z)：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762661015286-03d572ea-7683-45bd-bb89-431c476004a4.png)

移动小车，传感器的数据发生了变化。

## 接触传感器

我之后引入了接触传感器。顾名思义，这种传感器在接触到物体之后会发出信号。我搭建了一个墙，并在小车上添加了一个接触传感器。当传感器接触到障碍也就是墙时，小车会停止移动，以防止其受到损伤。首先搭建一个墙的模型：

```xml
<model name='wall'>
    <static>true</static>
    <pose>5 0 0 0 0 0</pose><!--pose relative to the world-->
    <link name='box'>
        <visual name='visual'>
            <geometry>
                <box>
                    <size>0.5 10.0 2.0</size>
                </box>
            </geometry>
            <!--let's add color to our link-->
            <material>
                <ambient>0.0 0.0 1.0 1</ambient>
                <diffuse>0.0 0.0 1.0 1</diffuse>
                <specular>0.0 0.0 1.0 1</specular>
            </material>
        </visual>
        <collision name='collision'>
            <geometry>
                <box>
                    <size>0.5 10.0 2.0</size>
                </box>
            </geometry>
        </collision>
    </link>
</model>
```

重载世界，墙出现在世界中：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762661618258-dceb82d1-1246-41cb-b263-f482a822ca4a.png)

在墙上添加碰撞传感器。和图像传感器一样，都需要先注入插件：

```xml
<plugin filename="gz-sim-contact-system"
name="gz::sim::systems::Contact">
</plugin>
```

在墙的链接下添加碰撞传感器：

```xml
<sensor name='sensor_contact' type='contact'>
    <contact>
        <collision>collision</collision>
    </contact>
</sensor>
```

这个传感器的定义更简单，只需定义传感器的名称和传感器类型，再定义碰撞的名称`collision`即可。

我们也需要在墙模型下添加`TouchPlugin`插件：

```xml
<plugin filename="gz-sim-touchplugin-system"
name="gz::sim::systems::TouchPlugin">
<target>vehicle_blue</target>
<namespace>wall</namespace>
<time>0.001</time>
<enabled>true</enabled>
</plugin>
```

这个插件在墙被接触的时候发布消息。插件的标签如下：

- `<target>`: 定义和墙相联系的模型，也就是小车`vehicle_blue`
- `<namespace>`: 指定主题的命名空间，这样当小车碰撞到墙上的时候，墙会在`/wall/touched`主题上发布消息

重载世界并启用仿真，然后监听`/wall/touched`主题：

```shell
gz topic -e -t /wall/touched
```

向前移动小车，当小车撞到墙上的时候，会监听到`data: true`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762664486359-c4263ada-f7df-4c8b-8d76-b0052e0bf581.png)

可以使用`TriggeredPublisher`插件使小车在撞到墙上的时候停止运动：
```xml
<plugin filename="gz-sim-triggered-publisher-system"
    name="gz::sim::systems::TriggeredPublisher">
    <input type="gz.msgs.Boolean" topic="/wall/touched">
        <match>data: true</match>
    </input>
    <output type="gz.msgs.Twist" topic="/cmd_vel">
        linear: {x: 0.0}, angular: {z: 0.0}
    </output>
</plugin>
```

这个插件根据收到的输入信号发布输出消息。因而当收到`data: true`消息时，发布`linear: {x: 0.0}, angular: {z: 0.0}`使小车停下来。

## 雷达

该传感器可以测量小车和墙之间的距离，可以用来防止小车撞墙。

首先，在`vehicle_blue`模型标签内创建雷达框架，并将其安装在小车机壳`chassis`上。

```xml
<frame name="lidar_frame" attached_to='chassis'>
    <pose>0.8 0 0.5 0 0 0</pose>
</frame>
```

然后在`<world>`标签下注入插件：

```xml
<plugin
    filename="gz-sim-sensors-system"
    name="gz::sim::systems::Sensors">
    <render_engine>ogre2</render_engine>
</plugin>
```

在`chassis`链接标签下添加雷达传感器：

```xml
<sensor name='gpu_lidar' type='gpu_lidar'>"
    <pose relative_to='lidar_frame'>0 0 0 0 0 0</pose>
    <topic>lidar</topic>
    <update_rate>10</update_rate>
    <ray>
        <scan>
            <horizontal>
                <samples>640</samples>
                <resolution>1</resolution>
                <min_angle>-1.396263</min_angle>
                <max_angle>1.396263</max_angle>
            </horizontal>
            <vertical>
                <samples>1</samples>
                <resolution>0.01</resolution>
                <min_angle>0</min_angle>
                <max_angle>0</max_angle>
            </vertical>
        </scan>
        <range>
            <min>0.08</min>
            <max>10.0</max>
            <resolution>0.01</resolution>
        </range>
    </ray>
    <always_on>1</always_on>
    <visualize>true</visualize>
</sensor>
```

首先，定义传感器的名称和类型，然后定义`<pose>`标签，使其与`lidar_frame`关联。

在`<topic>`标签内，定义雷达数据发布的主题。

`<update_rate>`是雷达数据生成的频率，单位Hz。

在`<horizontal>` `<horizontal>`标签下，定义水平和垂直激光的属性：

- `<samples>`: 样本数是是每个完整激光扫描周期要生成的模拟激光雷达光线的数量
- `<resolution>`: 将这个数字乘以样本数，即可确定数据点的数量范围
- `<min_angle>` `<max_angle>`: 生成光线的角度范围

在`<range>`下，定义模拟光线的属性：

- `<min>` `<max>`: 定义光线的最小和最大距离
- `<resolution>`: 定义每条激光雷达光线的线性分辨率

另外：

- `<always_on>`: 如果为真（`1`），传感器将根据更新速率`<update_rate>`进行持续性的更新
- `<visualize>`: 如果为真，传感器将在图形界面显示

重载世界，启用仿真，然后监听`/lider`主题：

```shell
gz topic -e -t /lidar
```

> 返回的消息具有以下属性：

```text
message LaserScan
{
Header header              = 1;

string frame               = 2;
Pose world_pose            = 3;
double angle_min           = 4;
double angle_max           = 5;
double angle_step          = 6;
double range_min           = 7;
double range_max           = 8;
uint32 count               = 9;
double vertical_angle_min  = 10;
double vertical_angle_max  = 11;
double vertical_angle_step = 12;
uint32 vertical_count      = 13;

repeated double ranges              = 14;
repeated double intensities         = 15;
}
```

如果要避免小车撞墙的话，则需要写一个节点。该节点订阅`/lidar`并发布控制小车的指令消息。

## 编写节点

ROS和Gazebo的节点默认是不互通的，如果要进行两种节点间的通信的话，需要借助`ros_gz_bridge`，详见[此处](https://docs.ros.org/en/rolling/p/ros_gz_bridge/__README.html#bridge-communication-between-ros-and-gazebo)。

这里使用Gazebo的节点编写方式。

在`CMakeLists.txt`中写入：

```cmake
find_package(gz-transport13 QUIET REQUIRED OPTIONAL_COMPONENTS log)
set(GZ_TRANSPORT_VER ${gz-transport13_VERSION_MAJOR})
# ...
add_executable(avoid_wall src/AvoidWall.cpp)
target_link_libraries(avoid_wall gz-transport${GZ_TRANSPORT_VER}::core)
```

声明向`cmd_vel`主题发布信息的节点：

```c++
std::string topic_pub = "/cmd_vel";
gz::transport::Node node;
auto pub = node.Advertise<gz::msgs::Twist>(topic_pub);
```

节点需要执行的方法：

```c++
void cb(const gz::msgs::LaserScan &_msg) {
    gz::msgs::Twist data;

    bool allMore = true;
    for (int i = 0; i < _msg.ranges_size(); i++) {
        if (_msg.ranges(i) < 1.0) {
            allMore = false;
            break;
        }
    }
    if (allMore) { // 如果所有光线的范围都大于1.0
        data.mutable_linear()->set_x(0.5);
        data.mutable_angular()->set_z(0.0);
    } else {
        data.mutable_linear()->set_x(0.0);
        data.mutable_angular()->set_z(0.5);
    }

    pub.Publish(data);
}
```

在这里，这个回调方法检查所有光线的范围是否大于`1.0`，如果是，向小车发布向前进的消息；如果否，则令小车旋转。

```c++
int main(int argc, char **argv)
{
std::string topic_sub = "/lidar";
// 订阅主题，然后执行回调方法
if (!node.Subscribe(topic_sub, cb))
{
std::cerr << "Error subscribing to topic [" << topic_sub << "]" << std::endl;
return -1;
}

    // 等待
    gz::transport::waitForShutdown();

    return 0;
}
```

在主方法中，订阅了`lidar`主题，并直到节点停止运行之前保持等待。

然后定位到输出可执行程序的目录下，运行节点：

```shell
./avoid_wall
```

重载世界，并启用仿真，然后就能看到一个小车向前移动，当接近墙的时候，小车会停止前进，一直向左转，直到前方不再有障碍为止，之后继续前进。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762676657942-b76d83bc-ed85-4483-be95-794e1ce245ce.png)

## Gazebo节点启动文件

和 ROS2 一样，Gazebo 节点也有类似的一键启动多个节点的`.gzlaunch`文件。和ROS2的`launch.py`不同，`.gzlaunch`文件用的是XML。

```xml
<?xml version='1.0'?>
<gz version='1.0'>
  <executable name='sensor-world'>
    <command>gz sim sensor_tutorial.sdf</command>
  </executable>

  <executable name='lidar_node'>
    <command>./build/lidar_node</command>
  </executable>

</gz>
```

运行以下命令以启动（如果文件名称是`sensor_launch.gzlaunch`）：

```shell
gz launch sensor_launch.gzlaunch
```

# `rviz`中订阅 Gazebo 话题

参照[此处](https://github.com/gazebosim/ros_gz/tree/jazzy)安装`ros_gz`。

这里实践一下订阅激光雷达的话题：

用`launch.py`启动 Gazebo：

```shell
ros2 launch ros_gz_sim gz_sim.launch.py gz_args:=sensor_robot.sdf
```

`ros_gz_sim`还提供了仅运行 Gazebo 服务器的无 GUI 的`launch.py`：

```shell
ros2 launch ros_gz_sim gz_server.launch.py world_sdf_file:=sensor_robot.sdf
```

然后，手动桥接 ROS2 节点和 Gazebo 节点：

```shell
ros2 run ros_gz_bridge parameter_bridge /lidar@sensor_msgs/msg/LaserScan@gz.msgs.LaserScan
```
运行`rviz`之后，设定全局`map`：

```shell
ros2 run tf2_ros static_transform_publisher 0.0 0.0 0.0 0.0 0.0 0.0 map 100
```

按话题`/lidar`添加`LaserScan`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1762684046014-a40a2b5a-4456-4545-8b7a-996c254a57c8.png)

之后，在 Gazebo 启用仿真，接着在运行`rviz`的窗口可以看到激光雷达扫描的点云图了。

[语雀笔记页面](https://www.yuque.com/u29112212/ucgrla/ghg3fa74k9k4xbga)