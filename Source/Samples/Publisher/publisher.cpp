#include "pch.h"
#include "publisher.h"

#include "../../EventBus/event_bus.h"
#include "../Shared/entities.h"
#include "../Shared/message_token.h"

using namespace frontier;

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
