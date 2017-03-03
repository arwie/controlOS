/*
* Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
*/

#ifndef CHANNELSTATE_HPP_
#define CHANNELSTATE_HPP_


class ChannelState : public Channel
{
public:
	ChannelState(const Message& args) : Channel("state", args)	{}

	int receive(MessagePtr& message, chrono::milliseconds timeout) override
	{
		unique_lock<mutex> lock(blockMtx);

		if (timeout.count() > 0) {
			blockCond.wait_for(lock, timeout, [this]() { return state || closed; });
		}

		if (!state || closed)
			return false;

		message = make_unique<Message>(*state);

		return message->event;
	}


	void send(const Message& message) override
	{
		{ lock_guard<mutex> lock(blockMtx);
			state = make_unique<Message>(message);
		}
		blockCond.notify_all();
	}


	void reset() override
	{
		{ lock_guard<mutex> lock(blockMtx);
			state.reset();
		}
		Channel::reset();
	}


	MessagePtr state;
};

#endif /* CHANNELSTATE_HPP_ */
