# 1449

## 描述

昨天将服务器数据库数据 (data/)目录下的数据拷贝到本机上后，使用 `Navicat` 连接，在修改数据的时候出现错误

`The user specified as a definer ('staticecg'@'%') does not exist`，

## 解决方案

因为服务器账户是 `staticecg`，而本机没这个账户，那给这个账户授权就可以了。因为本机使用，不需要特殊权限，所以全部权限授予。

~~~mysql
# 授予权限
grant all privileges on *.* to staticecg@"%" identified by "."
> OK

# 刷新权限
flush privileges
> OK
~~~

## 扩展

参考：[MySQL授权命令grant的使用方法](https://www.cnblogs.com/crxis/p/7044582.html)

`MySQL` 赋予用户权限命令的简单格式可概括为：

~~~mysql
grant 权限 on 数据库对象 to 用户

#example
grant select, insert, update, delete on testdb.* to common_user@'%'
grant create on testdb.* to developer@'192.168.0.%';
~~~

