<h1 id="MrC71">理解 Docker</h1>
<h2 id="dCyoJ">镜像</h2>

> 镜像是一个只读的模板，包含了运行应用所需的所有内容：代码、运行时、库文件、环境变量和配置文件。镜像就像是一个安装程序或者模板，它定义了应用运行所需的一切，但本身不能直接运行。
>
> 出自[Runoob.com](https://www.runoob.com/docker/docker-intro.html)
>

在我看来，镜像在虚拟机中的地位和在Docker中的地位是相近的。

虚拟机镜像体积庞大，一般是GB级别；Docker镜像体积相对小，一般是MB级别。这是因为前者的镜像中包含了一个完整的操作系统，依靠Hyper-V技术运行。后者则依靠主机上的操作系统运行，因而不包含一个完整操作系统。

<h2 id="okxqL">容器</h2>
> 容器是镜像的运行实例，是一个轻量级、可移植的执行环境。
>
> 出自[Runoob.com](https://www.runoob.com/docker/docker-intro.html)
>

观察Docker的Logo：一只鲸鱼驮着若干像集装箱一样的小方块。如果鲸鱼代表着主机和Docker的话，那么容器就是那些小方块。如果将镜像比作面向对象编程的高级语言中的类，那么容器就是其对象实例。和虚拟机相似，一个镜像可以创建多个容器。

<h2 id="yX9UW">仓库</h2>
> 仓库是存储和分发镜像的地方，可以包含一个镜像的多个版本。
>
> 出自[Runoob.com](https://www.runoob.com/docker/docker-intro.html)
>

和Git仓库的性质相近，只不过Docker仓库存储和分发的是镜像，Git仓库存储和分发的是代码。

<h1 id="oaqAq">准备工作</h1>
<h2 id="L2OfT">Docker 的安装和配置</h2>

执行以下命令安装Docker Engine：（来自[此处](https://www.runoob.com/docker/ubuntu-docker-install.html)）

```plain
# 下载并执行Docker官方安装脚本
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh

# 启动Docker服务
sudo systemctl start docker
sudo systemctl enable docker
```

尝试拉取一个镜像：

```plain
docker pull ubuntu
```

返回如下：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760794202392-afa08bc7-85df-4e8a-b623-8abe6af70d02.png)

由[此处](https://blog.csdn.net/BaoITcore/article/details/127736052)得知，默认情况下，docker 命令会使用Unix socket 与 Docker 引擎通讯。而只有`root`组和`docker`组的用户才可以访问 Docker 引擎的。因此解决方案便是将当前用户加入`docker`组中，然后更新`docker`组。

```plain
sudo gpasswd -a username docker && newgrp docker
```

之后，我想到镜像拉取要和Docker Hub连接，然而Docker Hub受到防火长城的影响，拉取镜像非常缓慢，而国内镜像据说又不是很稳定，于是只好使用梯子绕过防火长城。

> docker pull /push 的代理被 systemd 接管，所以需要设置 systemd...
>
> 版权声明：本文为 neucrack 的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
>
> 原文链接：[https://neucrack.com/p/286](https://neucrack.com/p/286)
>

执行以下命令：

```plain
sudo mkdir -p /etc/systemd/system/docker.service.d
sudo vim /etc/systemd/system/docker.service.d/http-proxy.conf
```

在`http-proxy.conf`中，加入以下内容：

```yaml
[Service]
Environment="HTTP_PROXY=http://127.0.0.1:10808"
Environment="HTTPS_PROXY=http://127.0.0.1:10808"
```

现在，docker pull/push就可以走梯子的代理了。但是build和容器仍然走不了。根据[此处](https://docs.docker.com/engine/cli/proxy/)的说法，安装Docker Desktop来设置会方便一些。于是我安装了Docker Desktop。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760799578059-32553311-291b-468a-8bf7-129993d84d86.png)

可以在这里设置host模式，使容器在运行时与宿主机共享网络。也就是说，只要电脑上运行着梯子，容器就可以访问Github

现在拉取Ubuntu镜像并创建一个容器试试：

```plain
docker pull ubuntu
docker run -it ubuntu /bin/bash
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760800129348-b3f368be-4c03-49b9-937f-ff71e4886aa6.png)

创建容器成功。输入`exit`可以关闭容器终端，此时容器会停止运行。

> `/bin/bash`: 放在镜像名后的是命令，这里我们希望有个交互式 Shell，因此用的是`/bin/bash`。
>
> 出自[runoob.com](https://www.runoob.com/docker/docker-container-usage.html)
>

注意：如果没有`/bin/bash`的话，Ubunntu容器默认使用`/bin/sh`，而`/bin/sh`并不是交互式Shell——也就是说，这个容器一运行就会因为没有可执行的命令而结束。即便用`docker start -i`启动，将预设的命令执行完后也会自动结束，不给用户任何的输入机会。

<h1 id="jYjyB">终端命令</h1>

这些命令有不少都已经集成到了Docker Desktop的图形界面中。

<h2 id="ImPjE">容器生命周期的管理</h2>
<h3 id="VHRkE">run</h3>

创建并运行一个容器：

```plain
docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
```

可用选项：

+ `-d`: 后台运行容器并返回容器 ID。
+ `-it`: 交互式运行容器，分配一个伪终端。
+ `--name`: 给容器指定一个名称。
+ `-p`: 端口映射，格式为`host_port:container_port`。
+ `-v`: 挂载卷，格式为`host_dir:container_dir`。
+ `--rm`: 容器停止后自动删除容器。
+ `--env`或`-e`: 设置环境变量。
+ `--network`: 指定容器的网络模式。
+ `--restart`: 容器的重启策略（如`no` `on-failure` `always` `unless-stopped`）。
+ `-u`: 指定用户。

（`--rm`并不会自动删除后台运行的容器，也就是说，这个选项和-d同时使用时不会生效。）

例如，创建并运行一个Ngnix容器：

```plain
docker run -d -p 8080:80 -v /home/cyberkanjousen/web_test:/data --name test nginx
```

解释：后台运行一个命名为`test`的 nginx 容器，将主机的`8080`端口映射到容器的`80`端口，并将主机的 `/home/cyberkanjousen/web_test`目录挂载到容器的`/data`目录。

在主机浏览器访问`http://localhost:8080`：![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760845193202-a9230206-6a79-4346-a2cd-b43d977f4d21.png)

当然，如果在这个容器上搞静态网页的话，还需要编辑配置文件——我把它放到后面再做。

<h3 id="WjSSb">start</h3>
启动容器：

```plain
docker start [OPTIONS] CONTAINER [CONTAINER...]
```

可用选项：

+ `-a`: 附加到容器的标准输入输出流。
+ `-i`: 附加并保持标准输入打开。

这条命令可以同时启动多个容器：

```plain
docker start container1 container2 container3
```

<h3 id="XmTpC">stop</h3>
停止容器：

```plain
docker stop [OPTIONS] CONTAINER [CONTAINER...]
```

可用选项：

+ `-t` `--time`: 停止容器之前等待的秒数，默认是 10 秒。

<h3 id="rGE6B">restart</h3>
重启容器：

```plain
docker restart [OPTIONS] CONTAINER [CONTAINER...]
```

可用选项：

+ `-t` `--time`: 停止容器之前等待的秒数，默认是 10 秒。

<h3 id="qBAKJ">kill</h3>
立刻终止容器：

```plain
docker kill [OPTIONS] CONTAINER [CONTAINER...]
```

可用选项：

+ `-s` `--signal`: 发送给容器的信号（默认为`SIGKILL`）。

可用信号：

+ `SIGKILL`: 强制终止进程（默认信号）。
+ `SIGTERM`: 请求进程终止。
+ `SIGINT`: 发送中断信号，通常表示用户请求终止。
+ `SIGHUP`: 挂起信号，通常表示终端断开。

<h3 id="TPTSe">rm</h3>
删除已经停止的容器（未停止的容器不会被删除）。

```plain
docker rm [OPTIONS] CONTAINER [CONTAINER...]
```

可用选项：

+ `-f` `--force`: 强制删除正在运行的容器（使用`SIGKILL`信号）。
+ `-l` `--link`: 删除指定的连接，而不是容器本身。
+ `-v` `--volumes`: 删除容器挂载的卷。

删除所有已经停止的容器：

```plain
docker rm $(docker ps -a -q)
```

其中`docker ps -a -q`会返回所有已经停止的容器。

<h3 id="PCpY6">pause/unpause</h3>
暂停/恢复容器：

```plain
docker pause CONTAINER [CONTAINER...]
docker unpause CONTAINER [CONTAINER...]
```

> 使用场景：
>
> + 临时暂停活动: 当需要临时暂停容器中的所有活动以进行系统维护或资源管理时，可以使用`docker pause`。
> + 资源管理: 在需要重新分配系统资源时，暂停不必要的容器以释放资源。
> + 调试和故障排除: 在调试或故障排除过程中暂停容器以分析当前状态。
>
> 出自[runoob.com](https://www.runoob.com/docker/docker-pause-unpause-command.html)
>

<h3 id="XMFV9">create</h3>
```plain
docker create [OPTIONS] IMAGE [COMMAND] [ARG...]
```

可用选项：

+ `--name`: 给容器指定一个名称。
+ `-p, --publish`: 端口映射，格式为`host_port:container_port`。
+ `-v, --volume`: 挂载卷，格式为`host_dir:container_dir`。
+ `-e, --env`: 设置环境变量。
+ `--network`: 指定容器的网络模式。
+ `--restart`: 容器的重启策略（如`no` `on-failure` `always` `unless-stopped`）。
+ `-u, --user`: 指定用户。
+ `--entrypoint`: 覆盖容器的默认入口点。
+ ``--detach`: 在后台创建容器。

<h3 id="nn56Q">exec</h3>
在运行中的容器运行一个新命令：

```plain
docker exec [OPTIONS] CONTAINER COMMAND [ARG...]
```

可用选项：

+ `-d` `--detach`: 在后台运行命令。
+ `--detach-keys`: 覆盖分离容器的键序列。
+ `-e, --env`: 设置环境变量。
+ `--env-file`: 从文件中读取环境变量。
+ `-i, --interactive`: 保持标准输入打开。
+ `--privileged`: 给这个命令额外的权限。
+ `--user, -u`: 以指定用户的身份运行命令。
+ `--workdir, -w`: 指定命令的工作目录。
+ `-t, --tty`: 分配一个伪终端。

> 使用场景
>
> + 调试容器: 进入容器内部进行调试和排查问题。
> + 管理任务: 在容器内运行附加的管理任务或维护操作。
> + 监控和检查: 在容器内执行监控和检查命令，获取运行状态和日志。
>
> 出自[runoob.com](https://www.runoob.com/docker/docker-exec-command.html)
>

<h3 id="UBEvW">rename</h3>
重命名容器：

```plain
docker rename NAME|ID NEW_NAME
```

<h2 id="vpGwY">容器操作</h2>
<h3 id="XnUji">ps</h3>
返回容器列表：

```plain
docker ps [OPTIONS]
```

可用选项：

+ `-a` `--all`: 显示所有容器，包括停止的容器。
+ `-q` `--quiet`: 只显示容器 ID。
+ `-l` `--latest`: 显示最近创建的一个容器，包括所有状态。
+ `-n`: 显示最近创建的`n`个容器，包括所有状态。
+ `--no-trunc`: 不截断输出。
+ `-s` `--size`: 显示容器的大小。
+ `--filter` `-f`: 根据条件过滤显示的容器。
+ `--format`: 使用 Go 模板语法格式化输出。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760847516613-087dceed-3893-46c6-8e76-74a92fbc180f.png)

<h3 id="g23Uc">inspect</h3>
返回 JSON 格式的 Docker 对象（容器、镜像、卷、网络等）的详细信息。

```plain
docker inspect [OPTIONS] NAME|ID [NAME|ID...]
```

可用选项：

+ `-f` `--format`: 使用 Go 模板语法格式化输出。
+ `--type`: 返回指定类型的对象信息（可选类型：`container` `image` `network` `volume`）。

将信息输出到`.json`文件：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760848315866-43c17d7a-d1a1-43b6-bf7f-1c8054d9debb.png)  
![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760848258645-83b346b0-5edb-437e-a07b-80d3f3ad6045.png)

<h3 id="ePANe">logs</h3>
返回容器日志输出：

```plain
docker logs [OPTIONS] CONTAINER
```

可用选项：

+ `-f` `--follow`: 跟随日志输出。
+ `--since`: 从指定时间开始显示日志。
+ `-t` `--timestamps`: 显示日志时间戳。
+ `--tail`: 仅显示日志的最后部分，例如`--tail 10`显示最后 10 行。
+ `--details`: 显示提供给日志的额外详细信息。
+ `--until`: 显示直到指定时间的日志。

跟随日志输出：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760849734369-d9a8457c-57b6-4b01-aef4-88361b5708a3.png)

输入`^C`即可退出。

<h3 id="ujMd7">commit</h3>
将容器的当前状态保存为一个新的 Docker 镜像:

```plain
docker commit [OPTIONS] CONTAINER [REPOSITORY[:TAG]]
```

可用选项：

+ `-a`: 提交的镜像作者。
+ `-c`: 使用`Dockerfile`指令来创建镜像。
+ `-m`: 提交时的说明文字。
+ `-p`: 提交镜像前暂停容器（默认为`true`）。

<h3 id="CkMC8">cp</h3>
在 Docker 容器和宿主机之间复制文件或目录：

```plain
docker cp [OPTIONS] SRC_PATH CONTAINER:DEST_PATH # 从主机复制文件或目录到容器
docker cp [OPTIONS] CONTAINER:SRC_PATH DEST_PATH # 从容器复制文件或目录到主机
```

比如：我要修改先前创建的`test`容器中的`nginx.conf`，就可以把这个文件复制到主机，在主机上修改，然后复制到容器里：

> By default, the configuration file is named `nginx.conf` and placed in the directory `/usr/local/nginx/conf`, `/etc/nginx`, or `/usr/local/etc/nginx`.
>
> 出自[nginx.org](https://nginx.org/en/docs/beginners_guide.html)
>

```plain
docker cp test:/etc/nginx/nginx.conf /home/cyberkanjousen
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760864310478-962c345b-6464-4dbe-87e4-e65c6994d312.png)

> `nginx.conf`的配置详见[此处](https://nginx.org/en/docs/beginners_guide.html)。
>

配置完成后，再复制回容器中（此时会覆盖原文件），并重载Nginx：

```plain
docker cp /home/cyberkanjousen/nginx.conf test:/etc/nginx
docker exec test nginx -s reload
```

（实际上直接在Docker Desktop上就可以修改，这里是为了练习一下`cp`命令。）

我在`/home/cyberkanjousen/web_test`（这个路径已经挂载在容器的`/data`了）下放了一个`home.html`：

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>test</title>
</head>
<body>
    <h1>Hello World!</h1>
</body>
</html>
```

在浏览器中访问`http://localhost:8080/home.html`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760865588721-f5b049ba-0c78-405a-a154-d95cae8d3f30.png)

<h2 id="f5dRU">镜像</h2>
<h3 id="tLVBa">images</h3>
列出本地的 Docker 镜像：

```plain
docker images [OPTIONS] [REPOSITORY[:TAG]]
```

可用选项：

+ `-a` `--all`: 显示所有镜像（包括中间层镜像）。
+ `--digests`: 显示镜像的摘要信息。
+ `-f` `--filter`: 过滤输出，基于提供的条件。
+ `--format`: 使用 Go 模板格式化输出。
+ `--no-trunc`: 显示完整的镜像 ID。
+ `-q` `--quiet`: 只显示镜像 ID。

比如，列出所有本地镜像：

```plain
docker images
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760850596644-f6fbff07-8af9-41db-a1ba-d20a55705156.png)

<h3 id="a0FVx">rmi</h3>
删除一个或多个 Docker 镜像：

```plain
docker rmi [OPTIONS] IMAGE [IMAGE...]
```

可用选项：

+ `-a` `--all-tags`: 指定仓库名称时，删除该仓库下的所有镜像。
+ `-f` `--force`: 强制删除镜像，即使该镜像被容器使用。
+ `--help`: 打印帮助信息并退出。
+ `--no-prune`: 不删除悬空的父镜像。
+ `-q` `--quiet`: 安静模式，不显示删除镜像的详细信息。

<h3 id="H1dBJ">pull</h3>
拉取一个镜像：（默认标签为`lastest`）

```plain
docker pull [OPTIONS] NAME[:TAG|@DIGEST]
```

+ `--all-tags` `-a`: 下载指定镜像的所有标签。
+ `--disable-content-trust`: 跳过镜像签名验证。

比如，拉取一个最新版本的Nginx镜像：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760851024302-ea9f13ca-8ea7-4f75-a769-73c8b236a459.png)

<h3 id="DnOru">build</h3>
通过读取`Dockerfile`中定义的指令，逐步构建镜像，并将最终结果保存到本地镜像库中：

```plain
docker build [OPTIONS] PATH | URL | -
```

常用可用选项：

+ `-t` `--tag`: 为构建的镜像指定名称和标签。
+ `-f` `--file`: 指定`Dockerfile`的路径（默认是`PATH`下的`Dockerfile`）。
+ `--build-arg`: 设置构建参数。
+ `--no-cache`: 不使用缓存层构建镜像。
+ `--rm`: 构建成功后删除中间容器（默认开启）。
+ `--force-rm`: 无论构建成功与否，一律删除中间容器。
+ `--pull`: 始终尝试从注册表拉取最新的基础镜像。

<h1 id="xoYih">Dockerfile</h1>
Dockerfile 的指令如下（出自[runoob.com](https://www.runoob.com/docker/docker-dockerfile.html)）：

| Dockerfile 指令 | 说明 |
| --- | --- |
| `FROM` | 指定基础镜像，用于后续的指令构建。 |
| `LABEL` | 添加镜像的元数据，使用键值对的形式。 |
| `RUN` | 在构建过程中在镜像中执行命令。 |
| `CMD` | 指定容器创建时的默认命令。（可以被覆盖）<br/>注意：如果 Dockerfile 中如果存在多个 CMD 指令，仅最后一个生效。 |
| `ENTRYPOINT` | 设置容器创建时的主要命令。（不可被覆盖） |
| `EXPOSE` | 声明容器运行时监听的特定网络端口。 |
| `ENV` | 在容器内部设置环境变量。 |
| `ADD` | 将本机的文件、目录或远程URL复制到镜像中。（相对`ADD`而言更推荐使用`COPY`，但后者在不解压的前提下，无法复制`.tar`压缩文件） |
| `COPY` | 将本机的文件或目录复制到镜像中。 |
| `VOLUME` | 为容器创建挂载点或声明卷。 |
| `WORKDIR` | 设置后续指令的工作目录。 |
| `USER` | 指定后续指令的用户上下文。 |
| `ARG` | 定义在构建过程中传递给构建器的变量，可使用 "docker build" 命令设置。 |
| `ONBUILD` | 当该镜像被用作另一个构建过程的基础时，添加触发器。 |
| `STOPSIGNAL` | 设置发送给容器以退出的系统调用信号。 |
| `HEALTHCHECK` | 定义周期性检查容器健康状态的命令。 |
| `SHELL` | 覆盖Docker中默认的shell，用于RUN、CMD和ENTRYPOINT指令。 |


比如，我要定制一个Ngnix镜像，该镜像的`/usr/share/nginx/html`下有我之前写的那个`home.html`。于是新建一个目录，在这个空目录下新建`Dockerfile`文件：

```dockerfile
FROM nginx
CMD cd /usr/share/nginx/html \
    && touch home.html \
    && echo "<!DOCTYPE html>" >> home.html \
    && echo "<html>" >> home.html \
    && echo "<head>" >> home.html \
    && echo "<meta charset="utf-8">" >> home.html \
    && echo "<title>test</title>" >> home.html \
    && echo "</head>" >> home.html \
    && echo "<body>" >> home.html \
    && echo "    <h1>Hello World!</h1>" >> home.html \
    && echo "</body>" >> home.html \
    && echo "</html>" >> home.html \
    && cd /
```

之所以要使用`&&`而不用多个`CMD`，是因为：

> Dockerfile 的指令每执行一次都会在 docker 上新建一层。所以过多无意义的层，会造成镜像膨胀过大。例如：
>

```dockerfile
FROM centos
RUN yum -y install wget
RUN wget -O redis.tar.gz "http://download.redis.io/releases/redis-5.0.3.tar.gz"
RUN tar -xvf redis.tar.gz
```

> 以上执行会创建 3 层镜像。
>
> 出自[runoob.com](https://www.runoob.com/docker/docker-dockerfile.html)
>

`CMD`等同理。

在这个目录下，运行：

```plain
docker build -t nginx:dwb .
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760869345543-8ae9d6ef-126d-4696-832c-15b0b2eb24a0.png)

随后该镜像就会出现在镜像列表中：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760869726593-4b09f73f-9c77-4ed3-9134-ce76ce77a92a.png)

`dwb`可以是任意合法字符串，只要不和已有的镜像的标签重复即可。若留空，则默认为`latest`标签。

`.`代表Docker引擎将使用这个上下文路径内的文件来构建镜像。

用这个镜像创建一个容器`dwb`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760873895018-acdd1353-2e4a-4e62-8310-b0697c44c41c.png)

访问`http://localhost:8080/home.html` ：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760874143634-659d3266-2675-4561-82ab-03638733ff40.png)

<h1 id="nl8DG">端口映射</h1>

> 在电脑网络中，端口转发或端口映射（英语：Port forwarding）是网络地址转换（NAT）的一种应用，当网络数据包穿越网关时，例如路由器或防火墙时，将带有 IP地址和端口的连线请求转发到指定的 IP 地址及其端口口上，换句话说，就是将一台主机的网络端口转发到另外一台主机并由另一台主机提供转发的网络服务。此技术最常用于将位于受保护网络或局域网（LAN）内主机上的服务提供给广域网（WAN）上的主机。
>
> 出自[zh.wikipedia.org](https://zh.wikipedia.org/wiki/%E7%AB%AF%E5%8F%A3%E8%BD%AC%E5%8F%91)
>

在没有公共IP的服务器上搭建网站时，为了将能够让局域网外计算机访问主机，就需要使用端口映射，将这个服务器的一个端口（通常是`8080`（HTTPS））暴露到广域网（暴露到广域网的端口可能与服务器防火墙开放的端口不同）。

在Docker中，情况也是相似的，端口映射可以使容器的一个端口暴露到局域网（容器的网络可以说是局域网内的局域网，也就是主机）中。

在`test`容器的实例中，我将主机的`8080`端口映射到容器的`80`端口上，使主机通过`http://localhost:8080/` 访问存放在容器上的静态网站。`-p`默认绑定的是TCP端口，如果要绑定UDP端口，则要在端口后加上`/udp`。

在这里，主机的`8080`端口映射到容器`dwb`的`80`端口上，并且是TCP端口。

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760873895018-acdd1353-2e4a-4e62-8310-b0697c44c41c.png)

<h1 id="SFU2y">Docker Hub</h1>

在执行`docker pull`命令时，若本地注册表上没有这个镜像，且镜像的源的设定保持默认不便，那么就会从Docker Hub上拉取镜像。在准备工作中，通过配置代理，使主机绕过防火长城，成功拉取了镜像到本地。

将本地的镜像推送到Docker Hub和将本地的项目推送到GitHub类似（如果两者都用命令行进行推送的话）。首先，需要在Docker Hub上注册一个账号，然后新建一个仓库：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760874654284-9db68116-38c5-4e61-a159-574a251d4e7d.png)

根据[官方文档](https://docs.docker.com/desktop/setup/sign-in/#credentials-management-for-linux-users)，在登录本地Docker之前，必须初始化`pass`。

首先生成一个GPG密钥：

```plain
gpg --generate-key
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760876598921-859c48a4-970d-496e-8db9-6b750afcad5d.png)

复制GPG ID并初始化`pass`：

```plain
pass init <your_generated_gpg-id_public_key>
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760876856385-2f8271a8-b5ab-46fd-9911-2b6298a8f185.png)

然后就可以登录Docker了：（也可以在Docker Desktop的图形界面登录）

```plain
docker login
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760876993096-69b684ec-a318-4e01-b225-0317ce9d16c3.png)

和CLion一样，Docker Desktop也可以直接通过图形界面进行镜像推送。命令行界面的话则使用`tag` `push`命令：

```plain
docker tag <existing-image> <hub-user>/<repo-name>[:<tag>]
docker push <hub-user>/<repo-name>:<tag>
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760878007637-7f8af0ea-78f8-418d-9957-cd9b0676ee4f.png)

推送完成：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760878070327-4d05785e-03ba-4057-9ac9-89d35bb03a92.png)

