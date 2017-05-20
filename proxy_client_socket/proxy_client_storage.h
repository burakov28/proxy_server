#ifndef PROXY_CLIENT_STORAGE_H
#define PROXY_CLIENT_STORAGE_H

#include <iostream>
#include <memory>

#include "../external_server_socket/external_server_socket.h"
	
struct proxy_server_storage;

struct proxy_client_storage {
public:
	proxy_client_storage(advanced_file_descriptor<proxy_client_storage> &fd, epoll &ep, 
		proxy_server_storage &server, unsigned long long client_id);

	~proxy_client_storage();

	static void on_in(advanced_file_descriptor<proxy_client_storage> &fd);
	static void on_out(advanced_file_descriptor<proxy_client_storage> &fd);
	static void on_time_expired(advanced_file_descriptor<proxy_client_storage> &fd);

	void write_to_buffer_with_check(const std::string &message);
	void destroy(const std::string &message);
	void write_to_buffer(const std::string &message);
	void commit_request(std::string &html_req);
	void close_request(const std::string &message);
	void log(const std::string &message);
	unsigned long long get_id() const;
	void set_external_server(int socket, const std::string &html_req);

private:
	static const unsigned long long CLIENT_TIMEOUT = 100; //in seconds
	static const size_t MAX_BUFFER_SIZE = 131072;

	epoll_registrator reg;
	epoll &ep;

	proxy_server_storage &server;
	unsigned long long client_id;

	std::string request_buffer;

	std::unique_ptr<external_server_socket> external_server_ptr;
};

#include "../external_server_socket/external_server_storage.cpp"

#endif	