#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::map<std::string, std::map<std::string, std::string>> configData;

void Config::loadConfig(const std::string &filepath)
{
    parseConfig(filepath);
}

std::string Config::get(const std::string &section, const std::string &key)
{
    if (configData.find(section) != configData.end())
    {
        if (configData[section].find(key) != configData[section].end())
        {
            return configData[section][key];
        }
    }
    return "";
}

void Config::parseConfig(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open config file: " << filepath << std::endl;
        return;
    }

    std::string line, section;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        if (line[0] == '[' && line.back() == ']')
        {
            section = line.substr(1, line.size() - 2);
        }
        else
        {
            std::istringstream is_line(line);
            std::string key, value;
            if (std::getline(is_line, key, '=') && std::getline(is_line, value))
            {
                configData[section][key] = value;
            }
        }
    }
}
