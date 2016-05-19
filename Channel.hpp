#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_


class Channel : public enable_shared_from_this<Channel>
{
public:
	virtual void send(const Message& message)		{ throw exception(); }
	virtual void sendSelf(const Message& message)	{ throw exception(); }

	virtual bool receive(MessagePtr& message)								{ throw exception(); }
	virtual bool receive(MessagePtr& message, chrono::milliseconds timeout)	{ throw exception(); }

	virtual bool connected()								{ throw exception(); }
	virtual bool connected(chrono::milliseconds timeout)	{ throw exception(); }

	virtual void run()			{}
    virtual bool needsRunner()	{ return false; }

	virtual void open()		{ logMsg(LogInfo("channel opened")); }
	virtual void reset()	{ logMsg(LogInfo("channel reset")); }
	virtual void close()	{ logMsg(LogInfo("channel closed")); }
	virtual ~Channel()		{ logMsg(LogDebug("channel destroyed")); }

	virtual void logMsg(const Log& msg)
	{
		for (auto& kv : log)
			const_cast<Log&>(msg).put_child(kv.first, kv.second);

		::logMsg(msg);
	}

protected:
	Channel(const string& channelName, const Message& args)
		: log(args.get_child("log", Message()))
	{
		log.put("channel", channelName);
	}

	Message log;
};



class StatefulChannel : public virtual Channel
{
public:

	void reset() override
	{
		{ lock_guard<mutex> lock(stateMtx);
			state.clear();
		}
		Channel::reset();
	}

protected:
	StatefulChannel(const Message& args) : Channel("", args)	{}

	Message state;
	mutex stateMtx;
};



class BlockingChannel : public virtual Channel
{
public:

	bool receive(MessagePtr& message) override { return receive(message, chrono::milliseconds(poll)); }

	bool receive(MessagePtr& message, chrono::milliseconds timeout) override
	{
		unique_lock<mutex> lock(blockMtx);

		if (timeout.count() != poll) {
			blockCond.wait_for(lock, timeout, [this]() { return !receiveQueue.empty() || closed; });
		}

		if (receiveQueue.empty() || closed)
			return false;

		message = move(receiveQueue.front());
		receiveQueue.pop();

		return true;
	}


	void sendSelf(const Message& message) override
	{
		pushMessage(make_unique<Message>(message));
	}


	void reset() override
	{
		queue<MessagePtr> emptyQueue;

		{ lock_guard<mutex> lock(blockMtx);
			swap(receiveQueue, emptyQueue);
		}

		Channel::reset();
	}

	void close() override
	{
		{ lock_guard<mutex> lock(blockMtx);
			closed = true;
		}
		blockCond.notify_all();
		Channel::close();
	}


protected:
	BlockingChannel(const Message& args) : Channel("", args)	{}

	enum { poll = 0 };

	void pushMessage(MessagePtr&& message)
	{
		{ lock_guard<mutex> lock(blockMtx);
			receiveQueue.emplace(move(message));
		}
		blockCond.notify_all();
	}

	bool closed = false;
	mutex blockMtx;
	condition_variable blockCond;


private:
	queue<MessagePtr> receiveQueue;
};



class ConnectingChannel : public BlockingChannel
{
public:

	bool connected() override { return connected(chrono::milliseconds(poll)); }

	bool connected(chrono::milliseconds timeout) override
	{
		unique_lock<mutex> lock(blockMtx);

		if (timeout.count() != poll) {
			blockCond.wait_for(lock, timeout, [this]() { return isConnected || closed; });
		}

		return isConnected;
	}


protected:
	ConnectingChannel(const Message& args) : Channel("", args), BlockingChannel(args)	{}

	void setConnected(bool connected)
	{
		{ lock_guard<mutex> lock(blockMtx);
			isConnected = connected;
		}
		blockCond.notify_all();
	}


private:
	bool isConnected = false;
};

#endif /* CHANNEL_HPP_ */
