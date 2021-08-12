终究还是错付了~

终于还是把密码忘记了



记录 `MySQL` 密码修改

# 停止服务

`cmd` 或者直接服务管理界面关闭都可以，服务名看你自己的设置。

~~~shell
net stop MySQL80
~~~

![image-20210812221416758](images/image-20210812221416758.png)

# `CMD`打开`mysql`

![image-20210812221737321](images/image-20210812221737321.png)

打开`cmd`窗口

# 设置跳过密码

~~~shell
mysqld --console --skip-grant-tables --shared-memory
~~~

![image-20210812221909991](images/image-20210812221909991.png)

这时候是 光标一直闪，再打开一个 `terminal`

# 新开`terminal`

直接输入 `mysql` ，可以看到，已经能打开了

![image-20210812222013609](images/image-20210812222013609.png)

# 刷新权限

~~~shell
 flush privileges;
~~~

![image-20210812222344591](images/image-20210812222344591.png)

# 设置密码

比如设置 `123456`

~~~shell
set password for root@localhost='123456'
~~~

![image-20210812222434584](images/image-20210812222434584.png)

注意，最后还要加一个  `;`

# 成功

![image-20210812222546781](images/image-20210812222546781.png)

# 测试

## 打开服务

~~~shell
net start mysql80
~~~

## 打开终端

~~~shell
mysql -u root -p
~~~

![image-20210812222846539](images/image-20210812222846539.png)

![image-20210812224122318](images/image-20210812224122318.png)



# 另一种

![image-20210812224235436](images/image-20210812224235436.png)