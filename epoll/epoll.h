#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <unordered_map>
#include <stdint.h>
#include <sys/signalfd.h>
#include <unordered_set>

#include "../timer_container.h"

struct signal_fd;

struct epoll {
public:
	epoll();

	~epoll();

	void process();

	void set_signal_handler(sigset_t *mask);

private:
	static const size_t EPOLL_SIZE = 1024;
	static const unsigned long long INF = (unsigned long long) 2 * 1e6;

	file_descriptor epollfd;
	::epoll_event evs[EPOLL_SIZE];
	timer_container<void*> timer;
	std::unordered_map<void*, timer_container<void*>::iterator> in_timer;
	signal_fd sig;
	std::unordered_set<void*> deleted_fds;

	void add(file_descriptor &fd, uint32_t events, unsigned long long timeout = INF);
	void mod(file_descriptor &fd, uint32_t new_events, unsigned long long new_timeout = INF);
	void del(file_descriptor &fd);

	friend struct epoll_registrator;
};

#endif