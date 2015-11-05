#include "Service.h"
#include "Connection.h"

#include <thread>
#include <memory>
#include <map>
#include <mutex>

using namespace std;


static map<int, ServicePtr> services;
static mutex servicesMtx;



static void acceptorThread(ServicePtr service)
{
	try {
		service->accept();
	}
	catch (exception& e) {}

	// wait for all users of service to finish and clean up
	while (!service.unique()) {
		this_thread::sleep_for(chrono::milliseconds(100));
	}
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


void Service::send() {
}


void Service::accept()
{
	for(;;) {
		auto connection = make_shared<Connection>(socket.accept());
	}
}


void Service::close()
{
	socket.close();
}



void Service::open(int port)
{
	auto service = make_shared<Service>(port);

	thread acceptor(acceptorThread, service);
	acceptor.detach();

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
	// map: Concurrently accessing other elements is safe.
	return services.at(port);
}

