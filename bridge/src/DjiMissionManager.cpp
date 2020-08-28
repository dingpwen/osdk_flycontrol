#include "DjiMissionManager.hpp"
#include "osdkosal_linux.h"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void DjiMissionManager::start_mission(const std::string& message) {

}

bool DjiMissionManager::setUpSubscription(int responseTimeout) {
    // Telemetry: Verify the subscription
    ACK::ErrorCode subscribeStatus;

    subscribeStatus = vehicle->subscribe->verify(responseTimeout);
    if (ACK::getError(subscribeStatus) != ACK::SUCCESS)
    {
        ACK::getErrorCodeMessage(subscribeStatus, __func__);
        return false;
    }

    // Telemetry: Subscribe to flight status and mode at freq 10 Hz
    int       freq = 10;
    TopicName topicList10Hz[] = { TOPIC_GPS_FUSED };
    int       numTopic = sizeof(topicList10Hz) / sizeof(topicList10Hz[0]);
    bool      enableTimestamp = false;

    bool pkgStatus = vehicle->subscribe->initPackageFromTopicList(
        DEFAULT_PACKAGE_INDEX, numTopic, topicList10Hz, enableTimestamp, freq);
    if (!(pkgStatus))
    {
        return pkgStatus;
    }

    // Start listening to the telemetry data
    subscribeStatus =
        vehicle->subscribe->startPackage(DEFAULT_PACKAGE_INDEX, responseTimeout);
    if (ACK::getError(subscribeStatus) != ACK::SUCCESS)
    {
        ACK::getErrorCodeMessage(subscribeStatus, __func__);
        // Cleanup
        ACK::ErrorCode ack =
            vehicle->subscribe->removePackage(DEFAULT_PACKAGE_INDEX, responseTimeout);
        if (ACK::getError(ack))
        {
            std::cout << "Error unsubscribing; please restart the drone/FC to get "
                "back to a clean state.\n";
        }
        return false;
    }
    return true;
}

bool DjiMissionManager::teardownSubscription(const int pkgIndex, int responseTimeout) {
    ACK::ErrorCode ack =
        vehicle->subscribe->removePackage(pkgIndex, responseTimeout);
    if (ACK::getError(ack))
    {
        std::cout << "Error unsubscribing; please restart the drone/FC to get back "
            "to a clean state.\n";
        return false;
    }
    return true;
}