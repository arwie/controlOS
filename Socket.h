#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>

class Socket {
public:
	explicit Socket(int fd) :fd(fd) {}
	Socket(int domain, int type, int protocol);

	Socket(Socket&& rhs) noexcept :fd(rhs.fd) { rhs.fd = undefined; }

	void close() noexcept;
	~Socket();

	void reuseAddr();
	void bind(const struct sockaddr *addr, socklen_t addrlen);
	void listen(int n);
	Socket accept() const;

	operator int() const { return fd; }

	static Socket createListeningSocket(int port);

private:
	int fd;
	enum {undefined = -1};
};
#endif /* SOCKET_H_ */
