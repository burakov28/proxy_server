#include <bits/stdc++.h>

#include "../epoll/epoll_registrator.h"
#include "utils/utils.h"


using namespace std;

struct client_storage {
	client_storage(advanced_file_descriptor<client_storage> &fd, epoll &ep, string &req);


	static void on_in(advanced_file_descriptor<client_storage> &fd);
	static void on_out(advanced_file_descriptor<client_storage> &fd);
	static void on_time_expired(advanced_file_descriptor<client_storage> &fd);

	static const size_t READ_BUFFER = 1024;

	epoll_registrator reg;
	epoll &ep;
};

struct client {
	client(int fd, epoll &ep, string &req) : ptr(nullptr) {
		ptr = move(unique_ptr<advanced_file_descriptor<client_storage>>(
			new advanced_file_descriptor<client_storage>(
				fd, CLIENT_BUFFER, ep, req)));
	}

	static const size_t CLIENT_BUFFER = 131072;
	unique_ptr<advanced_file_descriptor<client_storage>> ptr;
};


client_storage::client_storage(advanced_file_descriptor<client_storage> &fd, epoll &ep, string &req) :
	reg(fd, ep, EPOLLIN | EPOLLOUT),
	ep(ep) {

	fd.write_to_buffer(req);
}

void client_storage::on_in(advanced_file_descriptor<client_storage> &fd) {
	static int a = 0;
	
	string resp = fd.read(READ_BUFFER);
	cout << endl << resp.size() << endl;
	cout << endl << a << endl;
	if (resp.size() == 0) {
		throw runtime_error("That's all");
	}

	cout << resp;
}

void client_storage::on_out(advanced_file_descriptor<client_storage> &fd) {
	fd.write();
	if (fd.buffer_empty()) {
		cout << endl << endl << endl << endl << "lol" << endl << endl << endl << endl;
		fd.get_storage().reg.modify(EPOLLIN);
	}	
}

void client_storage::on_time_expired(advanced_file_descriptor<client_storage> &fd) {
	throw runtime_error("Time expired!");	
}



int main() {
	string req = "GET http://stackoverflow.com HTTP/1.1\r\nHost: stackoverflow.com\r\nConnection: close\r\n\r\n";

	int fd = utils::get_socket_html_connection("stackoverflow.com");
	epoll ep;

	client cli(fd, ep, req);

	while (true) {
		ep.process();
	}
	return 0;
}