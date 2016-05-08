#ifndef CHANNELLOG_HPP_
#define CHANNELLOG_HPP_


static ChannelPtr channelLog;

static void logMessage(Message& message)
{
	ChannelPtr channelLogCopy = channelLog;
	if (!channelLogCopy) return;

	channelLogCopy->send(message);
}


class ChannelLog : public BlockingChannel
{
public:

	void open() override
	{
		if (channelLog) throw exception();
		channelLog = shared_from_this();
	}

	void send(const Message& message) override
	{
		sendSelf(message);
	}

	void close() override
	{
		channelLog.reset();

		BlockingChannel::close();
	}

};

#endif /* CHANNELLOG_HPP_ */
