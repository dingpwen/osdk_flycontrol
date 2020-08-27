/**
* 
*/
#ifndef CWEBSOCKET_BASE_HPP
#define CWEBSOCKET_BASE_HPP

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <unistd.h>
#include "dji_linux_helpers.hpp"
#include <mutex>


typedef websocketpp::client<websocketpp::config::asio_client> client;


// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

extern mutex mu;

class WebSocketBase {
private:
	client c;
	websocketpp::connection_hdl hdl;
	std::string url;
	websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_;

private:
	void init();

public:
	WebSocketBase(const std::string &url):url(url), status(not_connect){
		init();
	}
	~WebSocketBase() {
		terminate();
	}

	const std::string getUrl() { return url; }

	void on_open(websocketpp::connection_hdl hdl);
	void on_fail(websocketpp::connection_hdl hdl);
	void on_close(websocketpp::connection_hdl hdl);
	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
	void send_message(std::string const& payload);
	void send_message(void const* payload, size_t len, websocketpp::lib::error_code& ec);
	virtual void process_message(std::string const& payload);
	virtual void after_close();

	enum Status {
		not_connect = 0,
		connecting = 1,
		connected = 2
	};

protected:
	Status status;
	void connect();
	void close();
	void terminate();
};

#endif