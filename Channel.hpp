#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_


class Channel
{
public:

	virtual bool receive() { return false; }

	virtual void send()
	{

	}

	virtual void sendSelf() { throw exception(); }

	virtual void close()
	{

	}

	virtual int connected() const {
		return 0;
	}

	virtual void run() {}
    virtual bool needsRunner() { return false; }

    template<class Type>
    Type get(const string& path) const
    {
    	if (!messageRcv)
    		throw exception();

     	return messageRcv->get<Type>(path);
   }

    template<class Type>
    void put(const string& path, const Type& value)
    {
    	if (!messageSnd)
    		throw exception();

    	messageSnd->put(path, value);
    }

	virtual ~Channel() { }

protected:
	Channel() { }

	using MessagePtr = unique_ptr<Message>;
    MessagePtr messageRcv;
    MessagePtr messageSnd;

};
#endif /* CHANNEL_HPP_ */