<h1 id="XT0OR">在Docker上运行SpigotMC</h1>
既然能在Docker上运行Ngnix，那为什么不能运行Minecraft（Java版）服务器呢？于是我决定从Ubuntu镜像构建一个镜像，通过这个镜像创建的容器可以运行SpigotMC（MCJE 1.18.2）。

首先写一个Dockerfile：

```dockerfile
FROM ubuntu
RUN apt update \
    && apt install openjdk-17-jdk -y \
    && apt install wget -y \
    && cd /home \
    && mkdir server \
    && cd server \
    && wget -O spigotmc.jar "https://getbukkit.org/get/M3CuBYuR72VaQB3W6T2TTbPiklfqSn1u" \
    && touch start.sh \
    && echo "java -Xms4G -Xmx8G -jar spigotmc.jar --nogui" >> start.sh \
    && apt-get purge -y --auto-remove $buildDeps
```

（我本来安装的是JDK-21，结果SpigotMC提示其能够运行的最高的Java版本是JDK-18——害得我重新构建了一遍镜像。）

然后构建：

```plain
docker build -t cyberkanjousen/hello_cv:spigotmc .
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760888943935-b8a7a58f-2f15-4fa5-87d0-8c56d3f194ef.png)

用这个镜像创建一个容器：

```plain
docker run -it --name test -p 1919:810 cyberkanjousen/hello_cv:spigotmc /bin/bash
```

启动服务器：

```dockerfile
cd /home/server
./start.sh
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760889531520-25007d79-420f-4a33-88aa-def08ff72526.png)

