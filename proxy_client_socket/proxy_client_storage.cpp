#ifndef PROXY_CLIENT_STORAGE_CPP
#define PROXY_CLIENT_STORAGE_CPP 
	
proxy_client_storage::proxy_client_storage(advanced_file_descriptor<proxy_client_storage> &fd, epoll &ep, 
	proxy_server_storage &server, unsigned long long client_id) :
		reg(fd, ep, EPOLLIN, CLIENT_TIMEOUT),
		ep(ep),
		server(server),
		client_id(client_id),
		request_buffer(""), 
		external_server_ptr(nullptr) {}

proxy_client_storage::~proxy_client_storage() {}

void proxy_client_storage::on_in(advanced_file_descriptor<proxy_client_storage> &fd) {	
	try {
		proxy_client_storage &storage = fd.get_storage();
		std::string message = fd.read();
		if (message.size() == 0) {
			storage.destroy("Successfully close");
			return;
		}
	
		storage.write_to_buffer_with_check(message);
	} catch (std::runtime_error e) {
		fd.get_storage().destroy(e.what());
	}
}

void proxy_client_storage::on_out(advanced_file_descriptor<proxy_client_storage> &fd) {
	try {
		proxy_client_storage &storage = fd.get_storage();
		
		fd.write();
		if (fd.buffer_empty()) {
			storage.reg.modify(EPOLLIN, CLIENT_TIMEOUT);
		}
	} catch(std::runtime_error e) {
		fd.get_storage().destroy(e.what());
	}	
}

void proxy_client_storage::on_time_expired(advanced_file_descriptor<proxy_client_storage> &fd) {
	fd.get_storage().destroy("Client's time expired");
}

void proxy_client_storage::write_to_buffer_with_check(const std::string &message) {
	size_t bg = request_buffer.size();
	bg -= std::min(bg, (size_t) 3);
	request_buffer += message;

	size_t pos = request_buffer.find("\r\n\r\n", bg);
	std::string html_req = "";

	while (pos != std::string::npos) {
		html_req = request_buffer.substr(0, pos + 4);
		request_buffer = request_buffer.substr(pos + 4, request_buffer.size() - pos - 4);
		pos = request_buffer.find("\r\n\r\n", 0);
	}

	if (request_buffer.size() > MAX_BUFFER_SIZE) {
		destroy("Request buffer is overflowed for client socket");
	}

	if (html_req.size() > 0) {
		commit_request(html_req);
	}
}

unsigned long long proxy_client_storage::get_id() const {
	return client_id;
}

void proxy_client_storage::set_external_server(int socket, const std::string &html_req) {
	if (socket == proxy_server_storage::BAD_SOCKET) {
		log("bad request: " + html_req);
		destroy("bad request");
		return;
	}

	external_server_ptr = std::move(std::unique_ptr<external_server_socket>(
		new external_server_socket(
			socket,
			html_req,
			*this,
			ep)));
}

void proxy_client_storage::destroy(const std::string &message) {
	log("was destroyed: " + message);
	server.del_client(client_id);
}

void proxy_client_storage::close_request(const std::string &message) {
	log("External server closed: " + message);
	external_server_ptr = std::move(std::unique_ptr<external_server_socket>(nullptr));
}

void proxy_client_storage::write_to_buffer(const std::string &message) {
	try {
		reg.get_fd().write_to_buffer(message);
		reg.modify(EPOLLIN | EPOLLOUT, CLIENT_TIMEOUT);
	} catch (std::runtime_error e) {
		destroy(e.what());
	}
}

void proxy_client_storage::commit_request(std::string &html_req) {
	try {
		server.create_external_server(html_req, *this);
	} catch (std::runtime_error e) {
		log("bad request: " + std::string(e.what()));
	}
}

void proxy_client_storage::log(const std::string &message) {
	std::cerr << "Client socket " + std::to_string(reg.get_fd().get_fd()) + " : " << message << " with address: " << (unsigned long long) ((void*) &(reg.get_fd())) << std::endl;
}

#endif