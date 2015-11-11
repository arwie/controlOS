#include "Socket.h"

#include <string>
#include <exception>
#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

using namespace std;


Socket::Socket(int domain, int type, int protocol)
{
	fd = socket(domain, type, protocol);
	if (fd == -1) {
		perror("socket");
		throw exception();
	}
}


void Socket::close() noexcept
{
	::close(fd);
	fd = undefined;
}


Socket::~Socket()
{
	if (fd != undefined)
		close();
}


void Socket::reuseAddr()
{
	int optval = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
		perror("setsockopt");
		throw exception();
	}
}


void Socket::bind(const struct sockaddr* addr, socklen_t addrlen)
{
	if (::bind(fd, addr, addrlen) == -1) {
		perror("bind");
		throw exception();
	}
}


void Socket::listen(int n)
{
	if (::listen(fd, n) == -1) {
		perror("listen");
		throw exception();
	}
}


Socket Socket::accept() const
{
	int acceptedFd = ::accept(fd, NULL, NULL);
	if (acceptedFd == -1) {
		perror("accept");
		throw exception();
	}
	return Socket(acceptedFd);
}


int Socket::receive(void* buffer, size_t length)
{
	for (;;) {
		auto ret = recv(fd, buffer, length, 0);
		if (ret == -1) {
			if (errno == EINTR)
				continue;

			perror("listen");
			throw exception();
		}
		if (ret == 0) {
			throw exception();
		}
		return ret;
	}
}




class AddrInfo {
public:
	AddrInfo(const char *node, const char *service, const struct addrinfo *hints) {
		if (getaddrinfo(node, service, hints, &result) != 0) {
			//logFatal("%s", gai_strerror(err));
			throw exception();
		}
	}
	~AddrInfo() {
		freeaddrinfo(result);
	}
	operator addrinfo*() { return result; }
private:
	struct addrinfo *result;
};


Socket Socket::createListeningSocket(int port)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_STREAM;	// TCP
	hints.ai_family = AF_UNSPEC;		// Allows IPv4 or IPv6
	hints.ai_flags = AI_PASSIVE;		// Use wildcard IP address

	AddrInfo addrInfo(NULL, to_string(port).c_str(), &hints);

	for (struct addrinfo *rp = addrInfo; rp != NULL; rp = rp->ai_next)
	{
		try {
			Socket socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			socket.reuseAddr();
			socket.bind(rp->ai_addr, rp->ai_addrlen);
			socket.listen(8);
			return move(socket);
		}
		catch (exception& e) {
			/* Walk through returned list until we find an address structure
			   that can be used to successfully create and bind a socket */
		}
	}

	throw exception();
}
