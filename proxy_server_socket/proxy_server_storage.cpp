#ifndef PROXY_SERVER_STORAGE_CPP
#define PROXY_SERVER_STORAGE_CPP 

proxy_server_storage::proxy_server_storage(advanced_file_descriptor<proxy_server_storage> &fd, epoll &ep) : 
		pool(THREAD_POOL_SIZE),
		reg(fd, ep, EPOLLIN), 
		ep(ep),
		alarm(PERIOD, PERIOD, [this]() -> void {
				proxy_server_storage &server = *this;
				server.connect_all_queued_sockets();		
			}, 
			ep) {
			alarm.stop();
		}

proxy_server_storage::~proxy_server_storage() {}

void proxy_server_storage::add_client(int client_fd) {
	unsigned long long client_id = id_gen.next();
	clients.emplace(client_id, std::move(proxy_client_socket(client_fd, ep, *this, client_id)));
}

void proxy_server_storage::del_client(unsigned long long id) {
	clients.erase(id);
}

void proxy_server_storage::connect_all_queued_sockets() {
	server_locker.lock();

	//std::cerr << "SIZE SIZE SIZE" << queued_sockets.size() << std::endl;

	for (auto sc : queued_sockets) {
		unsigned long long id = sc.first;
		int socket = sc.second.first;
		std::string &html_req = sc.second.second;

		auto it = clients.find(id);
		if (it != clients.end()) {
			(it->second).get_storage().set_external_server(socket, html_req);
		}
	}

	queued_sockets.clear();

	alarm.stop();

	server_locker.unlock();
}

void proxy_server_storage::add_socket_to_queue(unsigned long long id, int socket, const std::string &html_req) {
	server_locker.lock();

	size_t sz = queued_sockets.size();

	queued_sockets[id] = {socket, html_req};

	if (sz == 0) alarm.wakeup();

	server_locker.unlock();
}

void proxy_server_storage::create_external_server(std::string &html_req, proxy_client_storage &parent) {
	pool.commit(external_server_creator(html_req, parent.get_id(), *this));
}

void proxy_server_storage::on_in(advanced_file_descriptor<proxy_server_storage> &fd) {
	::sockaddr_in addr;
	::socklen_t addr_len;

	int client_fd = ::accept4(fd.get_fd(), (::sockaddr *) &addr, &addr_len, SOCK_NONBLOCK);
	fd.get_storage().add_client(client_fd);
}

/*bool proxy_server_storage::has(unsigned long long id) {
	server_locker.lock();

	bool ans = (clients.find(id) != clients.end());

	server_locker.unlock();

	return ans;
}*/

#pragma GCC diagnostic ignored "-Wunused-parameter"
void proxy_server_storage::on_out(advanced_file_descriptor<proxy_server_storage> &fd) {
	throw std::logic_error("Unsupported operation: server socket was added on write");
}

void proxy_server_storage::on_time_expired(advanced_file_descriptor<proxy_server_storage> &fd) {
	throw std::logic_error("Unsupported operation: server socket's time was expired");
}

#endif