#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>
#include <exception>
#include <boost/regex.hpp>

using namespace std;

#define DEBUG(x)	if (true) { cerr << x << endl; }


class Message; using MessagePtr = unique_ptr<Message>;
class Channel; using ChannelPtr = shared_ptr<Channel>;

class Log;
static void logMsg(const Log&);


#include "Message.hpp"
#include "Log.hpp"
#include "Channel.hpp"
#include "ChannelLog.hpp"
#include "ChannelShell.hpp"
#include "ChannelState.hpp"
#include "ChannelFile.hpp"
#include "ChannelFifo.hpp"
#include "ChannelChangenotify.hpp"
#include "ChannelServerWebsocket.hpp"
#include "ChannelClientCncHaas.hpp"
#include "Manager.hpp"

#include "amcs.h"


static Manager manager;
thread_local MessagePtr messagePtr(new Message());
thread_local stack<MessagePtr> messageStack;


static int stxmccom_open(int channelType, int *error) noexcept
{
	*error = 0;
	try {
		switch(channelType) {
			case  1: return manager.openChannel(make_shared<ChannelLog>				(*messagePtr));
			case  2: return manager.openChannel(make_shared<ChannelShell>			(*messagePtr));
			case  3: return manager.openChannel(make_shared<ChannelState>			(*messagePtr));
			case  4: return manager.openChannel(make_shared<ChannelFifo>			(*messagePtr));
			case  5: return manager.openChannel(make_shared<ChannelChangenotify>	(*messagePtr));
			case  6: return manager.openChannel(make_shared<ChannelFileRead>		(*messagePtr));
			case  7: return manager.openChannel(make_shared<ChannelFileWrite>		(*messagePtr));
			case  8: return manager.openChannel(make_shared<ChannelServerWebsocket>	(*messagePtr));
			case  9: return manager.openChannel(make_shared<ChannelClientCncHaas>	(*messagePtr));
			default: throw exception();
		}
	} catch (exception& e) {
		*error = 1;
	}
	return -1;
}

static int stxmccom_connected(int channelId, int *error) noexcept
{
	*error = 0;
	try {
		return manager.getChannel(channelId)->connected();
	} catch (exception& e) {
		*error = 1;
	}
	return false;
}

static int stxmccom_connected_timed(int channelId, chrono::milliseconds timeout, int *error) noexcept
{
	*error = 0;
	try {
		return manager.getChannel(channelId)->connected(timeout);
	} catch (exception& e) {
		*error = 1;
	}
	return false;
}

static int stxmccom_receive(int channelId, int *error) noexcept
{
	*error = 0;
	try {
		return manager.getChannel(channelId)->receive(messagePtr);
	} catch (exception& e) {
		*error = 1;
	}
	return false;
}

static int stxmccom_receive_timed(int channelId, chrono::milliseconds timeout, int *error) noexcept
{
	*error = 0;
	try {
		return manager.getChannel(channelId)->receive(messagePtr, timeout);
	} catch (exception& e) {
		*error = 1;
	}
	return false;
}

static void stxmccom_send(int channelId, int *error) noexcept
{
	*error = 0;
	try {
		manager.getChannel(channelId)->send(*messagePtr);
	} catch (exception& e) {
		*error = 1;
		DEBUG("stxmccom_send: " << e.what());
	}
}

static void stxmccom_send_self(int channelId, int *error) noexcept
{
	*error = 0;
	try {
		manager.getChannel(channelId)->sendSelf(*messagePtr);
	} catch (exception& e) {
		*error = 1;
	}
}

static void stxmccom_reset(int channelId) noexcept
{
	try {
		manager.getChannel(channelId)->reset();
	} catch (exception& e) {}
}

static void stxmccom_close(int channelId) noexcept
{
	try {
		manager.closeChannel(channelId);
	} catch (exception& e) {}
}

static void stxmccom_close_all() noexcept
{
	try {
		manager.closeAllChannels();
	} catch (exception& e) {}
}


static void stxmccom_clear(int *error) noexcept
{
	*error = 0;
	try {
		messagePtr->clear();
	} catch (exception& e) {
		*error = 1;
	}
}

static int stxmccom_empty(int *error) noexcept
{
	*error = 0;
	try {
	 	return messagePtr->empty();
	} catch (exception& e) {
		*error = 1;
	}
	return true;
}


static void stxmccom_store(bool copy, int *error) noexcept
{
	*error = 0;
	try {
		if (messageStack.size() >= 5)
			throw exception();

		MessagePtr newMessagePtr(copy ? new Message(*messagePtr) : new Message());
	 	messageStack.emplace(move(messagePtr));
	 	messagePtr = move(newMessagePtr);
	} catch (exception& e) {
		*error = 1;
	}
}

static void stxmccom_restore(int *error) noexcept
{
	*error = 0;
	try {
		messagePtr = move(messageStack.top());
		messageStack.pop();
	} catch (exception& e) {
		*error = 1;
	}
}


static void stxmccom_delete(SYS_STRING* path) noexcept
{
	try {
		messagePtr->erase_with(amcsGetString(path));
	}
	catch (exception& e) {}
}


