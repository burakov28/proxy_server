#ifndef TIMER_H
#define TIMER_H

#include <sys/timerfd.h>
#include <functional>
#include <memory>


#include "../file_descriptors/advanced_file_descriptor.h"
#include "../epoll/epoll_registrator.h"
#include "timer_storage.h"

struct timer {	
	timer(unsigned long long first_len, unsigned long long period, std::function<void()> func, epoll &ep) :
			ptr(nullptr) {

		int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
		itimerspec it;
		it.it_interval = convert_time(period);
		it.it_value = convert_time(first_len);

		::timerfd_settime(timerfd, 0, &it, NULL);

		ptr = std::move(std::unique_ptr<advanced_file_descriptor<timer_storage>>(
			new advanced_file_descriptor<timer_storage>(
				timerfd,
				0,
				it,
				func,
				ep)));
	}

	void stop() {
		ptr->get_storage().stop();

	}

	void reset(unsigned long long first_len, unsigned long long period) {
		itimerspec it;
		it.it_interval = convert_time(period);
		it.it_value = convert_time(first_len);

		ptr->get_storage().reset(it);
	}

	void wakeup() {
		ptr->get_storage().wakeup();
	} 

private:
	static const unsigned long long NANOSEC_IN_SEC = (unsigned long long) 1e9;

	static timespec convert_time(unsigned long long nanosec_duration) {
		timespec ret;
		ret.tv_sec = nanosec_duration / NANOSEC_IN_SEC;
		ret.tv_nsec = nanosec_duration % NANOSEC_IN_SEC;
		return ret;
	}

	std::unique_ptr<advanced_file_descriptor<timer_storage>> ptr;
};

#endif