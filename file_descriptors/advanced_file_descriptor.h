#ifndef ADVANCED_FILE_DESCRIPTOR_H
#define ADVANCED_FILE_DESCRIPTOR_H

#include "file_descriptor.h"

template <typename AdvancedStorage>
struct advanced_file_descriptor : file_descriptor {
public:
	typedef typename std::function<void(advanced_file_descriptor<AdvancedStorage>&)> AdvancedFunc;

	template <typename... Args>
	advanced_file_descriptor(int fd, 
						 		size_t size, 
						 		Args&... args) : 
									file_descriptor(fd, 
										func_wrapper(AdvancedStorage::on_in), 
										func_wrapper(AdvancedStorage::on_out), 
										func_wrapper(AdvancedStorage::on_time_expired), 
										size),
									storage(*this, args...) {

	}

	virtual ~advanced_file_descriptor() {}

	AdvancedStorage& get_storage() {
		return storage;
	}

private:
	AdvancedStorage storage;

	static file_descriptor::Func func_wrapper(AdvancedFunc func) {
		return [=](file_descriptor &arg)
			{ return func(static_cast<advanced_file_descriptor<AdvancedStorage> &>(arg)); };
	}
};

#endif