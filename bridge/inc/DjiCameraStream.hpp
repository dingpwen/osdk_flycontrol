/**
* DjiCameraStream.hpp
*/

#ifndef DJI_CAMERA_STREAM_HPP
#define DJI_CAMERA_STREAM_HPP

#include <dji_vehicle.hpp>
#include <dji_camera_image.hpp>
class WorkerWebSocket;

class DjiCameraStream {
private:
	const Vehicle* vehicle;
	WorkerWebSocket* worker;
	bool is_stream_starting;
public:
	DjiCameraStream(const Vehicle* vehicle) :vehicle(vehicle), worker(nullptr), is_stream_starting(false){}
	DjiCameraStream(const Vehicle* vehicle, WorkerWebSocket* worker) :vehicle(vehicle), worker(worker), is_stream_starting(false) {}

	void set_worker(WorkerWebSocket* worker);
	void start_camera_stream();
	void stop_camera_stream();
private:
	void stream_data_callback(CameraRGBImage pImg, void* userData);
	void get_zip_data(const CameraRGBImage &pImg, std::vector<uint8_t>& zip_data);
};
#endif