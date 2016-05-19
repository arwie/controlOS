#ifndef CHANNELLOG_HPP_
#define CHANNELLOG_HPP_

#include <sys/syscall.h>


static ChannelPtr channelLog;

static void logMsg(const Log& msg)
{
	ChannelPtr channelLogCopy = channelLog;
	if (!channelLogCopy) return;

	const_cast<Log&>(msg).file("stxmccom");

	channelLogCopy->send(msg);
}


class ChannelLog : public BlockingChannel
{
public:
	ChannelLog(const Message& args)
		: Channel("log", args), BlockingChannel(args),
		  base(args.get_child("base", Message())),
		  priority(args.get<int>("priority", LogInfo::priority))
	{}

	void open() override
	{
		if (channelLog) throw exception();
		channelLog = shared_from_this();

		BlockingChannel::open();
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

		BlockingChannel::close();
	}

private:
	Message base;
	int priority;
};

#endif /* CHANNELLOG_HPP_ */
