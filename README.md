A lightweight event bus that implements publish-subscribe based on C++17.

基于C++17实现发布订阅的轻量级事件总线。

![事件总线](https://raw.githubusercontent.com/LeoYang-Chuese/EventBus/master/Images/EventBus_PublishSubscribe.png)

## 特点

- 用来简化线程间/组件间通信与数据传输，代替Event、Message和Callback Function等
- 基于发布/订阅模式，分离了事件的发送者和接收者，实现业务代码的解耦
- 通过懒汉式线程安全的单例调用发布订阅函数
- 超级轻量的实现，只有一个头(.h)文件和一个实现(.cpp)文件
- 发布订阅函数是线程安全的

## 示例

1.定义一个结构体

```c++
struct person
{
	std::string name;
	std::uint16_t age;
	std::string telephone_number;
};
```

2.定义一个消息令牌

```c++
const std::string send_person_token = "send_person_token";
```

3.实现消息发布

```c++
/// <summary>
/// 获取事件总线默认实例。
/// </summary>
event_bus& bus = event_bus::get_default();

/// <summary>
/// 发布消息。
/// </summary>
void publisher::publish_message() const
{
	person p;
	std::vector<person> p_vector;
	p_vector.reserve(100000);
	for (size_t i = 0; i < 1000; i++)
	{
		p.name = "xiao li zi";
		p.age = 99;
		p.telephone_number = "19911110000";
		p_vector.push_back(std::move(p));
	}

	bus.publish(send_person_token, p_vector);
}
```

4.实现消息订阅

```c++
auto subscribe_message = [&](const std::vector<person>& p_vector)
{
    const auto p = p_vector.back();
    const string str = string_format("%s   name：%s   age：%d   telephone_number：%s", get_current_datetime_string().c_str(),
                                     p.name.c_str(), p.age, p.telephone_number.c_str());
    cout << str << endl;
};

bus.subscribe<std::vector<person>>(send_person_token, subscribe_message);
```

## Licence

该项目根据[MIT许可证授权](https://github.com/LeoYang-Chuese/EventBus/blob/master/LICENSE)。