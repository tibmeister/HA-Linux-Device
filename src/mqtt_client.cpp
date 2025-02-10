#include "mqtt_client.h"
#include <iostream>

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

bool MQTTClient::publish(const std::string &topic, const std::string &message)
{
    int ret = mosquitto_publish(mosq, nullptr, topic.c_str(), message.size(), message.c_str(), 0, false);
    return (ret == MOSQ_ERR_SUCCESS);
}
