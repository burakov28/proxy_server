#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

namespace utils {

	int string_to_int(const std::string &str) {
		int ans = 0;
		size_t bg = 0;
		if (str.size() == 0) {
			return -1;
		}

		if (str[0] == '-') {
			++bg;
		}

		for (; bg < str.size(); ++bg) {
			if (!(('0' <= str[bg]) && (str[bg] <= '9'))) return -1;
			ans *= 10;
			ans += str[bg] - '0';
		}


		if (bg == 1) ans *= -1;
		return ans;
	}

	void log(const std::string &message) {
		std::cerr << "utils.h: " << message << std::endl;
	}

	std::pair<std::string, int> get_host(const std::string &html_req) {
		std::string templ = "\r\nHost: ";
		size_t pos = html_req.find(templ, 0);

		if (pos == std::string::npos) {
			throw std::runtime_error("Request does not contain OPTION \"Host\"");
		}

		pos += templ.size();

		size_t end = html_req.find("\r\n", pos);
		if (end == std::string::npos) {
			throw std::runtime_error("Wrong format of request");
		}

		
		std::string ret = html_req.substr(pos, end - pos);

		int port = -1;
		size_t port_pos = ret.find(":", 0);
		if (port_pos != std::string::npos) {
			port = string_to_int(ret.substr(port_pos + 1, ret.size() - port_pos - 1));
			ret = ret.substr(0, port_pos);
		}

		log("Host: " + ret);
		log("Port: " + std::to_string(port));
		return {ret, port};
	}

	int get_socket_html_connection(const std::string &host_name, int port) {		
		hostent *host = ::gethostbyname(host_name.c_str());

		
		if (host == NULL) {
			throw std::runtime_error("Error getting host by name: \"" + host_name + "\"");
		}


		::sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		if (port == -1) port = 80;
		addr.sin_port = htons(port);
	
		inet_pton(AF_INET, ::inet_ntoa(*((in_addr *) host->h_addr_list[0])), &addr.sin_addr);

		std::string ad = ::inet_ntoa(addr.sin_addr);
		log("Address : " + ad);

		int socket_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
		if (socket_fd == -1) {
			throw std::runtime_error("Error open socket (utils.h)");
		}
		
		while (::connect(socket_fd, (sockaddr *) &addr, sizeof(addr)) < 0) {
			/*int err = errno;
			if (err != EINPROGRESS) {
				log("connection error: " + std::to_string(err));
					throw std::runtime_error("error establishing connection (utils.h)");
			}*/
			//log("connection error: " + std::to_string(err));
			//throw std::runtime_error("error establishing connection (utils.h)");

		}

		/*int flags = ::fcntl(socket_fd, F_GETFL, 0);
		if (flags < 0) {
			int err = errno;
			log("connection error1: " + std::to_string(err));
			throw std::runtime_error("error making socket nonblocking");
		}

		flags |= O_NONBLOCK;
		if (fcntl(socket_fd, F_SETFL, flags) != 0) {
			int err = errno;
			log("connection error2: " + std::to_string(err));
			throw std::runtime_error("error making socket nonblocking");
		}*/

		return socket_fd;
	}


}

#endif