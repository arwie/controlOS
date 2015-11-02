
#ifndef SERVICE_H_
#define SERVICE_H_

#include "Message.h"
#include "Socket.h"

#include <map>

class Service {
public:
	Service(int port);
	Service(Service&& rhs) noexcept = default;
	~Service();

	int connected() const;

	int receive();
	void send();


	static void open(int port);
	static void close(int port);
	static Service& instance(int port);

private:
	Socket socket;

	Message msgIn;
	Message msgOut;

	static std::map<int, Service> services;
};

#endif /* SERVICE_H_ */
