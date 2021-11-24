# Detailed Description

The QSharedPointer is an **automatic**, shared pointer in C++. It behaves exactly like a normal pointer for normal purposes, including respect for constness.

QSharedPointer will delete the pointer it is holding when it goes out of scope, provided no other QSharedPointer objects are referencing it.

A QSharedPointer object can be created from a normal pointer, another QSharedPointer object or by promoting a [QWeakPointer](https://doc.qt.io/qt-5/qweakpointer.html) object to a strong reference. 

# Thread-Safety

QSharedPointer and [QWeakPointer](https://doc.qt.io/qt-5/qweakpointer.html) are reentrant classes. This means that, in general, a given QSharedPointer or [QWeakPointer](https://doc.qt.io/qt-5/qweakpointer.html) object **cannot** be accessed by multiple threads at the same time without synchronization.





# 参考

* [官方文档](https://doc.qt.io/qt-5/qsharedpointer.html#details)

