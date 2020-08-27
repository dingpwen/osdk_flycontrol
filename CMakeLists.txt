project(flycontrol)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread  -lboost_system -g -O0")


#websocket with boost
FIND_PACKAGE(Boost REQUIRED system filesystem)
if(Boost_FOUND)
    message(STATUS "boost include path is : ${Boost_INCLUDE_DIRS}")
    message(STATUS "boost library path is : ${Boost_LIBRARY_DIRS}")
    message(STATUS "boost libraries is : ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(WARNING "boost not found.")
endif()

set(WEBSOCKET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/websocketpp)
include_directories(${WEBSOCKET_DIR})

#DJ OSDK part

#include_directories(/usr/local/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/linux/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/linux/hal)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/linux/osal)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/net/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/bridge/inc)

message((${DJIOSDK_INCLUDE_DIRS}))

if(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|i686|x86|AMD64|x86_64")
    set(ARCH x86)
    add_definitions(-DDJIOSDK_HARDWARE_TYPE=3)
elseif((CMAKE_SYSTEM_PROCESSOR MATCHES "^arm*") OR (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch"))
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH armv7)
        add_definitions(-DDJIOSDK_HARDWARE_TYPE=1)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH armv8)
        add_definitions(-DDJIOSDK_HARDWARE_TYPE=2)
    endif()
elseif()
    add_definitions(-DDJIOSDK_HARDWARE_TYPE=0)
endif()

set(ADVANCED_SENSING 1)
set(WAYPT2_CORE 1)

if (ADVANCED_SENSING)
  add_definitions(-DADVANCED_SENSING)
endif()

if (WAYPT2_CORE)
    add_definitions(-DWAYPT2_CORE)
endif()

set(OSDK_CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Onboard-SDK/osdk-core)
# Add module path for <name>.cmake conf files
set(CMAKE_MODULE_PATH ${OSDK_CORE_PATH}/cmake-modules)

message("-----------OSDK_LIB_HEADERS_DIR------------")
set(OSDK_LIB_HEADERS_DIR
        ${OSDK_CORE_PATH}/api/inc
        ${OSDK_CORE_PATH}/modules/inc/payload
        ${OSDK_CORE_PATH}/modules/inc/flight
        ${OSDK_CORE_PATH}/modules/inc/mop
        ${OSDK_CORE_PATH}/platform/inc
        ${OSDK_CORE_PATH}/logger/inc
        ${OSDK_CORE_PATH}/utility/inc
        ${OSDK_CORE_PATH}/linker/${ARCH}/inc
        ${OSDK_CORE_PATH}/modules/inc/filemgr
        )

include_directories(${OSDK_LIB_HEADERS_DIR})
message((${OSDK_LIB_HEADERS_DIR}))

message("-----------ADVANCED_SENSING_HEADERS_DIR------------")
set(ADVANCED_SENSING_SOURCE_ROOT ${OSDK_CORE_PATH}/advanced-sensing)
set(ADVANCED_SENSING_HEADERS_DIR
    ${ADVANCED_SENSING_SOURCE_ROOT}/api/inc
    ${ADVANCED_SENSING_SOURCE_ROOT}/platform/inc
    ${ADVANCED_SENSING_SOURCE_ROOT}/protocol/inc
    ${ADVANCED_SENSING_SOURCE_ROOT}/camera_stream/src
    ${ADVANCED_SENSING_SOURCE_ROOT}/ori-osdk-core/protocol/inc
    ${ADVANCED_SENSING_SOURCE_ROOT}/ori-osdk-core/hal/inc
    ${ADVANCED_SENSING_SOURCE_ROOT}/ori-osdk-core/platform/linux/inc
    )
include_directories(${ADVANCED_SENSING_HEADERS_DIR})
message((${ADVANCED_SENSING_HEADERS_DIR}))

find_package(LibUSB REQUIRED)
find_package(FFMPEG REQUIRED)
#find_package(Waypt2Core REQUIRED)

if(FFMPEG_FOUND)
    message( STATUS "Found FFmpeg ${FFMPEG_VERSION} installed in the system.")
    message( STATUS " - Includes: ${FFMPEG_INCLUDE_DIRS}")
    message( STATUS " - Libraries: ${FFMPEG_LIBRARIES}")
else()
    message("Cannot Find FFMPEG")
endif(FFMPEG_FOUND)

include_directories(${LIBUSB_1_INCLUDE_DIRS})
include_directories(${FFMPEG_INCLUDE_DIR})

#link_libraries(dji-linker)
#link_libraries(advanced-sensing)
link_libraries(djiosdk-core)
link_libraries(${LIBUSB_1_LIBRARIES})
#link_libraries(${FFMPEG_LIBRARIES})

#src
file(GLOB SOURCE_FILES *.hpp *.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/common/dji_linux_environment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/common/dji_linux_helpers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/hal/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/linux/osal/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/net/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/bridge/src/*.cpp
        ${LINKER_HEADER_SRC}
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} pthread dji-linker advanced-sensing ${FFMPEG_LIBRARIES} swresample)


