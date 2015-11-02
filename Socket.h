/*
 * Socket.h
 *
 *  Created on: 01.11.2015
 *      Author: client
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>

class Socket {
public:
	Socket(int domain, int type, int protocol);
	explicit Socket(int fd) : fd(fd) {}
	inline Socket(Socket&& rhs) noexcept :fd(rhs.fd) { rhs.fd = -1; }

	void reuseAddr();
	void bind(const struct sockaddr *addr, socklen_t addrlen);
	void listen(int n);
	Socket accept() const;

	inline operator int() const { return fd; }
	virtual ~Socket();

	static Socket createListeningSocket(int port);
private:
	int fd;
};
#endif /* SOCKET_H_ */
