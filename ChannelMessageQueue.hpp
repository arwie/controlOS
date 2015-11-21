#ifndef CHANNELMESSAGEQUEUE_HPP_
#define CHANNELMESSAGEQUEUE_HPP_


class ChannelMessageQueue : public Channel
{
public:

	void pushMessage(MessagePtr&& message)
	{
		{ lock_guard<mutex> lock(receiveMtx);
			receiveQueue.emplace(move(message));
		}

		receiveCond.notify_all();
	}


	bool receive(MessagePtr& message) override
	{
		unique_lock<mutex> lock(receiveMtx);

		while(receiveQueue.empty()) {
			receiveCond.wait(lock);
		}

		message = move(receiveQueue.front());
		receiveQueue.pop();

		return true;
	}


	void sendSelf(MessagePtr& message) override
	{
		if (!message)
			throw exception();

		pushMessage(move(message));
	}


	void send(MessagePtr& message) override
	{
		sendSelf(message);
	}


protected:

	queue<MessagePtr> receiveQueue;
    mutex receiveMtx;
    condition_variable receiveCond;
};

#endif /* CHANNELMESSAGEQUEUE_HPP_ */
