#ifndef CHANNELCHANGENOTIFY_HPP_
#define CHANNELCHANGENOTIFY_HPP_


class ChannelChangenotify : public BlockingChannel
{
public:

	void send(MessagePtr& update) override
	{
		for (auto& kv : deref(update))
		{
			auto& key = kv.first;
			auto& newValue = kv.second;

			const auto oldValueIt = current.find(key);

			if (oldValueIt != current.not_found())
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
				current.put_child(key, newValue);
			}
		}
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

	Message current;
};

#endif /* CHANNELCHANGENOTIFY_HPP_ */
