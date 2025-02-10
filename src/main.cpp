#include "config.h"
#include "mqtt_client.h"
#include "battery_status.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp> // JSON Library

using json = nlohmann::json;

#define CONFIG_FILE "/etc/ha-mqtt-device.conf"

void publishBatteryStatus(MQTTClient &client, const std::string &state_topic)
{
    if (BatteryStatus::isLaptop())
    {
        std::string status = BatteryStatus::getBatteryStatus();
        int percentage = BatteryStatus::getBatteryPercentage();
        int full_charge = BatteryStatus::getBatteryFullCharge();
        int time_remaining = BatteryStatus::getBatteryTimeRemaining();

        // Construct JSON payload with battery info
        json payload = {
            {"battery_status", status},
            {"battery_percentage", percentage},
            {"battery_full_charge", full_charge},
            {"battery_time_remaining", time_remaining},
        };

        client.publish(state_topic, payload.dump(), true);
        std::cout << "Published Battery Info: " << payload.dump() << std::endl;
    }
    else
    {
        std::cout << "This is not a laptop or no battery detected." << std::endl;
    }
}

int main()
{
    Config::loadConfig(CONFIG_FILE);

    std::string broker = Config::get("MQTT", "broker");
    int port = std::stoi(Config::get("MQTT", "port"));
    std::string client_id = Config::get("MQTT", "client_id");

    int interval = std::stoi(Config::get("App", "interval"));

    MQTTClient client(broker, port, client_id);
    std::string hostname = client.getHostname();
    std::string state_topic = "homeassistant/sensor/" + hostname + "/state";

    if (!client.connect())
    {
        std::cerr << "MQTT connection failed!" << std::endl;
        return 1;
    }

    std::cout << "MQTT Client connected successfully!" << std::endl;

    // Publish Discovery Message
    client.publishDiscoveryMessage();
    std::cout << "Home Assistant Discovery Message Sent." << std::endl;

    while (true)
    {
        // Publish battery status information
        publishBatteryStatus(client, state_topic);

        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }

    return 0;
}
