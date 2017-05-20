#ifndef THREAD_POOL_H
#define THREAD_POOL_H 

#include <thread>
#include <mutex>

#include "concurrent_queue.h"
#include "executor.h"

struct thread_pool {
public:
	thread_pool(size_t threads) :
			threads(threads),
			ths(new std::thread[threads]),
			queues(new concurrent_queue<std::function<void(void)>>[threads]) {

		if (threads == 0) throw std::logic_error("Create empty thread pool");

		for (size_t i = 0; i < threads; ++i) {
			ths[i] = std::thread(executor<std::function<void(void)>>(queues[i]));
		}
	}

	~thread_pool() {
		for (size_t i = 0; i < threads; ++i) {
			queues[i].locker.lock();
			queues[i].interrupt = true;
			if (queues[i].size_non_block() == 0) {
				queues[i].global_locker.unlock();
			}

			queues[i].locker.unlock();

			ths[i].join();
		}

		std::cout << "Stopped!!!" << std::endl;

		delete[] queues;
		delete[] ths;
	}

	void commit(std::function<void(void)> task) {	
		size_t min_size = queues[0].size_non_block();
		concurrent_queue<std::function<void(void)>> *q = &queues[0];

		for (size_t i = 0; i < threads; ++i) {
			if (min_size < queues[i].size_non_block()) {
				min_size = queues[i].size_non_block();
				q = &queues[i];
			}
		}

		q->locker.lock();
		q->push_non_block(task);
		if (q->size_non_block() == 1) {
			q->global_locker.unlock();
		}

		q->locker.unlock();
	}

private:
	size_t threads;
	std::thread *ths;
	concurrent_queue<std::function<void(void)>> *queues;
};

#endif