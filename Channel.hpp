#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_


class Channel
{
public:

	virtual int connected()		{ return true; }
	virtual int connectedWait()	{ return true; }

	virtual bool receive(MessagePtr& message)								{ throw exception(); }
	virtual bool receive(MessagePtr& message, chrono::milliseconds timeout)	{ throw exception(); }

	virtual void send(MessagePtr& message)		{ throw exception(); }
	virtual void sendSelf(MessagePtr& message)	{ throw exception(); }

	virtual void run()			{}
    virtual bool needsRunner()	{ return false; }

	virtual void close()	{}
	virtual ~Channel()		{}

protected:
	Channel()	{}
};



class BlockingChannel : public Channel
{
public:

	int connected() override
	{
		return connections;
	}

	int connectedWait() override
	{
		unique_lock<mutex> lock(blockMtx);
		while(!connections && !closed) {
			blockCond.wait(lock);
		}
		return connections;
	}


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


	void close() override
	{
		{ lock_guard<mutex> lock(blockMtx);
			closed = true;
		}
		blockCond.notify_all();
	}


protected:

	void setConnected(int conn)
	{
		{ lock_guard<mutex> lock(blockMtx);
			connections = conn;
		}
		blockCond.notify_all();
	}

	void pushMessage(MessagePtr&& message)
	{
		{ lock_guard<mutex> lock(blockMtx);
			receiveQueue.emplace(move(message));
		}
		blockCond.notify_all();
	}


private:
	enum { poll = 0 };

	bool closed = false;
	int connections = 0;
	queue<MessagePtr> receiveQueue;
    mutex blockMtx;
    condition_variable blockCond;
};

#endif /* CHANNEL_HPP_ */
