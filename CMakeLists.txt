cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ha-mqtt-device VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Mosquitto library
find_package(PkgConfig REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)

# Find the nlohmann_json library
find_package(nlohmann_json REQUIRED)

# Define source files
set(SOURCES
    src/main.cpp
    src/config.cpp
    src/mqtt_client.cpp
)

# Define include directories
include_directories(
    include
    ${MOSQUITTO_INCLUDE_DIRS}
)

# Create the executable
add_executable(ha-mqtt-device ${SOURCES})

# Link Mosquitto library
target_link_libraries(ha-mqtt-device ${MOSQUITTO_LIBRARIES})

# Install target
install(TARGETS ha-mqtt-device DESTINATION /usr/local/bin)

# Install configuration file
install(FILES config/ha-mqtt-device.conf DESTINATION /etc)

# Install systemd service file
install(FILES systemd/ha-mqtt-device.service DESTINATION /etc/systemd/system)

# Add uninstall target
add_custom_target(uninstall
    COMMAND xargs rm < install_manifest.txt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)