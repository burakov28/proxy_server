#ifndef CLIENT_HANDLER_CPP
#define CLIENT_HANDLER_CPP 

client_handler::client_handler(int client_fd, unsigned long long client_id, proxy_server_storage &server) :
	client_fd(client_fd),
	client_id(client_id),
	server(server) {}

client_handler::~client_handler() {}

void client_handler::operator()() {
	epoll ep;
	proxy_client_socket client(client_fd, ep);

	try {
		while (true) {
			ep.process();
			//std::cout << "lol" << std::endl;
		}
	} catch(...) {
		//std::cout << "Hello" << std::endl;
		//server.add_dead_thread(client_id);
	}
	//server.del_client(client_id);
}

#endif