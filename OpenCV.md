1. 验证GCC、CMake已经安装。  
![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760360051609-cb081de0-3c85-40c7-9013-9cc3f2d9e149.png)
2. 前往[OpenCV官网](about:blank)下载源码，然后解压缩。  
![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760360602095-b766fa89-00ad-439d-ae3c-170cb18d8532.png)
3. 在opencv文件夹下新建build文件夹，转到该文件夹中。

```plain
cd opencv
mkdir build
cd build
```

4. 用CMake配置OpenCV：

```plain
cmake -D CMAKE_BUILD_TYPE=Release -D OPENCV_GENERATE_PKGCONFIG=YES ..
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760361554575-d76924d9-cac5-46ec-be96-2abe578c5741.png)

5. 进行编译，这里使用8核。这一过程需要二十多分钟。

```plain
make -j8
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760363223724-06faecef-09a0-43e1-bc3a-b0ff44dc90cd.png)

6. 随后整合进install文件夹中。

```plain
sudo make install
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760363385957-c396985d-f77e-4a10-b276-9151784f2286.png)

7. 设置环境变量。  
vim中按i进入输入模式，编辑完成后按esc退出输入模式，再输入:wq保存并退出。

```plain
sudo vim /etc/bash.bashrc
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig #在文件末尾加入
:wq
source /etc/bash.bashrc
```

8. 验证其成功添加至环境变量。  
![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760365844098-2ed15c06-9295-48c3-8390-0e85aad28fa2.png)
9. 配置动态库环境。新建文件/etc/ld.so.conf.d/opencv4.conf，在文件末尾加入OpenCV的lib路径，保存并退出。

```plain
sudo vim /etc/ld.so.conf.d/opencv4.conf
/usr/local/lib #输入这一行
:wq
sudo ldconfig
```



