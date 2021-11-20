# 元对象

**Q_OBJECT**

该宏提供了对元对象的访问，使得能够使用比如信号和槽等`QObject`的更多特性。元对象提供了诸如类名、属性和方法等的信息，也被称为“[反射](http://en.wikipedia.org/wiki/Reflection_(computer_science))”。

通过使用`QMetaObject`，我们能够用如下代码显示一些类的信息：

~~~c++
QObject obj;  
const QMetaObject *metaObj = obj.metaObject();  
qDebug() << "class name: " << metaObj->className();  
qDebug() << "class info count: " << metaObj->classInfoCount();  
qDebug() << "methods: ";  
// 从QMetaObject::methodOffset()开始打印，使其不会显示父类的方法  
for (int i = metaObj->methodOffset(); i < metaObj->methodCount(); ++i)  
  qDebug() << metaObj->method(i).methodType() << " " << metaObj->method(i).signature();  
~~~

由于C++并没有提供对这些信息的任何支持，Qt引入了[元对象编译器](http://doc.trolltech.com/moc.html)（`moc`）来完成相应的工作。`moc`会读取每个头文件，如果发现其中定义的类是继承自`QObject`，且定义了*Q_OBJECT*宏，便会创建一个相应的C++源代码文件（`moc_*.cpp`），来完成这些工作。通过代码生成的工作，Qt不仅能够获得诸如Java等语言的**灵活性**，还能很好的保证继承自C++的**性能和可扩展性**。

这个和 C# 里面的差不太多啊

假设我们有如下所示的简单类：

~~~c++
class MyObject : public QObject  
{  
  Q_OBJECT  
public:  
  explicit MyObject(QObject *parent = 0);  
  void myFunc();  
public slots:  
  void mySlot(int myParam);  
signals:  
  void mySignal(int myParam);  
};  
~~~

`moc`会自动创建以下信息：

~~~c++
// 保存在QMetaObject::d.data指向的空间，其起始部分是一个QMetaObjectPrivate结构体  
static const uint qt_meta_data_MyObject[] = {  
  5,       // 版本号，其内部结构在Qt开发中有所改变  
  0,       // 类名，其值为字符串qt_meta_stringdata_MyObject的偏移量  
  // 以下值为（数量，索引）对  
  0,    0, // 类信息  
  2,   14, // 这里定义了两个方法，其起始索引为14（即signal部分）  
  0,    0, // 属性  
  0,    0, // 枚举  
  0,    0, // 构造函数  
  0,       // 标识  
  1,       // signal数量  
  // 对于signal、slot和property，其signature和parameters为字符串qt_meta_stringdata_MyObject的偏移量  
  // signals: signature, parameters, type, tag, flags  
  18,   10,    9,    9, 0x05,  
  // slots: signature, parameters, type, tag, flags  
  32,   10,    9,    9, 0x0a,  
  0        // eod  
};  
// 保存在QMetaObject::d.stringdata指向的空间  
static const char qt_meta_stringdata_MyObject[] = {  
  "MyObject/0/0myParam/0mySignal(int)/0"  
  "mySlot(int)/0"  
};  
~~~

以上信息，及其基类的相关信息，都保存在该类对应的元对象中：

~~~c++
const QMetaObject MyObject::staticMetaObject = {  
  { &QObject::staticMetaObject, // 指向其基类的元对象，保存在QMetaObject::d.superdata  
    qt_meta_stringdata_MyObject, qt_meta_data_MyObject, 0 }  
};  
~~~

这样，如果我们希望对`QObject`的对象*进行类型转换，就不需使用开销较大的运算符*dynamic_cast*， 而能够直接使用`qobject_cast`。该模板函数利用了元对象系统的信息，避免了在运行时进行类型转换：

~~~c++
template <class T> inline T qobject_cast(QObject *object)  
{  
#if !defined(QT_NO_QOBJECT_CHECK)  
  reinterpret_cast(0)->qt_check_for_QOBJECT_macro(*reinterpret_cast(object));  
#endif  
  return static_cast(reinterpret_cast(0)->staticMetaObject.cast(object));  
}  
~~~

这里，目标类型的元对象仅仅检查其是否从自身继承而来：

~~~c++
const QObject *QMetaObject::cast(const QObject *obj) const  
{  
  if (obj) {  
    const QMetaObject *m = obj->metaObject();  
    do {  
      if (m == this)  
        return obj;  
    } while ((m = m->d.superdata));  
  }  
  return 0;  
}  
~~~

此外，`moc`会为每一个**信号**创建相应函数。当信号被emit时，该函数会被自动调用：

~~~c++
void MyObject::mySignal(int _t1)  
{  
  void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };  
  // 检查链接到该信号的所有slot，并根据链接类型进行调用  
  QMetaObject::activate(this, &staticMetaObject, 0, _a);  
}  
~~~

最后，这些信号都会通过`moc`创建的`qt_metacall`函数被调用：

~~~c++
int MyObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)  
{  
  // 如果该函数已被基类调用，则直接返回  
  _id = QObject::qt_metacall(_c, _id, _a);  
  if (_id < 0)  
    return _id;  
  // 根据函数的ID进行调用  
  if (_c == QMetaObject::InvokeMetaMethod) {  
    switch (_id) {  
    case 0: mySignal((*reinterpret_cast< int(*)>(_a[1]))); break;  
    case 1: mySlot((*reinterpret_cast< int(*)>(_a[1]))); break;  
    default: ;  
    }  
    // 删除被该类“消耗”的ID，使得其子类类在处理时ID总是从0开始，而返回值-1则表示该函数已被调用  
    _id -= 2;  
  }  
  return _id;  
}  
~~~

