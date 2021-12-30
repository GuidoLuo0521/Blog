# 感谢

## WOBOQ ![image-20211230135545965](Q_ENUM.assets/image-20211230135545965.png)

* [New in Qt 5.5: Q_ENUM and the C++ tricks behind it](https://woboq.com/blog/q_enum.html)

# New in Qt 5.5: Q_ENUM and the C++ tricks behind it

Qt 5.5 was just released and with it comes a new [`Q_ENUM`](https://doc.qt.io/qt-5/qobject.html#Q_ENUM) macro, a better alternative to the now deprecated Q_ENUMS (with S).

In this blog post, I will discuss this new Qt 5.5 feature; What it does, and how I implemented it. If you are not interested by the implementation details, skip to the conclusion to see what you can do in Qt 5.5 with `Q_ENUM`.

## The problem

In order to better understand the problem it solves, let us look at this typical sample code using Q_ENUMS as it already could have been written with Qt 4.0.

```
class FooBar : public QObject {
  Q_OBJECT
  Q_ENUMS(Action)
public:
  enum Action { Open, Save, New, Copy, Cut, Paste, Undo, Redo, Delete };

  void myFunction(Action a) {
    qDebug() << "Action is: " << a;
    //...
  }
};
```

But here, the `qDebug` will look like this:
`Action is: 8`
It would be much better if I could see the text instead such as:
`Action is: Delete`

`Q_ENUMS` tells `moc` to register the names of the enum value inside its QMetaObject so that it can be used from Qt Designer, from QtScript or from QML. However it is not working yet with `qDebug`.

One could use the information in the `QMetaObject` while overloading the `operator<<` for QDebug and use QMetaObject's API:

```
QDebug operator<<(QDebug dbg, FooBar::Action action)
{
  static int enumIdx = FooBar::staticMetaObject.indexOfEnumerator("Action");
  return dbg << FooBar::staticMetaObject.enumerator(enumIdx).valueToKey(action);
}
```

That has been working fine since Qt 4.0, but you have to manually write this operator and it is a lot of code that is somehow error prone. Most of Qt's own enumerations did not even have such operator.

## The Solution

I wanted this to be automatic. The problem is that we had no way to get the QMetaObject of the enclosed QObject (or Q_GADGET) associated with a given enumeration. We also need the name of the enumeration to be passed as an argument to `QMetaObject::indexOfEnumerator`.
Let us suppose we have some magic functions that would do exactly that. (We will see later how to make them):

```
QMetaObject` `*qt_getEnumMetaObject(ENUM);``const` `char` `*qt_getEnumName(ENUM);
```

We could then do:

```
template <typename T>
QDebug operator<<(QDebug dbg, T enumValue)
{
    const QMetaObject *mo = qt_getEnumMetaObject(enumValue);
    int enumIdx = mo->indexOfEnumerator(qt_getEnumName(enumValue));
    return dbg << mo->enumerator(enumIdx).valueToKey(enumValue);
}
```

[Argument dependent lookup (ADL)](http://en.cppreference.com/w/cpp/language/adl) will find the right overload for `qt_getEnumMetaObject` and `qt_getEnumName`, and this function will work. The problem is that this template will match any type, even the ones that are not enumerations or that are not registered with `Q_ENUM` for which `qt_getEnumMetaObject(enum)` would not compile. We have to use [SFINAE](http://en.cppreference.com/w/cpp/language/sfinae) (substitution failure is not an error) to enable this operator only if `qt_getEnumMetaObject(enum)` compiles:

```
template <typename T>
typename QtPrivate::QEnableIf<QtPrivate::IsQEnumHelper<T>::Value , QDebug>::Type
operator<<(QDebug dbg, T enumValue)
{
    const QMetaObject *mo = qt_getEnumMetaObject(enumValue);
    int enumIdx = mo->indexOfEnumerator(qt_getEnumName(enumValue));
    return dbg << mo->enumerator(enumIdx).valueToKey(enumValue);
}
```

QEnableIf is the same as [std::enable_if](http://en.cppreference.com/w/cpp/types/enable_if) and [IsQEnumHelper](https://code.woboq.org/qt5/qtbase/src/corelib/kernel/qmetatype.h.html#QtPrivate::IsQEnumHelper) is implemented this way:

```
namespace QtPrivate {
template<typename T> char qt_getEnumMetaObject(const T&);

template<typename T>
struct IsQEnumHelper {
  static const T &declval();
  // If the type was declared with Q_ENUM, the friend qt_getEnumMetaObject()
  // declared in the Q_ENUM macro will be chosen by ADL, and the return type
  // will be QMetaObject*.
  // Otherwise the chosen overload will be the catch all template function
  // qt_getEnumMetaObject(T) which returns 'char'
  enum {
    Value = sizeof(qt_getEnumMetaObject(declval())) == sizeof(QMetaObject*)
  };
};
}
```

So now it all boils down to how to implement the `Q_ENUM` macro to declare this `qt_getEnumMetaObject`.
We need to implement the function `qt_getEnumMetaObject` in the same namespace as the class. Yet, the macro is used within the class. How can we implement the function in the class? Perhaps using some static function or some template magic? No! We are going to use a `friend` function. Indeed, it is possible to define a function in a friend declaration. As an illustration:

```
namespace ABC {
  class FooBar {
    friend int foo() { return 456; }
  };
}
```

`foo` is in the namespace `ABC` (or the global namespace if `FooBar` was not in a namespace). But the interesting fact is that in the body of that function, the lookup is done within the class's scope:

```
class FooBar {
  friend const QMetaObject *getFooBarMetaObject() { return &staticMetaObject; }

  static const QMetaObject staticMetaObject;
};
```

This uses the `staticMetaObject` of the class (as declared in the Q_OBJECT macro). The function can just be called by `getFooBarMetaObject();` (without the `FooBar::` that would be required if it was a static function instead of a friend).
With that we can now construct the `Q_ENUM` macro:

```
#define Q_ENUM(ENUM) \``  ``friend` `constexpr` `const` `QMetaObject` `*qt_getEnumMetaObject(ENUM) noexcept { ``return` `&staticMetaObject; } \``  ``friend` `constexpr` `const` `char` `*qt_getEnumName(ENUM) noexcept { ``return` `#ENUM; }
```

Each instance of this macro will create a new overload of the functions for the given enum type. However, this needs the ENUM type to be declared when we declare the function. Therefore we need to put the Q_ENUM macro after the enum declaration. This also permits only one enum per macro while Q_ENUMS could have several.

(moc will still interpret the `Q_ENUM` macro like the old `Q_ENUMS` macro and generate the same data.)

Using this, I also introduced a new static function [`QMetaEnum::fromType()`](https://doc.qt.io/qt-5/qmetaenum.html#fromType) which let you easily get a QMetaEnum for a given type. This is how it is implemented:

```
template<typename T>
QMetaEnum QMetaEnum::fromType()
{
  const QMetaObject *metaObject = qt_getEnumMetaObject(T());
  const char *name = qt_getEnumName(T());
  return metaObject->enumerator(metaObject->indexOfEnumerator(name));
}
```

We can also integrate it with `QMetaType` to register this type automatically and register the correspding meta object to the metatype system. From that, we can use this information in `QVariant` to convert from a string or to a string.

(Note: The code snippets shown were slightly simplified for the purpose of the blog. Check the real implementation of [the debug `operator<<`](https://code.woboq.org/qt5/qtbase/src/corelib/io/qdebug.h.html#_Z26qt_QMetaEnum_debugOperatorR6QDebugiPK11QMetaObjectPKc), or [`QMetaEnum::fromType`](https://code.woboq.org/qt5/qtbase/src/corelib/kernel/qmetaobject.h.html#_ZN9QMetaEnum8fromTypeEv), or [`QTest::toString`](https://code.woboq.org/qt5/qtbase/src/testlib/qtestcase.h.html#_ZN5QTest8Internal8toStringET_).

## Conclusion

`Q_ENUM` is like the old `Q_ENUMS` but with those differences:

- It needs to be placed after the enum in the source code.
- Only one enum can be put in the macro.
- It enables [`QMetaEnum::fromType()`](https://doc.qt.io/qt-5/qmetaenum.html#fromType).
- These enums are automatically declared as a QMetaTypes (no need to add them in `Q_DECLARE_METATYPE` anymore).
- enums passed to qDebug will print the name of the value rather than the number.
- When put in a QVariant, `toString` gives the value name.
- The value name is printed by [QCOMPARE](https://doc.qt.io/qt-5/qtest.html#QCOMPARE) (from Qt 5.6).

You can read more articles about Qt internals [on our blog](https://woboq.com/blog/).



# 总结

功能上就是增加可以直接打印出枚举的字符串而非一个 int 数值，更直观一点。

后面再来学学别人的思考思路，和实现方法。