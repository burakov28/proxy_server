#ifndef EPOLL_REGISTRATOR_CPP
#define EPOLL_REGISTRATOR_CPP

template <typename FileDescriptor>
epoll_registrator::epoll_registrator(FileDescriptor &_fd, epoll &ep, uint32_t events, unsigned long long timeout) :
	fd(static_cast<file_descriptor&>(_fd)), ep(ep), timeout(timeout) {

	//std::cerr << "Insert to epoll. fd: " << fd.get_fd() << " address: " << (unsigned long long) ((void *) &fd) << std::endl;
	ep.add(fd, events, timeout);
}

epoll_registrator::~epoll_registrator() {
	//std::cerr << "Delete from epoll. fd: " << fd.get_fd() << " address: " << (unsigned long long) ((void *) &fd) << std::endl;
	ep.del(fd);
} 

void epoll_registrator::modify(uint32_t new_events, unsigned long long new_timeout) {
	if (new_timeout != (unsigned long long) -1) {
		timeout = new_timeout;
	} 

	ep.mod(fd, new_events, timeout);
}

file_descriptor& epoll_registrator::get_fd() {
	return fd;
}

#endif