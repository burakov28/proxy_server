#ifndef EPOLL_REGISTRATOR_H
#define EPOLL_REGISTRATOR_H

#include "../file_descriptors/advanced_file_descriptor.h"

#include "signal_fd.h"
#include "epoll.h"

struct epoll_registrator {
	template <typename FileDescriptor>
	epoll_registrator(FileDescriptor &fd, epoll &ep, uint32_t events, unsigned long long timeout = epoll::INF);
	
	~epoll_registrator();

	void modify(uint32_t events, unsigned long long new_timeout = -1);

	file_descriptor& get_fd();

private:
	file_descriptor &fd;
	epoll &ep;
	unsigned long long timeout;

	friend struct epoll;
};

#include "signal_storage.h"
#include "signal_fd.cpp"

#include "epoll.cpp"
#include "epoll_registrator.cpp"

#endif