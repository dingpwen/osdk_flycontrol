#include "MainWebSocket.hpp"
#include <iostream>
#include <unistd.h>
#include <thread>

void MainWebSocket::start_run() {
	connect();
}

void MainWebSocket::after_close() {
	std::cout << "MainWebSocket::after_close\n";
	std::thread(std::bind(&MainWebSocket::reconnect, this)).detach();
}

void MainWebSocket::reconnect() {
	while (status != connected) {
		usleep(10000000);
		std::cout << "retry connect to " << getUrl() << std::endl;
		connect();
	}
}

void MainWebSocket::process_message(std::string const& payload) {
	std::cout << "Recieve text message:" << payload << std::endl;
	std::string url("");
	if (is_register_request(payload, url)) {
		if (url.find_first_of("ws://") == 0) {
			bridge->start_worker(url);
		}
	}
	else {
		bridge->start_action(payload);
	}
}

bool MainWebSocket::is_register_request(const std::string& message, std::string& url) {
	return true;
}