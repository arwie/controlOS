/*
* Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
*/

#ifndef CHANNELFIFO_HPP_
#define CHANNELFIFO_HPP_


class ChannelFifo : public BlockingChannel
{
public:
	ChannelFifo(const Message& args) : Channel("fifo", args), BlockingChannel(args)	{}

	void send(const Message& message) override
	{
		sendSelf(message);
	}

};

#endif /* CHANNELFIFO_HPP_ */
