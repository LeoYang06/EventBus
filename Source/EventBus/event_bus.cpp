#include "pch.h"
#include "event_bus.h"

using namespace frontier;

/// <summary>
/// 私有构造函数。
/// </summary>
event_bus::event_bus()
= default;

/// <summary>
/// 析构函数。
/// </summary>
event_bus::~event_bus()
= default;

/// <summary>
/// 获取单例。
/// </summary>
/// <returns>唯一实例。</returns>
inline event_bus& event_bus::get_default()
{
	static event_bus instance;
	return instance;
}
