# NeuTalk 聊天应用 开发者README

目录：

- Qt JSON数据的使用方法
- 注册流程
- 登录流程
- 聊天流程
- 其他请求
- 待实现特性





## Qt JSON数据的使用方法

### JSON长啥样

> JSON： JavaScript 对象表示法（ JavaScript Object Notation） 是一种轻量级的数据交换格式。 它基于ECMAScript的一个子集。 JSON采用完全独立于语言的文本格式， 但是也使用了类似于C语言家族的习惯（ 包括C、 C++、 C#、 Java、 JavaScript、 Perl、 Python等） 。这些特性使JSON成为理想的数据交换语言。 易于人阅读和编写， 同时也易于机器解析和生成(一般用于提升网络传输速率)。JSON 解析器和 JSON 库支持许多不同的编程语言。 JSON 文本格式在语法上与创建 JavaScript 对象的代码相同。 由于这种相似性， 无需解析器， JavaScript 程序能够使用内建的 eval() 函数， 用 JSON 数据来生成原生的 JavaScript 对象。JSON 是存储和交换文本信息的语法。 类似 XML。 JSON 比 XML 更小、 更快， 更易解析。JSON 具有自我描述性， 语法简洁， 易于理解。

JSON：字符串，可以从里面读数据

花括号{}代表一个对象

中括号[]代表一个数组，里面有多个JSON对象

冒号和引号""=""代表一个键值对，一个对象里有若干个键值对

### QJsonObject 类

就是一个Json对象，里面有若干个键值对，键可以为QString，而值是QJsonValue对象

#### 写入操作

```C++
iterator QJsonObject::insert(const QString &key, const QJsonValue &value)
```

插入键为key，值为value的键值对，返回插入键值对的迭代器

#### 读取操作

```C++
QJsonValue QJsonObject::value(const QString &key) const
```

查一个键为key的QJsonValue对象

#### 关于键的操作

```C++
bool QJsonObject::contains(const QString &key) const
```

查一个键是不是在里面，不在里面就拿不到值

```C++
QStringList QJsonObject::keys() const
```

查一个对象里的所有键，返回一个列表

### QJsonValue 类

一个值可能有6种数据类型 bool, double, string, object, array, null

#### 查看这个东西是哪种类型

```C++
QJsonValue::Type type()
```

这个返回一个枚举类型，也可以使用isBool(), isString()这样的返回true或false

#### 取数据

检查类型之后，可以使用.toString()，.toObject()函数，把这个QJsonValue转化成对应的数据

### QJsonDocument 类

帮助我们传输的

构造函数 QJsonDocument::QJsonDocument(const QJsonObject &*object*)

然后使用

``` C++
QByteArray QJsonDocument::toJson(QJsonDocument::JsonFormat format)
```

把它变成字符串

如果那个参数填成QJsonDocument::Compact就会变成紧凑的json，没有空格和换行，读起来比较费劲，但可能会有利于传输





## 注册流程

#### 每个标号代表一个报文类型！

1. C>S 请求注册，账号邮箱，请求发送验证邮件
2. S>C 前提1，请求已收到，正在发送
3. S>C 前提2，发送验证邮件成功，记录该邮箱+验证码对应的datetime
4. C>S 前提-，请求注册，包含邮箱+验证码+密码散列值
5. S>C 前提4，注册失败，返回失败原因
6. S>C 前提4，注册成功，服务器返回账户uid



## 登录流程

#### 每个标号代表一个报文类型！

1. C>S 请求登录，提交邮箱/uid+密码散列值
2. S>C 前提1，服务器已收到登录请求，等待进一步接收消息
3. S>C 前提1，登录失败，返回失败原因
4. S>C 前提1，登录成功，等待同步数据
5. S>C 前提4，同步数据（用户个人profile+好友列表+群组列表）
6. S>C 前提5，同步数据完成



## 聊天流程

#### 每个标号代表一个报文类型！

对话指自己和一个好友，或自己所处的一个群组，对话用uid标识

1. C>S 打开对话，请求同步历史消息

2. S>C 前提1，同步特定对话的历史消息列表

3. C>S 前提-，发送聊天消息至某个对话

   服务端检查发送者和对话的有效性，然后将聊天消息转发至对话中的其他用户，最后将它保存到服务端数据库

4. S>C 前提-，用户接收到聊天消息



## 其他请求

- 拉取好友的个人profile
- 拉取群组信息
- 拉取好友在线信息
- 更新个人profile



## 待实现特性Features

。。。
