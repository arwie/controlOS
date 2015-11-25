#ifndef CHANNELFIFO_HPP_
#define CHANNELFIFO_HPP_


class ChannelFifo : public BlockingChannel
{
public:

	void sendSelf(MessagePtr& message) override
	{
		if (!message)
			throw exception();

		pushMessage(move(message));
	}


	void send(MessagePtr& message) override
	{
		sendSelf(message);
	}

};

#endif /* CHANNELFIFO_HPP_ */
