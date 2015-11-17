#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <map>
#include <set>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>
#include <exception>

using namespace std;

#include "Message.hpp"
#include "Channel.hpp"
#include "ChannelMessageQueue.hpp"
#include "ChannelServerWebsocket.hpp"
#include "ChannelManager.hpp"

#include "amcs.h"


static ChannelManager manager;


static int stxmccom_open_fifo(int *error) noexcept
{
	try {
		return manager.openChannel(make_shared<ChannelMessageQueue>());
	} catch (exception& e) {
		return -1;
	}
}

static int stxmccom_open_server_sebsocket(int port, int *error) noexcept
{
	*error = 0;
	try {
		return manager.openChannel(make_shared<ChannelServerWebsocket>(port));
	} catch (exception& e) {
		*error = 1;
	}
	return -1;
}

static int stxmccom_connected(int channelId, int *error) noexcept
{
	try {
		return manager.getChannel(channelId)->connected();
	} catch (exception& e) {
	}
		return 0;
}

static void stxmccom_connected_wait(int channelId, int *error) noexcept
{
	try {
	} catch (exception& e) {
	}
}

static int stxmccom_receive(int channelId, int *error) noexcept
{
	*error = 0;
	try {
		return manager.getChannel(channelId)->receive();
	} catch (exception& e) {
		*error = 1;
	}
	return false;
}

static void stxmccom_send(int channelId, int *error) noexcept
{
	*error = 0;
	try {
		manager.getChannel(channelId)->send();
	} catch (exception& e) {
		*error = 1;
	}
}

static int stxmccom_get_long(int channelId, SYS_STRING* path, int *error) noexcept
{
	*error = 0;
	try {
		return manager.getChannel(channelId)->get<int>(amcsGetString(path));
	} catch (exception& e) {
		*error = 1;
	}
	return 0;
}

static double stxmccom_get_double(int channelId, SYS_STRING* path, int *error) noexcept
{
	*error = 0;
	try {
		return manager.getChannel(channelId)->get<double>(amcsGetString(path));
	} catch (exception& e) {
		*error = 1;
	}
	return 0.0;
}

static SYS_STRING* stxmccom_get_string(int channelId, SYS_STRING* path, int *error) noexcept
{
	*error = 0;
	try {
		return amcsNewMcString(manager.getChannel(channelId)->get<string>(amcsGetString(path)));
	} catch (exception& e) {
		*error = 1;
	}
	return amcsNewMcString(string());
}

static void stxmccom_put_long(int channelId, SYS_STRING* path, int value, int *error) noexcept
{
	*error = 0;
	try {
		manager.getChannel(channelId)->put(amcsGetString(path), value);
	} catch (exception& e) {
		*error = 1;
	}
}

static void stxmccom_put_double(int channelId, SYS_STRING* path, double value, int *error) noexcept
{
	*error = 0;
	try {
		manager.getChannel(channelId)->put(amcsGetString(path), value);
	} catch (exception& e) {
		*error = 1;
	}
}

static void stxmccom_put_string(int channelId, SYS_STRING* path, SYS_STRING* value, int *error) noexcept
{
	*error = 0;
	try {
		manager.getChannel(channelId)->put(amcsGetString(path), amcsGetString(value));
	} catch (exception& e) {
		*error = 1;
	}
}

static void stxmccom_close(int channelId) noexcept
{
	try {
		manager.closeChannel(channelId);
	} catch (exception& e) {
	}
}


 
 
extern "C" {

	int STXMCCOM_OPEN_FIFO(int *error) {
		return stxmccom_open_fifo(error);
	}

	int STXMCCOM_OPEN_SERVER_WEBSOCKET(int port, int *error) {
		return stxmccom_open_server_sebsocket(port, error);
	}

	int STXMCCOM_CONNECTED(int channelId, int *error) {
		return stxmccom_connected(channelId, error);
	}

	void STXMCCOM_CONNECTED_WAIT(int channelId, int *error) {
		stxmccom_connected_wait(channelId, error);
	}

	int STXMCCOM_RECEIVE(int channelId, int *error) {
		return stxmccom_receive(channelId, error);
	}

	void STXMCCOM_SEND(int channelId, int *error) {
		stxmccom_send(channelId, error);
	}

	int STXMCCOM_GET_LONG(int channelId, SYS_STRING* path, int *error) {
		return stxmccom_get_long(channelId, path, error);
	}

	double STXMCCOM_GET_DOUBLE(int channelId, SYS_STRING* path, int *error) {
		return stxmccom_get_double(channelId, path, error);
	}

	SYS_STRING* STXMCCOM_GET_STRING(int channelId, SYS_STRING* path, int *error) {
		return stxmccom_get_string(channelId, path, error);
	}

	void STXMCCOM_PUT_LONG(int channelId, SYS_STRING* path, int value, int *error) {
		stxmccom_put_long(channelId, path, value, error);
	}

	void STXMCCOM_PUT_DOUBLE(int channelId, SYS_STRING* path, double value, int *error) {
		stxmccom_put_double(channelId, path, value, error);
	}

	void STXMCCOM_PUT_STRING(int channelId, SYS_STRING* path, SYS_STRING* value, int *error) {
		stxmccom_put_string(channelId, path, value, error);
	}

	void STXMCCOM_CLOSE(int channelId) {
		stxmccom_close(channelId);
	}

}
