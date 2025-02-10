#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include <mosquitto.h>

class MQTTClient
{
public:
    MQTTClient(const std::string &broker, int port, const std::string &client_id);
    ~MQTTClient();

    bool connect();
    void disconnect();
    bool publish(const std::string &topic, const std::string &message);

private:
    struct mosquitto *mosq;
    std::string broker;
    int port;
    std::string client_id;
};

#endif // MQTT_CLIENT_H
