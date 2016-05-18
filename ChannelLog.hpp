#ifndef CHANNELLOG_HPP_
#define CHANNELLOG_HPP_


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
		: Channel(args), BlockingChannel(args),
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
		if (message.get<int>("priority", LogDebug::priority) > priority)
			return;

		auto messagePtr = make_unique<Message>(message);

		for (auto& kv : base)
			messagePtr->put_child(kv.first, kv.second);

		//messagePtr->put("timestamp", chrono::steady_clock::now().time_since_epoch());
		//messagePtr->put("pid", pid);

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
