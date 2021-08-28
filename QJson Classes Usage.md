# JSON长啥样

> JSON： JavaScript 对象表示法（ JavaScript Object Notation） 是一种轻量级的数据交换格式。 它基于ECMAScript的一个子集。 JSON采用完全独立于语言的文本格式， 但是也使用了类似于C语言家族的习惯（ 包括C、 C++、 C#、 Java、 JavaScript、 Perl、 Python等） 。这些特性使JSON成为理想的数据交换语言。 易于人阅读和编写， 同时也易于机器解析和生成(一般用于提升网络传输速率)。JSON 解析器和 JSON 库支持许多不同的编程语言。 JSON 文本格式在语法上与创建 JavaScript 对象的代码相同。 由于这种相似性， 无需解析器， JavaScript 程序能够使用内建的 eval() 函数， 用 JSON 数据来生成原生的 JavaScript 对象。JSON 是存储和交换文本信息的语法。 类似 XML。 JSON 比 XML 更小、 更快， 更易解析。JSON 具有自我描述性， 语法简洁， 易于理解。

JSON：字符串，可以从里面读数据

花括号{}代表一个对象

中括号[]代表一个数组，里面有多个JSON对象

冒号和引号""=""代表一个键值对，一个对象里有若干个键值对

# QJsonObject 类

就是一个Json对象，里面有若干个键值对，键可以为QString，而值是QJsonValue对象

### 写入操作

```C++
iterator QJsonObject::insert(const QString &key, const QJsonValue &value)
```

插入键为key，值为value的键值对，返回插入键值对的迭代器

### 读取操作

```C++
QJsonValue QJsonObject::value(const QString &key) const
```

查一个键为key的QJsonValue对象

### 关于键的操作

```C++
bool QJsonObject::contains(const QString &key) const
```

查一个键是不是在里面，不在里面就拿不到值

```C++
QStringList QJsonObject::keys() const
```

查一个对象里的所有键，返回一个列表

# QJsonValue 类

一个值可能有6种数据类型 bool, double, string, object, array, null

### 查看这个东西是哪种类型

```C++
QJsonValue::Type type()
```

这个返回一个枚举类型，也可以使用isBool(), isString()这样的返回true或false

### 取数据

检查类型之后，可以使用.toString()，.toObject()函数，把这个QJsonValue转化成对应的数据

# QJsonDocument 类

帮助我们传输的

构造函数 QJsonDocument::QJsonDocument(const QJsonObject &*object*)

然后使用

``` C++
QByteArray QJsonDocument::toJson(QJsonDocument::JsonFormat format)
```

把它变成字符串

如果那个参数填成QJsonDocument::Compact就会变成紧凑的json，没有空格和换行，读起来比较费劲，但可能会有利于传输

对QJsonDocument对象使用.object()，即可提取到里面的对象

> 关于如何从比特串转对象，我个人觉得把它封装到Connection里比较好
