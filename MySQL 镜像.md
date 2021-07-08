# 数据库镜像

## 1、建立备份账户

~~~mysql
GRANT ALL PRIVILEGES ON *.* TO 'backup'@'localhost' IDENTIFIED BY 'koanruler' WITH GRANT OPTION;
~~~

![1625709332274](images/1625709332274.png)

## 2、备份数据库

​		这期间不允许“主数据库”的任何写操作。防止数据不同步。

![1625709496034](images/1625709496034.png)

## 3、修改配置表

**主数据库**

​		采用默认配置

		 #server-id=1
		 #log-bin=c:\mysql\log-bin.log 

**备用数据库**

~~~mysql
[mysqld]
server-id	= 2
master-host=localhost
master-user=backup
master-password=koanruler
replicate-do-db=backup
~~~

## 4、重启服务器

~~~powershell
net start MySQL5;
net start MySQL50;
~~~





## 5、结果

果然，没这么好的事情，哪有一次成功的。

​		修改A服务器中的数据，发现B中并没有修改。5为主服务器，50为备用服务器

![1625711356673](images/1625711356673.png)

主服务器中：

![1625711377143](images/1625711377143.png)

备用服务器：

![1625711436752](images/1625711436752.png)

## 6、继续改进

查看从数据库错误日志

![1625711630173](images/1625711630173.png)

显示错误

~~~tex
210708 10:24:28 [ERROR] Slave I/O: error connecting to master 'backup@localhost:3306' - retry-time: 60  retries: 86400, Error_code: 1045
~~~

### 查询Error Code

​		问度娘了，权限错误。看半天也看不到，算了，问谷哥咯，CV工程师上线

![1625712039863](images/1625712039863.png)

### **查看主数据库登陆表 **

~~~mysql
SELECT USER,HOST,PASSWORD FROM mysql.USER;
~~~

![1625712257720](images/1625712257720.png)

就是你咯![1625712311471](images/1625712311471.png)

这个说：`localhost`这个 `IP` 的朋友能够使用 一个叫 `backup`的账户登录。

### 查看账户权限

~~~mysql
SHOW GRANTS FOR 'backup'@'localhost';
~~~

![1625712441535](images/1625712441535.png)

咦，“西红柿”！！！！所有权限都可以啊？？？？咋回事，不按套路出牌啊~

慢！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！

![1625713697929](images/1625713697929.png)这什么玩意，兄弟，你连3307啊，跑3306去干哈

**配置文件**的锅咯，重新修改配置文件

**从数据库**

~~~ini
# Replication Master Server (default)
# binary logging is required for replication
log-bin=mysql-bin
relay-log=KoanRuler-relay-bin
binlog-do-db= #your database#
auto_increment_offset = 2 			# 这里是为了双向写入 # 2号数据库从2开始， # 三号服务器设置3   这样就不重复
auto_increment_increment = 2		# 数据库自增量，目前两个服务器，所以自增设置 2 ，如果三个服务器设置3合适
# required unique id between 2 and 2^32 - 1
# (and different from the master)
# defaults to 2 if master-host is set
# but will not function as a slave if omitted
server-id       = 2
# The replication master for this slave - required
#master-host     =   <hostname>
master-host = localhost
# The username the slave will use for authentication when connecting
# to the master - required
#master-user     =   <username>
master-user = backup
#
# The password the slave will authenticate with when connecting to
# the master - required
#master-password =   <password>
master-password = koanruler
#
# The port the master is listening on.
# optional - defaults to 3307
#master-port     =  <port>
master-port     =  3307
~~~



如果需要双向，那么

**主数据库**需要进行同样的配置，就是反过来的意思  现在 B是主服务器，A是从服务器了。

![1625716285918](images/1625716285918.png)

像上面一样，A指向B，B指向C，C指向A，一个循环指向。配置表中

修改完成，继续测试。

还是错误，这里忘截图了、、、、干。。。

**配置再次查看**

主服务器查看配置

~~~mysql
SHOW SLAVE STATUS;
~~~

![1625716539779](images/1625716539779.png)

没有任何东西，正常，因为，没配置主服务器。

从服务器查看配置

![1625716567779](images/1625716567779.png)

配置，端口3307，正常。



### 查看同步原理

在master机器上，主从同步事件会被写到特殊的log文件中(binary-log);

在slave机器上，slave读取主从同步事件，并根据读取的事件变化，在slave库上做相应的更改。 

<font color=red>是通过 log 的变化，来加入同步。</font>

找到一张图

![1625716708563](images/1625716708563.png)

就是说，通过日志的变化来同步！！！！！！！！！！！

写到这里，我Tom喵就Tom喵了。

我第一步直接复制的数据库啊，里面很多日志啊，删除日志试试。

啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊！！！！！！！！！

年轻的我，躁动的心！

可以了~

完~

~

