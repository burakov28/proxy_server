#ifndef SIGNAL_STORAGE_H
#define SIGNAL_STORAGE_H

struct signal_storage {
	signal_storage(advanced_file_descriptor<signal_storage> &fd, epoll &ep) : reg(fd, ep, EPOLLIN | EPOLLET) {}

	~signal_storage() {}
	
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	static void on_in(advanced_file_descriptor<signal_storage> &fd) {
		throw std::runtime_error("Termination");
	}

	static void on_out(advanced_file_descriptor<signal_storage> &fd) {
		throw std::logic_error("Signal file descriptor was added for write");
	}

	static void on_time_expired(advanced_file_descriptor<signal_storage> &fd) {
		throw std::logic_error("Signal file descriptor's time was expired");
	}

private:
	epoll_registrator reg;
};

#endif