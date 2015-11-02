
#include "Service.h"

using namespace std;


map<int, Service> Service::services;


Service::Service(int port)
	:socket(Socket::createListeningSocket(port))
{

}


Service::~Service() {
	// TODO Auto-generated destructor stub
}


int Service::connected() const {
}


int Service::receive() {
}


void Service::send() {
}



void Service::open(int port)
{
	services.emplace(port, port);
}


void Service::close(int port)
{
	services.erase(port);
}


Service& Service::instance(int port)
{
	return services.at(port);
}

