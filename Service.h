
#ifndef SERVICE_H_
#define SERVICE_H_

#include "Message.h"
#include "Socket.h"

#include <thread>
#include <map>
#include <mutex>


class Service final {
public:
	Service(int port);
	Service(Service&& rhs) noexcept = default;
	~Service();

	int connected() const;

	void receive();
	void send();


	static void open(int port);
	static void close(int port);
	static Service& instance(int port);

private:
	Socket socket;
	std::thread acceptor;

	Message msgIn;
	Message msgOut;

	static std::map<int, Service> services;
	static std::mutex servicesMtx;
};

#endif /* SERVICE_H_ */
