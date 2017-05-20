#ifndef EXTERNAL_SERVER_CREATOR_H
#define EXTERNAL_SERVER_CREATOR_H
	
struct external_server_creator {
public:
	external_server_creator(std::string html_req, unsigned long long parent_id, proxy_server_storage &server);

	~external_server_creator();

	void operator()();
	
private:
	std::string html_req;
	unsigned long long parent_id;
	proxy_server_storage &server;
};

#endif