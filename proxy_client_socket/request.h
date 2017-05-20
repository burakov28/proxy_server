#ifndef REQUEST_H
#define REQUEST_H

struct request {
public:
	request(unsigned long long client_id, unsigned long long request_id, std::string &&req) : 
		client_id(client_id),
		request_id(request_id),
		req(std::move(req)), 
		ans("") {}

	request(request &&other) :
		client_id(other.client_id), request_id(other.request_id),
		req(std::move(other.req)), ans(std::move(other.ans)) {}

	request& operator=(request &&other) {
		client_id = other.client_id; request_id = other.request_id;
		req = std::move(other.req); ans = std::move(other.ans);
	}

	void append_answer(const std::string message) {
		req += message;
	}

	std::string& get_answer() {
		return ans;
	}

	std::string& get_request() {
		return req;
	}

	unsigned long long get_req_id() const {
		return request_id;
	}

	unsigned long long get_cli_id() const {
		return client_id;
	}

private:
	unsigned long long client_id, request_id;
	std::string req, ans;
}

#endif