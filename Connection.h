/*
 * Connection.h
 *
 *  Created on: 01.11.2015
 *      Author: client
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Socket.h"

#include <memory>


class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

class Connection {
public:
	Connection(Socket&& socket);
	~Connection();

private:
	Socket socket;
};

#endif /* CONNECTION_H_ */
