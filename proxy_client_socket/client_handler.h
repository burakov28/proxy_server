#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "proxy_client_socket.h"

#include <chrono>

struct proxy_server_storage;

struct client_handler {
public:
	client_handler(int client_fd, unsigned long long client_id, proxy_server_storage &server);

	~client_handler();


	void operator()();

private:
	int client_fd;
	unsigned long long client_id;
	proxy_server_storage &server;
};


#endif