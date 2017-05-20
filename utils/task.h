#ifndef TASK_H
#define TASK_H 

#include <iostream>
#include <netdb.h>

struct task {
public:
	task(request &&req, proxy_server_storage &server) {}

	static std::string get_host(const std::string &html_req);
	void commit_connection(const std::string &host);

	void operator()();

private:
	request req;
	proxy_server_storage &server;
};

#endif