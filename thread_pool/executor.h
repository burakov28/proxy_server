#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "concurrent_queue.h"
	
template <typename T>
struct executor {
public:
	executor(concurrent_queue<T> &q) : q(q) {}

	void operator()() {
		try {
			while(true) {
				T elem = q.front();
				q.pop();
				elem();
			}	
		} catch(std::runtime_error &re) {
			std::cerr << "executor stopped: " << re.what() << std::endl;
		}
	}
private:
	concurrent_queue<T> &q;
};

#endif