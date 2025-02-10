#include "mqtt_client.h"
#include <iostream>
#include <unistd.h>
#include <nlohmann/json.hpp> // JSON library

using json = nlohmann::json;

std::string MQTTClient::getHostname()
{
    char hostname[128];
    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        return std::string(hostname);
    }
    else
    {
        return "unknown_device";
    }
}

MQTTClient::MQTTClient(const std::string &broker, int port, const std::string &client_id)
    : broker(broker), port(port), client_id(client_id)
{
    mosquitto_lib_init();
    mosq = mosquitto_new(client_id.c_str(), true, nullptr);
    if (!mosq)
    {
        std::cerr << "Failed to create Mosquitto instance." << std::endl;
    }
}

MQTTClient::~MQTTClient()
{
    disconnect();
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

bool MQTTClient::connect()
{
    if (mosquitto_connect(mosq, broker.c_str(), port, 60) != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Failed to connect to MQTT broker: " << broker << std::endl;
        return false;
    }
    return true;
}

void MQTTClient::disconnect()
{
    mosquitto_disconnect(mosq);
}

bool MQTTClient::publish(const std::string &topic, const std::string &message, bool retain)
{
    int ret = mosquitto_publish(mosq, nullptr, topic.c_str(), message.size(), message.c_str(), 0, retain);
    return (ret == MOSQ_ERR_SUCCESS);
}

// Publish the Home Assistant Discovery Message
void MQTTClient::publishDiscoveryMessage()
{
    std::string hostname = getHostname();
    std::string discovery_topic = "homeassistant/sensor/" + hostname + "/config";

    json discovery_msg = {
        {"name", "HA MQTT Device (" + hostname + ")"},
        {"unique_id", "ha_mqtt_device_" + hostname},
        {"state_topic", "homeassistant/sensor/" + hostname + "/state"},
        {"unit_of_measurement", "°C"},
        {"device", {{"identifiers", hostname}, {"name", "HA MQTT Device (" + hostname + ")"}, {"manufacturer", "Custom"}, {"model", "v1.0"}}}};

    std::string payload = discovery_msg.dump();
    publish(discovery_topic, payload, true);
}
