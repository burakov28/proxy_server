#ifndef PROXY_SERVER_SOCKET_CPP
#define PROXY_SERVER_SOCKET_CPP


proxy_server_socket::proxy_server_socket(epoll &ep, uint16_t port, uint32_t s_addr) : ptr(nullptr) {

	int socket_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (socket_fd == -1) throw std::runtime_error("Error of taking server's socket");

	::sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = s_addr;


	::bind(socket_fd, (::sockaddr *) &addr, sizeof(addr));
	/*
	if ( == -1) {
		::close(socket_fd);
		throw std::runtime_error("Error socket's binding");
	}*/
	if (::listen(socket_fd, 128) == -1) {
		::close(socket_fd);
		throw std::runtime_error("Error of starting for listening");
	}

	ptr = std::move(std::unique_ptr<advanced_file_descriptor<proxy_server_storage>>(new advanced_file_descriptor<proxy_server_storage>(
		socket_fd,
		0,
		ep
	)));
	//std::cout << "lol2" << std::endl;
}

proxy_server_socket::proxy_server_socket(proxy_server_socket &&other) : ptr(std::move(other.ptr)) {}

proxy_server_socket::~proxy_server_socket() {}

proxy_server_socket& proxy_server_socket::operator=(proxy_server_socket &&other) {
	ptr = std::move(other.ptr);
	return *this;
}


#endif