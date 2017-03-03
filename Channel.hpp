/*
* Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
*/

#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_


class Channel : public enable_shared_from_this<Channel>
{
public:
	virtual void send(const Message& message)		{ throw exception(); }
	virtual void sendSelf(const Message& message)	{ throw exception(); }

	virtual int receive(MessagePtr& message, chrono::milliseconds timeout)	{ throw exception(); }

	virtual void run()			{}
    virtual bool needsRunner()	{ return false; }

	virtual void open()		{ logMsg(LogInfo("channel opened")); }
	virtual void reset()	{ logMsg(LogInfo("channel reset")); }
	virtual ~Channel()		{ logMsg(LogDebug("channel destroyed")); }

	virtual void close()
	{
		{ lock_guard<mutex> lock(blockMtx);
			closed = true;
		}
		blockCond.notify_all();
		logMsg(LogInfo("channel closed"));
	}

	virtual void logMsg(Log&& msg)
	{
		for (auto& kv : log)
			msg.put_child(kv.first, kv.second);

		::logMsg(move(msg));
	}

protected:
	Channel(const string& channelName, const Message& args)
		: log(args.get_child("log", Message()))
	{
		log.put("channel", channelName);
	}

	bool closed = false;
	mutex blockMtx;
	condition_variable blockCond;
	Message log;
};



class QueuingChannel : public virtual Channel
{
public:

	int receive(MessagePtr& message, chrono::milliseconds timeout) override
	{
		unique_lock<mutex> lock(blockMtx);

		if (timeout.count() > 0) {
			blockCond.wait_for(lock, timeout, [this]() { return !receiveQueue.empty() || closed; });
		}

		if (receiveQueue.empty() || closed)
			return false;

		message = move(receiveQueue.front());
		receiveQueue.pop();

		return message->event;
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


protected:
	QueuingChannel(const Message& args) : Channel("", args)	{}

	void pushMessage(MessagePtr&& message)
	{
		{ lock_guard<mutex> lock(blockMtx);
			receiveQueue.emplace(move(message));
		}
		blockCond.notify_all();
	}


private:
	queue<MessagePtr> receiveQueue;
};


#endif /* CHANNEL_HPP_ */
