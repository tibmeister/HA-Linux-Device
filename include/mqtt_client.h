#include <string>
#include <mosquitto.h>

class MQTTClient
{
public:
    MQTTClient(const std::string &broker, int port, const std::string &client_id);
    MQTTClient(const std::string &broker, int port, const std::string &client_id, const std::string &username, const std::string &password);
    ~MQTTClient();

    bool connect();
    void disconnect();
    bool publish(const std::string &topic, const std::string &message, bool retain);
    void publishDiscoveryMessage();
    void setTLSOptions(bool tls_enabled, bool skip_verify);

private:
    std::string broker;
    int port;
    std::string client_id;
    std::string username;
    std::string password;
    bool tls_enabled = false; // TLS flag
    bool skip_verify = false; // Skip SSL verification flag

    mosquitto *mosq;
};
