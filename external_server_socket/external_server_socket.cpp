#ifndef EXTERNAL_SERVER_SOCKET_CPP
#define EXTERNAL_SERVER_SOCKET_CPP

external_server_socket::external_server_socket(int socket_fd, const std::string &html_req, 
	proxy_client_storage &parent, epoll &ep) : 
		ptr(nullptr) {

	std::cerr << "html_req: " << html_req << std::endl;
	ptr = std::move(std::unique_ptr<advanced_file_descriptor<external_server_storage>>(
		new advanced_file_descriptor<external_server_storage>(
			socket_fd,
			BUFFER_SIZE,
			ep,
			parent,
			html_req)));
}

external_server_socket::external_server_socket(external_server_socket &&other) : ptr(std::move(other.ptr)) {}

external_server_socket::~external_server_socket() {}

external_server_socket& external_server_socket::operator=(external_server_socket &&other) {
	ptr = std::move(other.ptr);
	return *this;
}

#endif