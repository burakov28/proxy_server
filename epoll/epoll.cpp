#ifndef EPOLL_CPP
#define EPOLL_CPP

epoll::epoll() {
	int fd = ::epoll_create1(0);
	if (fd < 0) {
		throw std::runtime_error("Error on epoll's creating");
	}

	epollfd.set_fd(fd);
}

epoll::~epoll() {}

void epoll::process() {
	unsigned long long ctime;
	while (!timer.empty() && (ctime = time(NULL)) >= timer.get_next_expiration()) {
		file_descriptor &fd = *(static_cast<file_descriptor*>(timer.get_next().get_element()));
		fd.on_time_expired();
	}

	long long timeout = -1;
	if (!timer.empty()) timeout = (timer.get_next_expiration() - time(NULL)) * 1000; //in milliseconds

	int ready = ::epoll_wait(epollfd.get_fd(), evs, EPOLL_SIZE, timeout);
	deleted_fds.clear();

	//std::cerr << "End of waiting" << std::endl;
	for (int i = 0; i < ready; ++i) {

		::epoll_event &ev = evs[i];
		void *ptr = ev.data.ptr;
		
		if (deleted_fds.find(ptr) != deleted_fds.end()) {
			continue;
		}

		/*if (in_timer.find(ptr) == in_timer.end()) {
			std::cerr << "Epoll. Wrong modification by address: " << (unsigned long long) ptr << std::endl;
		}*/
		//std::cerr << "epoll. modified address: " << (unsigned long long) ptr << std::endl;
		in_timer[ptr] = timer.modify(in_timer[ptr]);

		file_descriptor &fd = *(static_cast<file_descriptor*>(ptr));
		if (ev.events & EPOLLIN) {
			fd.on_in();
			continue;
		}
		if (ev.events & EPOLLOUT) fd.on_out();	
	}
}

void epoll::set_signal_handler(sigset_t *mask) {
	sig = std::move(signal_fd(mask, *this));
}

void epoll::add(file_descriptor &fd, uint32_t events, unsigned long long timeout) {
	void *ptr = static_cast<void*>(&fd);

	::epoll_event ev;
	ev.events = events;
	ev.data.ptr = ptr;

	int resp = ::epoll_ctl(epollfd.get_fd(), EPOLL_CTL_ADD, fd.get_fd(), &ev);
	if (resp == -1) {
		throw std::runtime_error("Error of adding to epoll. Epoll's fd: " + std::to_string(epollfd.get_fd()));
	}

	in_timer[ptr] = timer.add(ptr, timeout);
}

void epoll::mod(file_descriptor &fd, uint32_t new_events, unsigned long long new_timeout) {
	void *ptr = static_cast<void*>(&fd);

	::epoll_event ev;
	ev.events = new_events;
	ev.data.ptr = ptr;

	int resp = ::epoll_ctl(epollfd.get_fd(), EPOLL_CTL_MOD, fd.get_fd(), &ev);
	if (resp == -1) {
		throw std::runtime_error("Error of epoll's modification. Epoll's fd: " + std::to_string(epollfd.get_fd()));
	}
}

void epoll::del(file_descriptor &fd) {
	void *ptr = static_cast<void*>(&fd);

	deleted_fds.insert(ptr);

	timer.remove(in_timer[ptr]);
	in_timer.erase(ptr);
	::epoll_ctl(epollfd.get_fd(), EPOLL_CTL_DEL, fd.get_fd(), NULL);
}

#endif