static void stxmccom_with(SYS_STRING* path) noexcept
{
	try {
		messagePtr->with(amcsGetString(path));
	}
	catch (exception& e) {}
}


static void stxmccom_extract(SYS_STRING* path, int *error) noexcept
{
	*error = 0;
	try {
		messagePtr.reset(new Message(messagePtr->get_child_with(amcsGetString(path))));
	} catch (exception& e) {
		*error = 1;
	}
}


static void stxmccom_receive_string(SYS_STRING* str, int *error) noexcept
{
	*error = 0;
	try {
		messagePtr.reset(new Message(amcsGetString(str)));
	} catch (exception& e) {
		*error = 1;
	}
}

static string stxmccom_send_string(int *error) noexcept
{
	*error = 0;
	string str;
	try {
		str = messagePtr->toString();
	} catch (exception& e) {
		*error = 1;
	}
	return str;
}


template<class Type>
static Type stxmccom_get(SYS_STRING* path, int *error) noexcept
{
	*error = 0;
	try {
	 	return messagePtr->get_with<Type>(amcsGetString(path));
	} catch (exception& e) {
		*error = 1;
	}
	return Type();
}


template<class Type>
static void stxmccom_put(SYS_STRING* path, Type value, int *error) noexcept
{
	*error = 0;
	try {
		messagePtr->put_with(amcsGetString(path), value);
	} catch (exception& e) {
		*error = 1;
	}
}

static void stxmccom_put_string(SYS_STRING* path, SYS_STRING* value, int *error) noexcept
{
	try {
		stxmccom_put(path, amcsGetString(value), error);
	} catch (exception& e) {
		*error = 1;
	}
}



 
 
extern "C" {

	int STXMCCOM_OPEN(int channelType, int *error) {
		return stxmccom_open(channelType, error);
	}

	int STXMCCOM_CONNECTED(int channelId, int *error) {
		return stxmccom_connected(channelId, error);
	}

	int STXMCCOM_CONNECTED_TIMED(int channelId, int timeout, int *error) {
		return stxmccom_connected_timed(channelId, chrono::milliseconds(timeout), error);
	}

	int STXMCCOM_RECEIVE(int channelId, int *error) {
		return stxmccom_receive(channelId, error);
	}

	int STXMCCOM_RECEIVE_TIMED(int channelId, int timeout, int *error) {
		return stxmccom_receive_timed(channelId, chrono::milliseconds(timeout), error);
	}

	void STXMCCOM_SEND(int channelId, int *error) {
		stxmccom_send(channelId, error);
	}

	void STXMCCOM_SEND_SELF(int channelId, int *error) {
		stxmccom_send_self(channelId, error);
	}

	void STXMCCOM_RESET(int channelId) {
		stxmccom_reset(channelId);
	}

	void STXMCCOM_CLOSE(int channelId) {
		stxmccom_close(channelId);
	}

	void STXMCCOM_CLOSE_ALL(void) {
		stxmccom_close_all();
	}

	void STXMCCOM_CLEAR(int *error) {
		stxmccom_clear(error);
	}

	int STXMCCOM_EMPTY(int *error) {
		return stxmccom_empty(error);
	}

	void STXMCCOM_STORE(int copy, int *error) {
		stxmccom_store(copy, error);
	}

	void STXMCCOM_RESTORE(int *error) {
		stxmccom_restore(error);
	}

	void STXMCCOM_DELETE(SYS_STRING** path) {
		stxmccom_delete(*path);
	}

	void STXMCCOM_WITH(SYS_STRING** path) {
		stxmccom_with(*path);
	}

	void STXMCCOM_EXTRACT(SYS_STRING** path, int *error) {
		stxmccom_extract(*path, error);
	}

	void STXMCCOM_RECEIVE_STRING(SYS_STRING** str, int *error) {
		stxmccom_receive_string(*str, error);
	}

	SYS_STRING* STXMCCOM_SEND_STRING(int *error) {
		auto str = stxmccom_send_string(error);
		return str_GetString((unsigned char*)str.c_str(), str.length(), ASCII8_STRING_TYPE);
	}

	int STXMCCOM_GET_LONG(SYS_STRING** path, int *error) {
		return stxmccom_get<int>(*path, error);
	}

	double STXMCCOM_GET_DOUBLE(SYS_STRING** path, int *error) {
		return stxmccom_get<double>(*path, error);
	}

	SYS_STRING* STXMCCOM_GET_STRING(SYS_STRING** path, int *error)
	{
		auto str = stxmccom_get<string>(*path, error);
		return str_GetString((unsigned char*)str.c_str(), str.length(), ASCII8_STRING_TYPE);
	}

	void STXMCCOM_PUT_LONG(SYS_STRING** path, int value, int *error) {
		stxmccom_put(*path, value, error);
	}

	void STXMCCOM_PUT_DOUBLE(SYS_STRING** path, double value, int *error) {
		stxmccom_put(*path, value, error);
	}

	void STXMCCOM_PUT_STRING(SYS_STRING** path, SYS_STRING** value, int *error) {
		stxmccom_put_string(*path, *value, error);
	}

}
