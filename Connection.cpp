#include "Connection.h"

#include <iostream>
#include <array>
#include <regex>
#include <utility>

using namespace std;



Connection::Connection(Socket&& socket)
	:socket(move(socket))
{
	cerr << "new connection" << endl;
	// TODO Handshake

//	regex e(".*a(.*)?e(.*)");
//	smatch match;
//	array<char, 1500> buffer;
//
//	do {
//		inBuffer.append(buffer.data(), socket.receive(buffer.data(), buffer.size()));
//	}
//	while (regex_match (inBuffer, match, e));
//
//	inBuffer = match[2];
//	return Message(match[1]);

}


Message Connection::receive()
{
	array<char, 1500> buffer;

	do {
		inBuffer.append(buffer.data(), socket.receive(buffer.data(), buffer.size()));
	}
	while (inBuffer.find(0xFF));

	return Message(match[1]);
}


void Connection::send(const Message& message) {
}


void Connection::close()
{
	socket.close();
}
