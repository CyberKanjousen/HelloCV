1. 输入git，git已经安装。  
![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760351754682-bf2619b4-31c7-4d4c-9f83-d757576e0916.png)
2. 设置用户名和邮箱：

```plain
git config --global user.name "用户名"
git config --global user.email 邮箱
git config user.name #查看用户名
git config user.email #查看邮箱
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760420676255-cfebfdec-69d3-410d-a47f-059e0196c6ab.png)

3. 输入以下命令，生成SSH密钥。

```plain
ssh-keygen -t rsa -C "此处输入邮箱地址"
```

4. 生成的密钥默认保存在/home/用户名称/.ssh/id_rsa.pub中，可以用vim打开。进入github的Setting -> SSH and GPG Keys页面，添加一个新的SSH密钥，将返回的密钥复制到“Key”一栏，最后Add SSH Key即可。  
![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760357486474-4caefc37-0bf0-42ea-8397-eb808cc418fb.png)  
![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760359138939-6836ec89-2f39-46d3-8a3a-b52d3cd767f7.png)
5. 输入以下命令，验证成功完成了SSH密钥配置。

```plain
ssh -T git@github.com
```

![](https://cdn.nlark.com/yuque/0/2025/png/29543181/1760359488058-f5a6b442-5f34-43e3-bb83-b877d14fd6e8.png)

