/*
* Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
*/

#ifndef CHANNELLOG_HPP_
#define CHANNELLOG_HPP_

#include <sys/syscall.h>


static ChannelPtr channelLog;

static void logMsg(Log&& msg)
{
	ChannelPtr channelLogCopy = channelLog;
	if (!channelLogCopy) return;

	msg.file("mccom");

	channelLogCopy->send(msg);
}


class ChannelLog : public QueuingChannel
{
public:
	ChannelLog(const Message& args)
		: Channel("log", args), QueuingChannel(args),
		  base(args.get_child("base", Message())),
		  priority(args.get<int>("priority", LogInfo::priority))
	{}

	void open() override
	{
		if (channelLog) throw exception();
		channelLog = shared_from_this();

		QueuingChannel::open();
	}

	void send(const Message& message) override
	{
		thread_local pid_t  tid = syscall(SYS_gettid);	// cache tid (reduce syscalls)
		thread_local string prg = message.get<string>("prg", string());

		if (message.get<int>("priority", LogDebug::priority) > priority)
			return;

		auto messagePtr = make_unique<Message>(message);

		for (auto& kv : base)
			messagePtr->put_child(kv.first, kv.second);

		//messagePtr->put("timestamp", chrono::steady_clock::now().time_since_epoch());

		messagePtr->put("tid", tid);

		if (!prg.empty())
			messagePtr->put("prg", prg);

		pushMessage(move(messagePtr));
	}

	void close() override
	{
		channelLog.reset();

		QueuingChannel::close();
	}

private:
	Message base;
	int priority;
};

#endif /* CHANNELLOG_HPP_ */
