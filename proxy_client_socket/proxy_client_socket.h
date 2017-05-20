#ifndef PROXY_CLIENT_SOCKET_H
#define PROXY_CLIENT_SOCKET_H

#include "../utils/utils.h"
#include "../epoll/epoll_registrator.h"

#include "proxy_client_storage.h"

struct proxy_client_socket {
	proxy_client_socket(int fd, epoll &ep, proxy_server_storage &server, unsigned long long client_id);
	proxy_client_socket(proxy_client_socket &&other);

	~proxy_client_socket();

	proxy_client_socket& operator=(proxy_client_socket &&other);

	proxy_client_storage& get_storage();

private:
	static const size_t CLIENT_BUFFER_SIZE = 131072; //for answer from server

	std::unique_ptr<advanced_file_descriptor<proxy_client_storage>> ptr;
};

#include "proxy_client_socket.cpp"

#endif