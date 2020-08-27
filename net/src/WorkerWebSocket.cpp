#include "WorkerWebSocket.hpp"

void WorkerWebSocket::process_message(std::string const& payload) {
	if (bridge != nullptr) {
		bridge->start_action(payload);
	}
}

void WorkerWebSocket::after_close() {
	if (bridge != nullptr) {
		bridge->stop_worker();
	}
}

void WorkerWebSocket::start_connect() {
	connect();
}