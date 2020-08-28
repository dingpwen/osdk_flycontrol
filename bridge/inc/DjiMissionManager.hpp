/**
* file: DjiMissionManager.hpp
*/

#ifndef DJI_MOSSIONMANAGER_HPP
#define DJI_MOSSIONMANAGER_HPP

#include <DjiBase.hpp>

const int DEFAULT_PACKAGE_INDEX = 0;

class DjiMissionManager: public DjiBase {
public:
	DjiMissionManager(const Vehicle* vehicle) :DjiBase(vehicle, nullptr) {}
	DjiMissionManager(const Vehicle* vehicle, WorkerWebSocket* worker) :DjiBase(vehicle, worker) {}
	void start_mission(const std::string& message);
private:
	bool setUpSubscription(int responseTimeout);
	bool teardownSubscription(const int pkgIndex, int responseTimeout);

};

#endif