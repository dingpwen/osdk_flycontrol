/**
* WorkerWebSocket.hpp
*/
#ifndef WORKER_WEBSOCKET_HPP
#define WORKER_WEBSOCKET_HPP

#include <WebSocketBase.hpp>
#include <DjiBridge.hpp>

class WorkerWebSocket : public WebSocketBase {
private:
	DjiBridge * const bridge;

public:
	WorkerWebSocket(DjiBridge* const bridge, const std::string& url) :WebSocketBase(url), bridge(bridge) {

	}

	~WorkerWebSocket() {}

	void start_connect();

	virtual void process_message(std::string const& payload);
	virtual void after_close();
};
#endif