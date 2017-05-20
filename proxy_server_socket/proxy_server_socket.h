#ifndef PROXY_SERVER_SOCKET_H
#define PROXY_SERVER_SOCKET_H

#include "proxy_server_storage.h"

struct proxy_server_socket {
public:
	proxy_server_socket(epoll &ep, uint16_t port, uint32_t s_addr = INADDR_ANY);
	proxy_server_socket(proxy_server_socket &&other);

	~proxy_server_socket();

	proxy_server_socket& operator=(proxy_server_socket &&other);

private:
	std::unique_ptr<advanced_file_descriptor<proxy_server_storage>> ptr;
};

#include "proxy_server_socket.cpp"

#endif