#pragma once
#include <algorithm>
#include <string>
#include <functional>
#include <unordered_map>
#include <any>
#include <mutex>

namespace frontier
{
	/// <summary>
	/// 定义UnorderedMultimap类型。
	/// </summary>
	typedef std::unordered_multimap<std::string, std::function<void(std::any)>> unordered_multimap;

	/// <summary>
	/// 基于C++17实现发布订阅的轻量级事件总线。通过懒汉式线程安全的单例调用。
	/// </summary>
	class __declspec(dllexport) event_bus
	{
	private:
		/// <summary>
		/// 私有构造函数。
		/// </summary>
		event_bus();
	public:
		// 析构函数。
		~event_bus();

		/// <summary>
		/// 构造函数。
		/// </summary>
		event_bus(const event_bus&) = delete;

		/// <summary>
		/// 定义赋值操作符。
		/// </summary>
		/// <param>实例的引用。</param>
		/// <returns></returns>
		event_bus& operator=(const event_bus&) = delete;

		/// <summary>
		/// 获取单例。
		/// </summary>
		/// <returns>唯一实例。</returns>
		static event_bus& get_default();

		/// <summary>
		/// 订阅一个token令牌的消息。消息接收回调函数将在收到相应的消息时被调用。
		/// </summary>
		/// <param name="token">消息令牌。</param>
		/// <param name="handler">消息接收回调函数。</param>
		template<typename TMessage, typename EventHandler>
		void subscribe(const std::string& token, EventHandler& handler)
		{
			std::unique_lock<std::mutex> locker(sub_mtx_);
			subscribers_.emplace(token, [func = std::forward<EventHandler>(handler)](auto message){
				func(std::any_cast<TMessage>(message));
			});
		}

		/// <summary>
		/// 订阅一个token令牌的消息。消息接收回调函数将在收到相应的消息时被调用。
		/// </summary>
		/// <param name="token">消息令牌。</param>
		/// <param name="handler">消息接收回调函数。</param>
		template<typename TMessage, typename EventHandler>
		void subscribe(const std::string& token, EventHandler&& handler)
		{
			std::unique_lock<std::mutex> locker(sub_mtx_);
			subscribers_.emplace(token, [func = std::forward<EventHandler>(handler)](auto message){
				func(std::any_cast<TMessage>(message));
			});
		}

		/// <summary>
		/// 发送消息至消息接收者。该消息将被发送至所有订阅接收该消息类型的消息接收者。
		/// </summary>
		/// <param name="token">消息令牌。</param>
		/// <param name="message">消息。</param>
		template<typename TMessage>
		void publish(const std::string& token, const TMessage& message)
		{
			std::unique_lock<std::mutex> locker(pub_mtx_);
			const auto range = subscribers_.equal_range(token);
			std::for_each(range.first, range.second, [message](unordered_multimap::value_type& vt)
				{
					vt.second(std::any_cast<TMessage>(message));
				});
		}

		/// <summary>
		/// 发送消息至消息接收者。该消息将被发送至所有订阅接收该消息类型的消息接收者。
		/// </summary>
		/// <param name="token">消息令牌。</param>
		/// <param name="message">消息。</param>
		template<typename TMessage>
		void publish(const std::string& token, const TMessage&& message)
		{
			std::unique_lock<std::mutex> locker(pub_mtx_);
			const auto range = subscribers_.equal_range(token);
			std::for_each(range.first, range.second, [message](unordered_multimap::value_type& vt)
				{
					vt.second(std::any_cast<TMessage>(message));
				});
		}

	protected:
		/// <summary>
		/// 定义消息订阅者集合。
		/// </summary>
		unordered_multimap subscribers_;

		/// <summary>
		/// 定义消息发布互斥量。
		/// </summary>
		std::mutex pub_mtx_;

		/// <summary>
		/// 定义消息订阅互斥量。
		/// </summary>
		std::mutex sub_mtx_;
	};
}
