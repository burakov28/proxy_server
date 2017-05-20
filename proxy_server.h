#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include "proxy_server_socket/proxy_server_socket.h"

struct proxy_server {
public:
	proxy_server(uint16_t port, uint32_t addr = INADDR_ANY) : port(port), addr(addr) {}

	void start() {
		epoll ep;

		sigset_t mask;
		::sigemptyset(&mask);
		::sigaddset(&mask, SIGTERM);
		::sigaddset(&mask, SIGINT);
		::sigprocmask(SIG_BLOCK, &mask, NULL);
		
		ep.set_signal_handler(&mask);
		proxy_server_socket server(ep, port, addr);

		try {
			while (true) {
				ep.process();
			}	
		} catch (std::runtime_error &re) {
			log(std::string("server stopped: ") + re.what());
		} catch (std::logic_error &le) {
			log(std::string("server stopped in cause of logic error: ") + le.what());
		} catch(...) {
			log("server has stopped, unknown error");
		}
		
	}

	void log(const std::string &str) const {
		std::cerr << "proxy_server: " << str << std::endl;
	}

private:
	uint16_t port;
	uint32_t addr;
};

#endif