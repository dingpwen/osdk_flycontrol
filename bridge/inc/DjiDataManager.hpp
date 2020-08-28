/**
* DjiDataManager.hpp
*/
#ifndef DJI_DATAMANAGER_HPP
#define DJI_DATAMANAGER_HPP

#include <DjiBase.hpp>
#include <dji_telemetry.hpp>
using DJI::OSDK::Telemetry::GlobalPosition;

class DjiDataManager: public DjiBase {
private:
	bool running;
	std::vector< GlobalPosition> gps_data;

public:
	DjiDataManager(const Vehicle* vehicle) :DjiBase(vehicle, nullptr), running(false){}
	DjiDataManager(const Vehicle* vehicle, WorkerWebSocket* worker) :DjiBase(vehicle, worker), running(false) {}

	void start_worker();
	void data_collection();
	void stop_worker();
	GlobalPosition get_gps_data();
};

#endif