/**
* DjiBridge.hpp
* 
* DjiBridge works as bridge between dji onboard sdk and net websocket request, such as when we receive a command from client side, 
* we translate it to an aircraft action, and then call function to dji onboard sdk.
*/
#ifndef DJI_BRIDGE_HPP
#define DJI_BRIDGE_HPP

#include <dji_vehicle.hpp>
#include <iostream>
#include "DjiCameraStream.hpp"
#include "DjiDataManager.hpp"
#include "DjiMissionManager.hpp"

class WorkerWebSocket;

class DjiBridge {
private:
	const Vehicle* vehicle;
	bool is_working;
	WorkerWebSocket* worker;
	DjiCameraStream* camera_stream;
	DjiDataManager* data_manager;
	DjiMissionManager* mossion_manager;

public:
	DjiBridge(const Vehicle* vehicle):vehicle(vehicle), is_working(false), worker(nullptr){
		camera_stream = new DjiCameraStream(vehicle);
		data_manager = new DjiDataManager(vehicle);
		mossion_manager = new DjiMissionManager(vehicle);
	}
	~DjiBridge();

	void start_worker(const std::string& url);
	void stop_worker();
	void start_action(const std::string& message);

	enum dji_ops {
		dji_camera_stream_start = 0,
		dji_camera_stream_stop = 1,
		dji_mission = 2,
		dji_data_gps = 3,
		dji_data_battery = 4
	};


private:
	dji_ops get_action_type(const std::string &message);
};
#endif