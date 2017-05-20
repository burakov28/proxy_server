#ifndef TASK_CPP
#define TASK_CPP 

task::task(request &&req, proxy_server_storage &server) :
	req(std::move(req)),
	server(server) {}



void task::commit_connection(const std::string &host_name) {
	

	server.receive_connection(socket_fd, std::move(req));
}

void task::operator()() {
	commit_connection(get_host(req.get_request()));
}

#endif