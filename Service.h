
#ifndef SERVICE_H_
#define SERVICE_H_

#include "Message.h"
#include "Socket.h"
#include "Connection.h"

#include <memory>
#include <set>
#include <mutex>


class Service;
using ServicePtr = std::shared_ptr<Service>;

class Service {
public:
	Service(int port);
	~Service();

	int connected() const;

	void receive();
	void send();

	void close();

	void addConnection(Connection& connection);
	void removeConnection(Connection& connection);

	static void open(int port);
	static void close(int port);
	static ServicePtr instance(int port);

private:
	Socket socket;
	std::set<Connection*> connections;
	std::mutex connectionsMtx;

	Message msgIn;
	Message msgOut;

	static void acceptThread(ServicePtr service) noexcept;
	static void receiveThread(ServicePtr service, Socket&& socket) noexcept;
};

#endif /* SERVICE_H_ */
