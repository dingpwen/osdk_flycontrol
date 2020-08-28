/**
* DjiCameraStream.hpp
*/

#ifndef DJI_CAMERA_STREAM_HPP
#define DJI_CAMERA_STREAM_HPP

#include <DjiBase.hpp>
#include <dji_camera_image.hpp>

class DjiCameraStream: public DjiBase{
private:
	bool is_stream_starting;
public:
	DjiCameraStream(const Vehicle* vehicle) :DjiBase(vehicle, nullptr), is_stream_starting(false){}
	DjiCameraStream(const Vehicle* vehicle, WorkerWebSocket* worker) :DjiBase(vehicle, worker), is_stream_starting(false) {}

	void start_camera_stream();
	void stop_camera_stream();
private:
	void stream_data_callback(CameraRGBImage pImg, void* userData);
	void get_zip_data(const CameraRGBImage &pImg, std::vector<uint8_t>& zip_data);
};
#endif