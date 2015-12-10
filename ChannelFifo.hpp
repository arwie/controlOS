#ifndef CHANNELFIFO_HPP_
#define CHANNELFIFO_HPP_


class ChannelFifo : public BlockingChannel
{
public:

	void send(MessagePtr& message) override
	{
		sendSelf(message);
	}

};

#endif /* CHANNELFIFO_HPP_ */
