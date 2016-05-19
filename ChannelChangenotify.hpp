#ifndef CHANNELCHANGENOTIFY_HPP_
#define CHANNELCHANGENOTIFY_HPP_


class ChannelChangenotify : public BlockingChannel, private StatefulChannel
{
public:
	ChannelChangenotify(const Message& args) : Channel("changenotify", args), BlockingChannel(args), StatefulChannel(args)	{}

	void send(const Message& update) override
	{
		lock_guard<mutex> lock(stateMtx);

		for (auto& kv : update)
		{
			auto& key = kv.first;
			auto& newValue = kv.second;

			const auto oldValueIt = state.find(key);

			if (oldValueIt != state.not_found())
			{
				auto& oldValue = oldValueIt->second;

				if (newValue != oldValue) {
					sendChange(key, newValue, oldValue);
					oldValue = newValue;
				}
			}
			else
			{
				sendChange(key, newValue, newValue);
				state.put_child(key, newValue);
			}
		}
 	}


	void reset() override
	{
		BlockingChannel::reset();
	}

private:

	void sendChange(const auto& key, const auto& newValue, const auto& oldValue)
	{
		auto change = make_unique<Message>();
		change->put("key", key);
		change->put_child("value", newValue);
		change->put_child("oldvalue", oldValue);
		pushMessage(move(change));
	}
};

#endif /* CHANNELCHANGENOTIFY_HPP_ */
