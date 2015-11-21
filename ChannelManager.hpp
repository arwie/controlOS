#ifndef CHANNELMANAGER_HPP_
#define CHANNELMANAGER_HPP_


class ChannelManager
{
	using ChannelPtr = shared_ptr<Channel>;

public:

	int openChannel(ChannelPtr&& channel)
	{
		if (channel->needsRunner())
			thread(channelRunner, channel).detach();

		lock_guard<mutex> lock(channelsMtx);
		auto channelId = nextChannelId++;
		channels.emplace(channelId, move(channel));

		return channelId;
	}


	void closeChannel(int channelId)
	{
		unique_lock<mutex> lock(channelsMtx);
		ChannelPtr channel = move(channels.at(channelId));
		channels.erase(channelId);
		lock.unlock();

		channel->close();
	}


	ChannelPtr getChannel(int port)
	{
		lock_guard<mutex> lock(channelsMtx);
		return channels.at(port);
	}


private:

	static void channelRunner(ChannelPtr channel) noexcept
	{
		try {
			channel->run();
		}
		catch (exception& e) {
			// TODO: close channel
		}
	}


	map<int, ChannelPtr> channels;
	mutex channelsMtx;
	unsigned int nextChannelId = 1;
};

#endif /* CHANNELMANAGER_HPP_ */
