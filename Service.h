
#ifndef SERVICE_H_
#define SERVICE_H_

#include "Message.h"
#include "Socket.h"

#include <memory>


class Service;
using ServicePtr = std::shared_ptr<Service>;

class Service {
public:
	Service(int port);
	~Service();

	int connected() const;

	void receive();
	void send();

	void accept();
	void close();

	static void open(int port);
	static void close(int port);
	static ServicePtr instance(int port);

private:
	Socket socket;

	Message msgIn;
	Message msgOut;
};

#endif /* SERVICE_H_ */
