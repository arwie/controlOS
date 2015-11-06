#include "Connection.h"

#include <utility>

using namespace std;



Connection::Connection(Socket&& socket)
	:socket(move(socket))
{
	// TODO Handshake

}


Message Connection::receive()
{
	return Message();
}


void Connection::send(const Message& message) {
}


void Connection::close()
{
	socket.close();
}
