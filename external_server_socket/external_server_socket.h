#ifndef EXTERNAL_SERVER_SOCKET
#define EXTERNAL_SERVER_SOCKET

#include "external_server_storage.h"

struct external_server_socket {
public:
	external_server_socket(int socket_fd, const std::string &html_req, proxy_client_storage &parent, epoll &ep);
	external_server_socket(external_server_socket &&other);

	~external_server_socket();

	external_server_socket& operator=(external_server_socket &&);

private:
	static const size_t BUFFER_SIZE = 131072;

	std::unique_ptr<advanced_file_descriptor<external_server_storage>> ptr;
};

#include "external_server_socket.cpp"

#endif	