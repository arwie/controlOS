#ifndef MANAGER_HPP_
#define MANAGER_HPP_


class Manager
{
public:

	int openChannel(ChannelPtr&& channel)
	{
		channel->open();

		if (channel->needsRunner())
			thread(channelRunner, channel).detach();

		lock_guard<mutex> lock(channelsMtx);
		auto channelId = nextChannelId++;
		channels.emplace(channelId, move(channel));

		return channelId;
	}


	ChannelPtr getChannel(int port)
	{
		lock_guard<mutex> lock(channelsMtx);
		return channels.at(port);
	}


	void closeChannel(int channelId)
	{
		unique_lock<mutex> lock(channelsMtx);
		ChannelPtr channel = move(channels.at(channelId));
		channels.erase(channelId);
		lock.unlock();

		channel->close();
	}

	void closeAllChannels()
	{
		lock_guard<mutex> lock(channelsMtx);

		for(auto& kv : channels)
			kv.second->close();

		channels.clear();
	}

private:

	static void channelRunner(ChannelPtr channel) noexcept
	{
		try {
			channel->run();
		}
		catch (exception& e) {
			DEBUG("channelRunner exception: " << e.what());
			// TODO: close channel
		}
		//DEBUG("channelRunner exited");
	}


	map<int, ChannelPtr> channels;
	mutex channelsMtx;
	unsigned int nextChannelId = 1;
};

#endif /* MANAGER_HPP_ */
