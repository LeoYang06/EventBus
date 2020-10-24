#pragma once

/// <summary>
/// 消息发布器。
/// </summary>
class __declspec(dllexport) publisher
{
public:
	/// <summary>
	/// 发布消息。
	/// </summary>
	void publish_message() const;
};
