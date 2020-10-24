#include <iostream>
#include <atlconv.h>
#include <string>
#include <atlstr.h>
#include <sstream>

#include "../../EventBus/event_bus.h"
#include "../Publisher/publisher.h"
#include "../Shared/entities.h"
#include "../Shared/message_token.h"

using namespace std;
using namespace chrono;
using namespace frontier;

/// <summary>
/// 获取事件总线默认实例。
/// </summary>
event_bus& bus = event_bus::get_default();

/// <summary>
/// 生产消息线程。
/// </summary>
thread produce_message;

/// <summary>
/// 发布消息实例。
/// </summary>
publisher pub;

/// <summary>
/// 获取当前时间的字符串。
/// </summary>
/// <returns></returns>
inline string get_current_datetime_string()
{
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	char dtStr[24];
	sprintf_s(dtStr, "%04d-%02d-%02d %02d:%02d:%02d.%03d", sys_time.wYear, sys_time.wMonth, sys_time.wDay,
		sys_time.wHour,
		sys_time.wMinute, sys_time.wSecond, sys_time.wMilliseconds);
	return dtStr;
}

/// <summary>
/// 字符串格式化。
/// </summary>
/// <typeparam name="...Args">模板参数。</typeparam>
/// <param name="format">待格式化字符串。</param>
/// <param name="...args">可变数量参数。</param>
/// <returns>格式化后的字符串。</returns>
template <typename ... Args>
string string_format(const std::string& format, Args ... args)
{
	const size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	const std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

/// <summary>
/// 主函数。
/// </summary>
/// <returns></returns>
int main()
{
	auto subscribe_message = [&](const std::vector<person>& p_vector)
	{
		const auto p = p_vector.back();
		const string str = string_format("%s   name：%s   age：%d   telephone_number：%s", get_current_datetime_string().c_str(),
			p.name.c_str(), p.age, p.telephone_number.c_str());
		cout << str << endl;
	};

	bus.subscribe<std::vector<person>>(send_person_token, subscribe_message);

	produce_message = thread([]()
		{
			while (true)
			{
				pub.publish_message();
				this_thread::sleep_for(milliseconds(20));
			}
		});
	produce_message.detach();

	cin.clear();
	cin.ignore();
	return 0;
}
