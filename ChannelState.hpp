#ifndef CHANNELSTATE_HPP_
#define CHANNELSTATE_HPP_


class ChannelState : public StatefulChannel
{
public:

	void send(MessagePtr& update) override
	{
		lock_guard<mutex> lock(stateMtx);

		for (auto& kv : deref(update))
		{
			auto& key = kv.first;
			auto& newValue = kv.second;

			state.put_child(key, newValue);
		}
	}


	bool receive(MessagePtr& message) override
	{
		lock_guard<mutex> lock(stateMtx);

		message.reset(new Message(state));

		return true;
	}

};

#endif /* CHANNELSTATE_HPP_ */
