#ifndef SIGNAL_FD_H
#define SIGNAL_FD_H

#include <sys/signalfd.h>

struct signal_storage;
struct epoll;

struct signal_fd {
	signal_fd();
	signal_fd(sigset_t *mask, epoll &ep);
	signal_fd(signal_fd &&other);

	signal_fd& operator=(signal_fd &&other);

	~signal_fd();

private:
	std::unique_ptr<advanced_file_descriptor<signal_storage>> ptr;
};

#endif