拒绝访问。看来需要改一下文件权限了——这是以前每次创建MCJE服务器实例时不得不品的一环。当时订购的云服务器用SSH连接，登录的一般都是root用户，因而初始没有对`/home`目录下的执行权限，我觉得大概这是为了系统安全而特意设计的。我一般会执行以下命令：

```dockerfile
chmod ugo=rwx start.sh
```

之所以要给一下读写权限，是为了使改启动参数更加方便。然后再次运行：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760889578242-2922ceca-dce5-4bb4-b6d2-1cc3c6364e8c.png)

经典“同意EULA条款”。修改`eula.txt`文件，把`false`改为`true`：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760889749253-752eddca-6c8e-4613-95ee-fcff1b40e8e7.png)然后再次启动：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760889887945-6a08707b-3c99-437a-b6dc-72bea438a4e2.png)

启动成功，但这时候还连接不了，需要先输入`stop`关闭，然后修改一下`query.port`和`server-port`的端口：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760891088409-526dbe1d-8b06-4d83-a9ee-520636aa2eed.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760891117504-344642c7-d5d3-44c0-b2c5-833d0ce13ec1.png)

再次启动。客户端连接一下试试：

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760890013162-d1554aa5-3a21-463c-a15d-e2fe44b8d8eb.png)

可以成功连接！

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760891169039-5e15a350-770b-4031-b332-c95dcd506a91.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760891431263-a2884460-d21f-4368-b3ad-ded0d42473e5.png)

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760891464566-dc05b889-455e-40a2-b66f-bc4d553a6a25.png)

（有什么用呢？也许可以用来测试Spigot插件在不同版本Ubuntu的可用性……）

[语雀笔记页面](https://www.yuque.com/u29112212/ucgrla/skirwu5gdy1d7gux)