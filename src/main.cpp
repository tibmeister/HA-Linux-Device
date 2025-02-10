#include "config.h"
#include "mqtt_client.h"
#include <iostream>
#include <thread>
#include <chrono>

#define CONFIG_FILE "/etc/mqtt-app.conf"

int main()
{
    Config::loadConfig(CONFIG_FILE);

    std::string broker = Config::get("MQTT", "broker");
    int port = std::stoi(Config::get("MQTT", "port"));
    std::string topic = Config::get("MQTT", "topic");
    std::string client_id = Config::get("MQTT", "client_id");

    int interval = std::stoi(Config::get("App", "interval"));

    MQTTClient client(broker, port, client_id);

    if (!client.connect())
    {
        std::cerr << "MQTT connection failed!" << std::endl;
        return 1;
    }

    std::cout << "MQTT Client connected successfully!" << std::endl;

    while (true)
    {
        std::string message = "Test Message";
        if (client.publish(topic, message))
        {
            std::cout << "Message published: " << message << std::endl;
        }
        else
        {
            std::cerr << "Failed to publish message" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }

    return 0;
}
