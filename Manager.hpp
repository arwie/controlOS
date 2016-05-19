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
		channel->logMsg(LogDebug("com runner started").prg("comRunner"));
		try {
			channel->run();
		}
		catch (exception& e) {
			channel->logMsg(LogError("exception in com runner: "+string(e.what())));
		}
		channel->logMsg(LogDebug("com runner finished"));
	}


	map<int, ChannelPtr> channels;
	mutex channelsMtx;
	unsigned int nextChannelId = 1;
};

#endif /* MANAGER_HPP_ */
