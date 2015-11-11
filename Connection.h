/*
 * Connection.h
 *
 *  Created on: 01.11.2015
 *      Author: client
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Socket.h"
#include "Message.h"

#include <string>


class Connection {
public:
	Connection(Socket&& socket);

	Message receive();
	void send(const Message& message);

	void close();

private:
	Socket socket;
	std::string inBuffer;
};

#endif /* CONNECTION_H_ */
