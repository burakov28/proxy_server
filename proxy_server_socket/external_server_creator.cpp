#ifndef EXTERNAL_SERVER_CREATOR_CPP
#define EXTERNAL_SERVER_CREATOR_CPP 

external_server_creator::external_server_creator(std::string html_req, 
	unsigned long long parent_id, proxy_server_storage &server) :
		html_req(std::move(html_req)),
		parent_id(parent_id),
		server(server) {}

external_server_creator::~external_server_creator() {}

void external_server_creator::operator()() {
	try {
		std::pair<std::string, int> host = utils::get_host(html_req);

		int socket = utils::get_socket_html_connection(host.first, host.second);

		server.add_socket_to_queue(parent_id, socket, html_req);
	} catch (std::runtime_error &e) {
		server.add_socket_to_queue(parent_id, proxy_server_storage::BAD_SOCKET, e.what());
	}
}

#endif