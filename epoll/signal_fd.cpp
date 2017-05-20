#ifndef SIGNAL_FD_CPP
#define SIGNAL_FD_CPP

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

signal_fd::signal_fd() : ptr(nullptr) {}

signal_fd::signal_fd(sigset_t *mask, epoll &ep) : ptr(nullptr) {
	int fd = ::signalfd(-1, mask, O_NONBLOCK);
	if (fd < 0) throw std::runtime_error("Error signalfd creating");

	ptr = std::move(std::unique_ptr<advanced_file_descriptor<signal_storage>>(
		new advanced_file_descriptor<signal_storage>(
			fd, 0, ep)));
}

signal_fd::signal_fd(signal_fd &&other) : ptr(std::move(other.ptr)) {}

signal_fd::~signal_fd() {}

signal_fd& signal_fd::operator=(signal_fd &&other) {
	ptr = std::move(other.ptr);
	return *this;
}



#endif