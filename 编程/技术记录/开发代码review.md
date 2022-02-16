

几个关于代码方向的知识点

## 关于头文件

**说明**

>  `module`,`module1`,`module2`,`module3`均为 模块封装的`dll` 

~~~c++
#include "../module/demo.h"
#include "../module1/demo1.h"
#include "../module2/demo2.h"
#include "../module3/demo3.h"
~~~

为什么会使用上面这种，而不使用 `Qt` 的配置文件直接增加模块。如下面所写

~~~pro
INCLUDEPATH += $$PWD/../module/include
~~~

就是避免在不同的模块下会有相同名称的头文件，会造成歧义。

~~~c++
#include "../module4/test.h"
#include "../module5/test.h"
~~~



## if 最后的 else

if 最后的 else 最好是添加上，最好是打一个日志，这样方便往后查看。

~~~c++
void func()
{
    if( ...) {
        
    }
    else if ( ... ){
        
    }
    else {
        qDebug() << "....";
    }
}
~~~



## 关于加锁

锁的资源最好是把名称表明，对象的命名长一点没关系，但是一定要表明，不同的资源采用不同的锁，避免造成死锁的情况产生。

~~~c++
 QMutexLocker m_lock(&m_testlistlocker);
~~~

**锁的并不是代码，而是某个资源**，所以，锁要下沉，的粒度越小越好。



## 关于typedef 的定义位置

如果仅限本类里面使用，那么，最好将 定义类型 移动到类里面，避免在别人引用你头文件的时候，污染命名空间。

**不好的示范**

~~~c++
// test.h
typedef QList<int> TestList;

class Test
{
    private:
    TestList m_testList;
}
~~~

**优化的代码**

~~~c++
class Test
{
    typedef QList<int> TestList;
    
    private:
    TestList m_testList;
}
~~~



