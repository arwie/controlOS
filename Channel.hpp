#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_


class Channel
{
public:

	virtual int connected() const { return 0; }

	virtual bool receive(MessagePtr& message) { return false; }

	virtual void send(MessagePtr& message) {}
	virtual void sendSelf(MessagePtr& message) { throw exception(); }

	virtual void close() {}

	virtual void run() {}
    virtual bool needsRunner() { return false; }

	virtual ~Channel() {}

protected:
	Channel() {}

};
#endif /* CHANNEL_HPP_ */
