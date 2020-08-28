#include "DjiDataManager.hpp"
#include "WorkerWebSocket.hpp"

void DjiDataManager::start_worker() {
	gps_data.clear();
	gps_data.reserve(1000);
	running = true;
	std::thread(bind(&DjiDataManager::data_collection, this)).detach();
}

void DjiDataManager::data_collection() {
	while (running) {
		gps_data.push_back(vehicle->broadcast->getGlobalPosition());
		usleep(1000000);
	}
}

GlobalPosition DjiDataManager::get_gps_data() {
	return vehicle->broadcast->getGlobalPosition();
}

void DjiDataManager::stop_worker() {
	running = false;
}