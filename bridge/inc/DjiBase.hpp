/**
* file:DjiBase.hpp
*/
#ifndef DJI_BASE_HPP
#define DJI_BASE_HPP

#include <dji_vehicle.hpp>
class WorkerWebSocket;

class DjiBase {
protected:
	const Vehicle* vehicle;
	WorkerWebSocket* worker;

public:
	DjiBase(const Vehicle* vehicle, WorkerWebSocket* worker) :vehicle(vehicle), worker(worker) {}
	void set_worker(WorkerWebSocket* worker) {
		this->worker = worker;
	}
};

#endif