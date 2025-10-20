<h1 id="Y30eC">理解 CMake</h1>

> CMake 是个一个开源的跨平台自动化建构系统，用来管理软件建置的程序，并不依赖于某特定编译器，并可支持多层目录、多个应用程序与多个函数库。
>
> CMake 通过使用简单的配置文件 CMakeLists.txt，自动生成不同平台的构建文件（如 Makefile、Ninja 构建文件、Visual Studio 工程文件等），简化了项目的编译和构建过程。
>
> CMake 本身不是构建工具，而是生成构建系统的工具，它生成的构建系统可以使用不同的编译器和工具链。
>
> 出自[runoob.com](https://www.runoob.com/cmake/cmake-tutorial.html)
>

在我看来，一个项目的结构原本在编译器的眼里是杂乱无章的，而CMake的作用正是将项目的结构整合成能够被编译器识别的对象。`CMakeList.txt`文件相当于项目给CMake分配的“向导”，引领CMake对项目进行构建。不仅如此，CMake还可以引入外部库，比如OpenCV就需要通过CMake与项目链接。

<h1 id="THjVR">编写CMakeList.txt</h1>
<h2 id="u4be8">基本指令</h2>

> 1、指定 CMake 的最低版本要求：

```cmake
cmake_minimum_required(VERSION <version>)
```

> 例如：
>

```cmake
cmake_minimum_required(VERSION 3.10)
```

> 2、定义项目的名称和使用的编程语言：
>

```cmake
project(<project_name> [<language>...])
```

> 例如：
>

```cmake
project(MyProject CXX)
```

> 3、指定要生成的可执行文件和其源文件：
>

```cmake
add_executable(<target> <source_files>...)
```

> 例如：
>

```cmake
add_executable(MyExecutable main.cpp other_file.cpp)
```

> 4、创建一个库（静态库或动态库）及其源文件：
>

```cmake
add_library(<target> <source_files>...)
```

> 例如：
>

```cmake
add_library(MyLibrary STATIC library.cpp)
```

> 5、链接目标文件与其他库：
>

```cmake
target_link_libraries(<target> <libraries>...)
```

> 例如：
>

```cmake
target_link_libraries(MyExecutable MyLibrary)
```

> 6、添加头文件搜索路径：
>

```cmake
include_directories(<dirs>...)
```

> 例如：
>

```cmake
include_directories(${PROJECT_SOURCE_DIR}/include)
```

> 7、设置变量的值：
>

```cmake
set(<variable> <value>...)
```

> 例如：
>

```cmake
set(CMAKE_CXX_STANDARD 11)
```

> 8、指定头文件搜索路径：
>

```cmake
target_include_directories(TARGET target_name
                          [BEFORE | AFTER]
                          [SYSTEM] [PUBLIC | PRIVATE | INTERFACE]
                          [items1...])
```

> 例如：
>

```cmake
target_include_directories(MyExecutable PRIVATE ${PROJECT_SOURCE_DIR}/include)
```

> 9、安装规则：
>

```cmake
install(TARGETS target1 [target2 ...]
        [RUNTIME DESTINATION dir]
        [LIBRARY DESTINATION dir]
        [ARCHIVE DESTINATION dir]
        [INCLUDES DESTINATION [dir ...]]
        [PRIVATE_HEADER DESTINATION dir]
        [PUBLIC_HEADER DESTINATION dir])
```

> 例如：
>

```cmake
install(TARGETS MyExecutable RUNTIME DESTINATION bin)
```

> 10、条件语句 (`if` `elseif` `else` `endif`命令)
>

```cmake
if(expression)
# Commands
elseif(expression)
# Commands
else()
# Commands
endif()
```

> 例如：
>

```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Debug build")
endif()
```

> 11、自定义命令 (`add_custom_command`命令)：
>

```cmake
add_custom_command(
   TARGET target
   PRE_BUILD | PRE_LINK | POST_BUILD
   COMMAND command1 [ARGS] [WORKING_DIRECTORY dir]
   [COMMAND command2 [ARGS]]
   [DEPENDS [depend1 [depend2 ...]]]
   [COMMENT comment]
   [VERBATIM]
)
```

> 例如：
>

（构建结束后输出一句话。）

```cmake
add_custom_command(
   TARGET MyExecutable POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E echo "Build completed."
)
```

> 出自[runoob.com](https://www.runoob.com/cmake/cmake-basic.html)
>

这是CLion在我的项目里自动生成的`CMakeLists.txt`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760916436707-0275885b-68c3-4897-b74f-c8e26721cc42.png)

其中：

+ `cmake_minimum_required(VERSION 4.0)`: 指定CMake版本最低要求为`4.0`
+ `project(EncryptApplication)`: 定义项目的名称`EncryptApplication`
+ `set(CMAKE_CXX_STANDARD 23)`: 设置变量`C++标准`的值为`23`
+ `add_executable(CryptoTool main.cpp crypto_tool.cpp crypto_tool.h)`: 指定要生成的文件及其源文件

我的`CryptoTool`最初用的语言标准是C++20,后来通过改变`CMAKE_CXX_STANDARD`的值改成了C++23。在我看来，C++23会更好用一些。比如C++23相对于C++20，`cstring`库多了一个`std::string::contain()`方法。根据编辑器的代码优化提示，这个方法的效率比`std::string::find() != std::string::empty()`高。

<h2 id="FqGCw">变量和缓存</h2>
CMake可以使用变量来存储和传递信息，这些变量可以在`CMakeLists.txt`文件中定义和使用。变量可分为普通变量和缓存变量。

普通变量定义：

```cmake
set(MY_VAR "Hello World")
```

普通变量使用：

```cmake
message(STATUS "Variable MY_VAR is ${MY_VAR}")
```

缓存变量存储在 CMake 的缓存文件中，用户可以在 CMake 配置时修改这些值。缓存变量通常用于用户输入的设置，例如编译选项和路径。

缓存变量定义：

```cmake
set(MY_CACHE_VAR "DefaultValue" CACHE STRING "A cache variable")
```

缓存变量使用：

```cmake
message(STATUS "Cache variable MY_CACHE_VAR is ${MY_CACHE_VAR}")
```

<h2 id="IbDUo">查找库和包</h2>

CMake 可以通过`find_package()`指令自动检测和配置外部库和包。

常用于查找系统安装的库或第三方库。

基本用法：

```cmake
find_package(Boost REQUIRED)
```

指定版本：

```cmake
find_package(Boost 1.70 REQUIRED)
```

查找库并指定路径：

```cmake
find_package(OpenCV REQUIRED PATHS /path/to/opencv)
```

链接查找到的库：

```cmake
target_link_libraries(MyExecutable Boost::Boost)
```

设置包含目录和链接目录：

```cmake
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
```

比如：我要给我的项目添加OpenCV库，那么就需要加上（链接目录那一行要放在添加源文件那一行之后）（OpenCV的编译和安装放在后面说）：

```cmake
find_package(OpenCV REQUIRED)
target_link_libraries(CryptoTool ${OpenCV_LIBS})
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760919146412-908f500a-f5e3-4696-b231-c85fe3b33fdf.png)

第一行代码有什么用呢？根据[此处](https://cmake.org/cmake/help/latest/command/find_package.html)的说法：

> Find a package (usually provided by something external to the project), and load its package-specific details.
>

简而言之就是寻找指定的包，并加载找到的包。而`REQUIRED`则说明这个包是必需的，如果缺省，就会报错而不会进行构建。

> `include_directories()`和`target_include_directories()`：
>
> 在 CMake 中，`include_directories()`和`target_include_directories()`都用于指定头文件的搜索路径，但它们的作用范围和使用方式有显著区别。
>
> 相同点：
>
> + 两者都用于添加头文件的搜索路径，编译器会在这些路径中查找`#include`指令中指定的头文件。
> + 两者都支持绝对路径和相对路径，相对路径是相对于当前`CMakeLists.txt `文件所在的目录。
> + 两者都可以用于指定公共头文件路径`PUBLIC`、私有头文件路径`PRIVATE`或接口头文件路径`INTERFACE`。
>
> 区别：
>

| 特性          | `include_directories()` | `target_include_directories()`                 |
|-------------|-------------------------|------------------------------------------------|
| 作用范围        | 全局作用域，影响所有目标（target）。   | 仅作用于指定的目标（target）。                             |
| 推荐使用场景      | 适用于简单的项目或旧版 CMake 项目。   | 适用于现代 CMake 项目，推荐优先使用。                         |
| 目标关联性       | 不直接关联到特定目标，可能影响所有目标。    | 显式关联到特定目标，避免污染其他目标。                            |
| 可维护性        | 较差，容易导致全局路径污染。          | 较好，路径与目标绑定，逻辑清晰。                               |
| 作用域控制       | 无法精确控制路径的作用范围。          | 可以通过`PUBLIC` `PRIVATE` `INTERFACE`精确控制路径的作用范围。 |
| 现代 CMake 推荐 | 不推荐使用，除非有特殊需求。          | 推荐使用，符合现代 CMake 的最佳实践。                         |


> 出自[runoob.com](https://www.runoob.com/cmake/cmake-basic.html)
>

在我看来，可以说：在包含目录着一块，`include_directories()`已经退环境了，现在占主场的是`target_include_directories()`。

<h2 id="Ts46v">install() 指令配置项目安装路径</h2>
谷歌搜索AI的返回结果如下：

```cmake
# 安装目标 my_target 到指定路径下的库目录
install(TARGETS my_target DESTINATION lib)

# 安装一个可执行文件到指定路径下的 bin 目录
install(PROGRAMS my_executable DESTINATION bin)

# 安装一个配置文件到指定路径下的 share/my_project 目录
install(FILES config.xml DESTINATION share/my_project)
```

> `DESTINATION`的实际路径是`CMAKE_INSTALL_PREFIX`加上`DESTINATION`参数的值。
>
> 例如，如果`CMAKE_INSTALL_PREFIX`被设置为`/usr/local`，那么 `install(TARGETS my_target DESTINATION lib)`将会把`my_target`安装到 `/usr/local/lib`。
>

`CMAKE_INSTALL_PREFIX`是`cmake`命令的`-D`选项的一个变量。下文在自行构建OpenCV时将会用到。

<h1 id="W2YAp">CMake 构建</h1>
CMake推荐将构建文件放在源代码目录之外的独立目录中，这样可以保持源代码目录的整洁，并方便管理不同的构建配置。

新建并进入`build`文件夹：

```shell
mkdir build
cd build
```

随后使用CMake生成构建文件：

```shell
cmake ..
```

如需指定编译器，可以使用`-G`选项：

```shell
cmake -G "Ninja" ..
```

如需指定构建类型，可以使用`-DCMAKE_BUILD_TYPE`选项：

```shell
cmake -DCMAKE_BUILD_TYPE=Release ..
```

CMake 会输出配置过程中的详细信息，包括找到的库、定义的选项等，如果没有错误，构建系统文件将被生成到构建目录中。

然后，使用生成的构建文件进行编译和构建：

```shell
make [NAME] # 使用Makefile或类似构建系统，其中NAME是可选的，如无需构建特定目标，可以留空
ninja [NAME] # 使用Ninja，其中NAME是可选的，如无需构建特定目标，可以留空
msbuild MyProject.sln /p:Configuration=Release # 使用 Visual Studio
```

`-jN`是一个可用选项，代表使用`N`个线程进行编译，比如`-j8`。

编译完成后，可以清理构建文件：

```shell
make clean # 使用 Makefile
ninja clean # 使用 Ninja
rm -rf build/* # 手动删除
```

<h2 id="r0alH">尝试自行构建OpenCV</h2>
我尝试着构建OpenCV 4.12源码：

> 参考了[此处](https://github.com/milq/milq/blob/master/scripts/bash/install-opencv.sh)。
>

在[此处](https://opencv.org/releases/)下载源码压缩文件，解压到OpenCV文件中。

```shell
unzip opencv-4.12.0.zip
mv opencv-4.12.0 OpenCV
cd OpenCV
```

进入`build`文件夹：（这个目录默认已存在）

```shell
cd build
```

构建：

```shell
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_GENERATE_PKGCONFIG=YES ..
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760922949888-802bb6f1-2bed-4629-9651-9f184877d502.png)

+ `CMAKE_BUIED_TYPE`: 值为`RELEASE`时，启用优化和禁用调试信息
+ `CMAKE_INSTALL_PREFIX`: 安装路径，默认值为`/usr/local`（在这条命令的这一选项其实可以省去）
+ `OPENCV_GENERATE_PKGCONFIG`: 生成`.pc`文件，以便其他程序可以方便地使用OpenCV库

编译并安装：

```shell
make -j32 # 用32个线程编译，不到10分钟就能完成
sudo make install
sudo ldconfig
```

回到我的项目，现在可以在头文件里包含OpenCV库了：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760924595908-03e7c8b4-1b52-4ea6-b59a-4c9fcaebaf6c.png)

新建一个项目，配好`CMakeLists.txt`，把OpenCV自带的`example.cpp`的代码复制过来，构建、编译，然后运行一下：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760935782374-a55701b1-1b88-486a-a706-54a78fdb59f7.png)

出现摄像页面，说明OpenCV安装成功。

[语雀笔记页面](https://www.yuque.com/u29112212/ucgrla/omwnvh62afkgnosp)