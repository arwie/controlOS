
#include "Service.h"


using namespace std;


map<int, Service> Service::services;
mutex servicesMtx;



static void acceptorThread(int port)
{
	try {
		for(;;) {
			//Service::instance(port).socket.accept();
		}
	}
	catch(exception& e) {}
}



Service::Service(int port)
	:socket(Socket::createListeningSocket(port)), acceptor(acceptorThread, port)
{

}


Service::~Service()
{
	if (acceptor.joinable()) {
		try {
			socket.close();
			acceptor.join();
		} catch(exception& e) {}
	}
}


int Service::connected() const {
	return 0;
}


void Service::receive() {
}


void Service::send() {
}



void Service::open(int port)
{
	lock_guard<mutex> g {servicesMtx};
	services.emplace(port, port);
}


void Service::close(int port)
{
	lock_guard<mutex> g {servicesMtx};
	services.erase(port);
}


Service& Service::instance(int port)
{
	return services.at(port);
}

