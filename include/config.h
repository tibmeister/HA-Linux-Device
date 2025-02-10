#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:
    static void loadConfig(const std::string &filepath);
    static std::string get(const std::string &section, const std::string &key);

private:
    static void parseConfig(const std::string &filepath);
};

#endif // CONFIG_H
