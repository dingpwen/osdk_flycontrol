#include <iostream>
#include <unistd.h>
#include "dji_linux_helpers.hpp"
#include "MainWebSocket.hpp"


int main(int argc, char *argv[]) {
    // Setup OSDK.
    LinuxSetup linuxEnvironment(argc, argv);
    Vehicle* vehicle = linuxEnvironment.getVehicle();
    const char* acm_dev = linuxEnvironment.getEnvironment()->getDeviceAcm().c_str();
    vehicle->advancedSensing->setAcmDevicePath(acm_dev);
    if (vehicle == NULL) {
        std::cout << "Vehicle not initialized, exiting.\n";
        //return -1;
    }

    DjiBridge bridge(vehicle);
    MainWebSocket main_socket(&bridge, linuxEnvironment.getEnvironment()->getServerUrl());
    main_socket.start_run();
    //keepalive
    while (true) {
        sleep(5000);
    }
    std::cout << "main exit with object:" << &main_socket << std::endl;
    return 0;
}
