#ifndef PROXY_SERVER_STORAGE_H
#define PROXY_SERVER_STORAGE_H 

#include <mutex>
#include <unordered_map>
#include <vector>
#include <tuple>

#include "../utils/ids_generator.h"
#include "../thread_pool/thread_pool.h"
#include "../proxy_client_socket/proxy_client_socket.h"
#include "external_server_creator.h"
#include "../timer/timer.h"

struct proxy_server_storage {
public:
	static const int BAD_SOCKET = -1;

	proxy_server_storage(advanced_file_descriptor<proxy_server_storage> &fd, epoll &ep);
	
	proxy_server_storage(const proxy_server_storage &other) = delete;
	proxy_server_storage(proxy_server_storage &other) = delete;

	~proxy_server_storage();

	proxy_server_storage& operator=(const proxy_server_storage &other) = delete;
	proxy_server_storage& operator=(proxy_server_storage &other) = delete;

	void add_client(int client_fd);
	void del_client(unsigned long long id);
	void del_client_nonblock(unsigned long long id);
	void create_external_server(std::string &html_req, proxy_client_storage &parent);
	bool has(unsigned long long id);
	void add_socket_to_queue(unsigned long long id, int socket, const std::string &html_req);
	void connect_all_queued_sockets();

	static void on_in(advanced_file_descriptor<proxy_server_storage> &fd);
	static void on_out(advanced_file_descriptor<proxy_server_storage> &fd);
	static void on_time_expired(advanced_file_descriptor<proxy_server_storage> &fd);

private:
	static const size_t THREAD_POOL_SIZE = 10;
	static const unsigned long long PERIOD = 10 * 1000; //nanoseconds

	thread_pool pool;
	epoll_registrator reg;
	epoll &ep;	

	std::unordered_map<unsigned long long, proxy_client_socket> clients;
	ids_generator id_gen;
	
	timer alarm;
	std::unordered_map <unsigned long long, std::pair<int, std::string>> queued_sockets;
	std::mutex server_locker;

	unsigned long long get_next_id();
};

#include "../proxy_client_socket/proxy_client_storage.cpp"
#include "proxy_server_storage.cpp"
#include "external_server_creator.cpp"


#endif