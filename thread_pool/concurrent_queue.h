#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <mutex>
#include <queue>

template <typename T>
struct concurrent_queue {
public:
	concurrent_queue() : interrupt(false) {
		global_locker.lock();
	}

	void block() const {
		global_locker.lock();
		locker.lock();
		global_locker.unlock();
	}

	void unblock() const {
		locker.unlock();
	}

	size_t size_non_block() const {
		return q.size();
	}

	void push_non_block(const T &element) {
		q.push(element);
		if (q.size() == 1) {
			global_locker.unlock();
		}
	}

	void pop() {
		block();
		if (interrupt) throw std::runtime_error("Thread interrupted");
		q.pop();
		if (q.size() == 0) {
			global_locker.lock();
		}
		unblock();
	}


	T& front() {
		block();
		if (interrupt) throw std::runtime_error("Thread interrupted");
		T& ans = q.front();
		unblock();
		return ans;
	}

private:
	mutable bool interrupt;
	mutable std::mutex locker;
	mutable std::mutex global_locker;
	std::queue<T> q;

	friend struct thread_pool;
};

#endif