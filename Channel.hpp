#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_


class Channel
{
public:

	virtual int connected()		{ return conn; }
	virtual int connectedWait()
	{
		unique_lock<mutex> lock(connMtx);
		while(!connected()) {	// TODO: exit on close
			connCond.wait(lock);
		}
		return connected();
	}

	virtual bool receive(MessagePtr& message)	{ return false; }

	virtual void send(MessagePtr& message)		{}
	virtual void sendSelf(MessagePtr& message)	{ throw exception(); }

	virtual void run()			{}
    virtual bool needsRunner()	{ return false; }

	virtual void close()	{}
	virtual ~Channel()		{}

protected:
	Channel()	{}

	void setConnected(int connected)
	{
		{ lock_guard<mutex> lock(connMtx);
			conn = connected;
		}
		connCond.notify_all();
	}

private:
	int conn = true;
    mutex connMtx;
    condition_variable connCond;
};

#endif /* CHANNEL_HPP_ */
