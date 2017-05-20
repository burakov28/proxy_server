#ifndef EXTERNAL_SERVER_STORAGE_H
#define EXTERNAL_SERVER_STORAGE_H 

struct proxy_client_storage;

struct external_server_storage {
public:
	external_server_storage(advanced_file_descriptor<external_server_storage> &fd, epoll &ep, 
		proxy_client_storage &parent, const std::string &html_req);

	~external_server_storage();

	static void on_in(advanced_file_descriptor<external_server_storage> &fd);
	static void on_out(advanced_file_descriptor<external_server_storage> &fd);
	static void on_time_expired(advanced_file_descriptor<external_server_storage> &fd);

private:
	static const unsigned long long CLIENT_TIMEOUT = 100; //in seconds

	epoll_registrator reg;
	epoll &ep;
	proxy_client_storage &parent;
};

#endif