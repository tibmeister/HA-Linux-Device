#include "mqtt_client.h"
#include <iostream>
#include <nlohmann/json.hpp> // JSON library
#include <mosquitto.h>

using json = nlohmann::json;

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

MQTTClient::MQTTClient(const std::string &broker, int port, const std::string &client_id, const std::string &username, const std::string &password)
    : broker(broker), port(port), client_id(client_id), username(username), password(password)
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
    // If username and password are provided, use them to authenticate
    if (!username.empty() && !password.empty())
    {
        mosquitto_username_pw_set(mosq, username.c_str(), password.c_str());
    }

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
    std::string discovery_topic = "homeassistant/sensor/" + client_id + "/config";

    json discovery_msg = {
        {"name", "HA MQTT Device (" + client_id + ")"},
        {"unique_id", "ha_mqtt_device_" + client_id},
        {"state_topic", "homeassistant/sensor/" + client_id + "/state"},
        {"unit_of_measurement", "°C"},
        {"device", {{"identifiers", client_id}, {"name", "HA MQTT Device (" + client_id + ")"}, {"manufacturer", "Custom"}, {"model", "v1.0"}}}};

    std::string payload = discovery_msg.dump();
    publish(discovery_topic, payload, true);
}

void MQTTClient::setTLSOptions(bool tls_enabled, bool skip_verify)
{
    if (tls_enabled)
    {
        if (mosquitto_tls_set(mosq, NULL, NULL, NULL, NULL, NULL) != MOSQ_ERR_SUCCESS)
        {
            std::cerr << "Failed to set TLS options." << std::endl;
            return;
        }

        // Optionally, skip verification if configured
        if (skip_verify)
        {
            if (mosquitto_tls_insecure_set(mosq, true) != MOSQ_ERR_SUCCESS)
            {
                std::cerr << "Failed to set TLS insecure (skip verification)." << std::endl;
                return;
            }
        }
    }
}
