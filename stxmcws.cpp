#include "Service.h"

#include <iostream>

using namespace std;





static int stxmcws_open(int service) noexcept
{
	try {
		Service::open(service);
		return 0;
	} catch (exception& e) {
		return -1;
	}
}

static int stxmcws_connected(int service) noexcept
{
	try {
		return Service::instance(service).connected();
	} catch (exception& e) {
		return -1;
	}
}

static int stxmcws_close(int service) noexcept
{
	Service::close(service);
	return 0;
}

static int stxmcws_receive(int service) noexcept
{
	try {
		return Service::instance(service).receive();
	} catch (exception& e) {
		return -1;
	}
}

static int stxmcws_send(int service) noexcept
{
	try {
		Service::instance(service).send();
		return 0;
	} catch (exception& e) {
		return -1;
	}
}

static int stxmcws_read_long(int service, long *value) noexcept
{

	return 0;
}

static int stxmcws_read_double(int service, double *value) noexcept
{

	return 0;
}

static int stxmcws_write_long(int service, long value) noexcept
{

	return 0;
}

static int stxmcws_write_double(int service, double value) noexcept
{

	return 0;
}


 
 
extern "C" {

int STXMCWS_OPEN(int service) {
	return stxmcws_open(service);
}
int STXMCWS_CONNECTED(int service) {
	return stxmcws_connected(service);
}

int STXMCWS_CLOSE(int service) {
	return stxmcws_close(service);
}

int STXMCWS_RECEIVE(int service) {
	return stxmcws_receive(service);
}

int STXMCWS_SEND(int service) {
	return stxmcws_send(service);
}

int STXMCWS_READ_LONG(int service, long *value) {
	return stxmcws_read_long(service, value);
}

int STXMCWS_READ_DOUBLE(int service, double *value) {
	return stxmcws_read_double(service, value);
}

int STXMCWS_WRITE_LONG(int service, long value) {
	return stxmcws_write_long(service, value);
}

int STXMCWS_WRITE_DOUBLE(int service, double value) {
	return stxmcws_write_double(service, value);
}

}
