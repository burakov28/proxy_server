#ifndef TIMER_STORAGE_H
#define TIMER_STORAGE_H 


struct timer_storage {
public:
	timer_storage(advanced_file_descriptor<timer_storage> &fd, itimerspec &it, 
		std::function<void()> &func, epoll &ep) : 
			timerfd(fd.get_fd()), 
			reg(fd, ep, EPOLLIN),
			initial_spec(it),
			func(func) {}

	static void on_in(advanced_file_descriptor<timer_storage> &fd) {
		fd.get_storage().log("triggered");
		fd.get_storage().func();
	}

	static void on_out(advanced_file_descriptor<timer_storage> &fd) {
		throw std::logic_error("Timer descriptor was added on out");
	}

	static void on_time_expired(advanced_file_descriptor<timer_storage> &fd) {
		throw std::logic_error("Timer descriptor's time expired");
	}

	void stop() {
		log("stopped");
		timespec zero_time = {0, 0};
		itimerspec z = {zero_time, zero_time};
		::timerfd_settime(timerfd, 0, &z, NULL);
	}

	void reset(itimerspec &new_spec) {
		log("reset");
		initial_spec = new_spec;
		wakeup();
	}

	void wakeup() {
		log("wake up");
		::timerfd_settime(timerfd, 0, &initial_spec, NULL);
	}

private:
	int timerfd;
	epoll_registrator reg;
	itimerspec initial_spec;
	std::function<void()> func;

	void log(const std::string &message) {
		std::cerr << "Timer " << timerfd << ": " << message << std::endl;
	}

};

#endif