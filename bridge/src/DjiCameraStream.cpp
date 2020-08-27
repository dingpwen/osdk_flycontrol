#include "DjiCameraStream.hpp"
#include "WorkerWebSocket.hpp"

void DjiCameraStream::set_worker(WorkerWebSocket* worker) {
	this->worker = worker;
}


void DjiCameraStream::start_camera_stream() {
	if (vehicle != nullptr && !is_stream_starting) {
		char fpvName[] = "FPV_CAM";
		CameraImageCallback callback = (CameraImageCallback)&DjiCameraStream::stream_data_callback;
		is_stream_starting = true;
		vehicle->advancedSensing->startFPVCameraStream(callback, &fpvName);
	}
}

void DjiCameraStream::stop_camera_stream() {
	if (vehicle != nullptr) {
		vehicle->advancedSensing->stopFPVCameraStream();
	}
	is_stream_starting = false;
}

void DjiCameraStream::stream_data_callback(CameraRGBImage pImg, void* userData) {
	if (worker != nullptr) {
		std::vector<uint8_t> zip_data;
		get_zip_data(pImg, zip_data);
		int len = zip_data.size();
		uint8_t data[len];
		std::copy(zip_data.begin(), zip_data.end(), data);

		std::error_code ec;
		worker->send_message(data, len, ec);
		if (ec.value() != 0) {
			std::cout << "send img data error with code =" << ec.value() << std::endl;
		}
	}
}

void DjiCameraStream::get_zip_data(const CameraRGBImage &pImg, std::vector<uint8_t> &zip_data) {
	uint8_t data[pImg.rawData.size()];
	std::copy(pImg.rawData.begin(), pImg.rawData.end(), data);
	
	std::copy(data, data + sizeof(data)/sizeof(data[0]), zip_data.begin());
}