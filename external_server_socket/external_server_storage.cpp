#ifndef EXTERNAL_SERVER_STORAGE_CPP
#define EXTERNAL_SERVER_STORAGE_CPP 

#include <cassert>

external_server_storage::external_server_storage(advanced_file_descriptor<external_server_storage> &fd, epoll &ep, 
		proxy_client_storage &parent, const std::string &html_req) :
			reg(fd, ep, 0, CLIENT_TIMEOUT),
			ep(ep),
			parent(parent) {

	//std::cerr << "External server socket: " << std::to_string(fd.get_fd()) << " was created with address: " << (unsigned long long) (&fd) << std::endl;
	fd.write_to_buffer(html_req);
	reg.modify(EPOLLIN | EPOLLOUT, CLIENT_TIMEOUT);
}

external_server_storage::~external_server_storage() {
	//std::cerr << "External server socket: " << std::to_string(reg.get_fd().get_fd()) << " was destroyed" << std::endl;	
}

void external_server_storage::on_in(advanced_file_descriptor<external_server_storage> &fd) {
	try {
		std::string response = fd.read();
		//std::cout << "I'm read: \"" << response << "\"" << std::endl;

		if (response.size() != 0) fd.get_storage().parent.write_to_buffer(response);
		else fd.get_storage().parent.close_request("Request complete");
	} catch (std::runtime_error e) {
		fd.get_storage().parent.close_request(e.what());
	}
}

void external_server_storage::on_out(advanced_file_descriptor<external_server_storage> &fd) {
	try {
		fd.write();
		if (fd.buffer_empty()) {
			fd.get_storage().reg.modify(EPOLLIN, CLIENT_TIMEOUT);
		}	
	} catch (std::runtime_error e) {
		fd.get_storage().parent.close_request(e.what());
	}
} 

void external_server_storage::on_time_expired(advanced_file_descriptor<external_server_storage> &fd) {
	fd.get_storage().parent.close_request("Server do not response");	
}  

#endif