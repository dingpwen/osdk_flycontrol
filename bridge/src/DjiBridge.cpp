#include "DjiBridge.hpp"
#include "WorkerWebSocket.hpp"

void DjiBridge::start_worker(const std::string& url) {
	if (vehicle == nullptr || is_working) {
		return;
	}
	is_working = true;
	if (worker == nullptr) {
		worker = new WorkerWebSocket(this, url);
	} else if (url.compare(worker->getUrl()) != 0) {
		delete worker;
		worker = new WorkerWebSocket(this, url);
	}
	worker->start_connect();
	camera_stream->set_worker(worker);
}

void DjiBridge::stop_worker() {
	is_working = false;
	camera_stream->stop_camera_stream();
	camera_stream->set_worker(nullptr);
}

DjiBridge::~DjiBridge() {
	if (worker != nullptr) {
		delete worker;
		worker = nullptr;
	}
	if (camera_stream != nullptr) {
		delete camera_stream;
		camera_stream = nullptr;
	}
}

void DjiBridge::start_action(const std::string& message) {
	dji_ops op = get_action_type(message);
	switch(op){
	case dji_camera_stream_start:
		camera_stream->start_camera_stream();
		break;
	case dji_camera_stream_stop:
		camera_stream->stop_camera_stream();
		break;
	case dji_mission:
		break;
	case dji_data_gps:
		break;
	case dji_data_battery:
		break;
	default:
		break;
	}
}

DjiBridge::dji_ops DjiBridge::get_action_type(const std::string& message) {
	return dji_camera_stream_start;
}