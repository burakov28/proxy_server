#ifndef PROXY_CLIENT_SOCKET_CPP
#define PROXY_CLIENT_SOCKET_CPP


proxy_client_socket::proxy_client_socket(int fd, epoll &ep, proxy_server_storage &server, 
			unsigned long long client_id) : ptr(nullptr) {

	ptr = std::move(std::unique_ptr<advanced_file_descriptor<proxy_client_storage>>(
		new advanced_file_descriptor<proxy_client_storage>(
			fd,
			CLIENT_BUFFER_SIZE,
			ep,
			server,
			client_id)));
}

proxy_client_socket::proxy_client_socket(proxy_client_socket &&other) : ptr(std::move(other.ptr)) {}

proxy_client_socket::~proxy_client_socket() {}

proxy_client_storage& proxy_client_socket::get_storage() {
	return ptr->get_storage();
}

proxy_client_socket& proxy_client_socket::operator=(proxy_client_socket &&other) {
	ptr = std::move(other.ptr);
	return *this;
}

#endif