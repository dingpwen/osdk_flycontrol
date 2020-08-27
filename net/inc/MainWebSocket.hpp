/**
* MainWebSocket.hpp
*/
#ifndef MAIN_WEBSOCKET_HPP
#define MAIN_WEBSOCKET_HPP

#include <WebSocketBase.hpp>
#include <DjiBridge.hpp>

class MainWebSocket :public WebSocketBase {
private:
	DjiBridge* const bridge;

private:
	void reconnect();
	bool is_register_request(const std::string& message, std::string& url);

public:
	MainWebSocket(DjiBridge* const bridge, const std::string& url) :WebSocketBase(url), bridge(bridge){
	}

	~MainWebSocket() {
		terminate();
	}
	
	void start_run();
	virtual void process_message(std::string const& payload);
	virtual void after_close();
};
#endif