#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H

#include <iostream>
#include <functional>
#include <string>
#include <unistd.h>
#include <string.h>

struct file_descriptor {
public:
	typedef std::function<void(file_descriptor&)> Func;

	file_descriptor(int fd = -1, Func in_handler = empty_func, 
								 Func out_handler = empty_func, 
								 Func time_expired_handler = empty_func, 
						 			size_t size = 0) : 
										fd(fd), buffer_max_size(size), 
										in_handler(in_handler), 
										out_handler(out_handler), 
										time_expired_handler(time_expired_handler) {

	}

	virtual ~file_descriptor() {
		//std::cerr << "Socket closed: " + std::to_string(fd) << std::endl;	
		if (fd != -1) ::close(fd);
	}

	std::string read(size_t read_buffer_size = READ_BUFFER_SIZE) const {
		char read_buffer[read_buffer_size];
		memset(read_buffer, 0, sizeof(read_buffer));

		int was_read = ::read(fd, read_buffer, read_buffer_size);
		if (was_read == -1) {
			throw std::runtime_error("Read error. fd: " + std::to_string(fd));
		}
		std::string ret(read_buffer, was_read);
		return ret;
	}

	void write_to_buffer(const std::string &message) {
		//std::cerr << message << std::endl;
		if (rest_of_buffer() < message.size()) {
			throw std::runtime_error("Output buffer overflowed " + std::to_string(fd));
		}
		buffer += message;
	}

	void write() {
		if (buffer_empty()) {
			throw std::logic_error("Nothing to write. Output buffer is empty. fd: " + std::to_string(fd));
		}

		int was_written = ::write(fd, buffer.c_str(), buffer.size());

		if (was_written > 0) {
			shift_buffer_left(was_written);
		}
		if (was_written < 0) {
			throw std::runtime_error("Write to buffer error. fd: " + std::to_string(fd));
		}	
	}

	void on_in() {
		in_handler(*this);
	}

	void on_out() {
		out_handler(*this);
	}

	void on_time_expired() {
		time_expired_handler(*this);
	}



	int get_fd() const {
		return fd;
	}

	void set_fd(int new_fd) {
		if (fd != -1) ::close(fd);
		fd = new_fd;
	}

	void shift_buffer_left(int shift) {
		buffer = buffer.substr(shift, buffer.size() - shift);
	}

	size_t rest_of_buffer() const {
		return buffer_max_size - buffer.size();
	}

	void resize_buffer(size_t new_size) {
		if (new_size < buffer.size()) {
			buffer = buffer.substr(0, new_size);
		}
		else {
			std::string tmp(new_size, '\0');
			for (size_t i = 0; i < buffer.size(); ++i) {
				tmp[i] = buffer[i];
			}
			std::swap(buffer, tmp);
		}
		buffer_max_size = new_size;
	}

	bool buffer_empty() const {
		return buffer.empty();
	}

	void clear() {
		buffer.clear();
	}

	static void empty_func(file_descriptor &fd) {
		throw std::logic_error("Unsupported operation" + std::to_string(fd.fd));
	}

private:
	static const size_t READ_BUFFER_SIZE = 1024;

	int fd;
	std::string buffer;
	size_t buffer_max_size;

	Func in_handler, out_handler, time_expired_handler;
};

#endif