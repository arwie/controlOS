#include "Service.h"
#include "Connection.h"

#include <thread>
#include <memory>
#include <map>
#include <mutex>

using namespace std;


static map<int, ServicePtr> services;
static mutex servicesMtx;



void Service::open(int port)
{
	try {
		close(port);
	} catch (exception& e) {}

	auto service = make_shared<Service>(port);

	thread(acceptThread, service).detach();

	{ lock_guard<mutex> g(servicesMtx);
		services.emplace(port, move(service));
	}
}


void Service::close(int port)
{
	auto service = instance(port);

	{ lock_guard<mutex> g(servicesMtx);
		services.erase(port);
	}

	service->close();
}


ServicePtr Service::instance(int port)
{
	{ lock_guard<mutex> g(servicesMtx);
		return services.at(port);
	}
}



void Service::acceptThread(ServicePtr service) noexcept
{
	try {
		for(;;) {
			thread(receiveThread, service, service->socket.accept()).detach();
		}
	}
	catch (exception& e) {}

	// wait for all users of service to finish and clean up
	while (!service.unique()) {
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}


void Service::receiveThread(ServicePtr service, Socket&& socket) noexcept
{
	try {
		Connection connection(move(socket));
		service->addConnection(connection);

		try {
			for(;;) {
				connection.receive();
			}
		} catch (exception& e) {}

		service->removeConnection(connection);
	}
	catch (exception& e) {}
}




Service::Service(int port)
	:socket(Socket::createListeningSocket(port))
{

}


Service::~Service()
{

}


int Service::connected() const {
	return 0;
}


void Service::receive() {
}


void Service::send()
{
	{ lock_guard<mutex> g(connectionsMtx);
		//iterate over connections
	}
}


void Service::close()
{
	socket.close();

	{ lock_guard<mutex> g(connectionsMtx);
		//iterate over connections
	}
}


void Service::addConnection(Connection& connection)
{
	{ lock_guard<mutex> g(connectionsMtx);
		connections.insert(&connection);
	}
}


void Service::removeConnection(Connection& connection)
{
	{ lock_guard<mutex> g(connectionsMtx);
		connections.erase(&connection);
	}
}
