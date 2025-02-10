#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

std::map<std::string, std::map<std::string, std::string>> configData;

static std::string trim(const std::string &str)
{
    // Remove leading spaces
    size_t first = str.find_first_not_of(" \t\n\r");
    // Remove trailing spaces
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == std::string::npos || last == std::string::npos)
        return ""; // Empty string if no non-space characters are found

    return str.substr(first, last - first + 1); // Return trimmed string
}

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
        // Remove leading/trailing spaces from the line
        line = trim(line);

        // Skip empty lines or comments
        if (line.empty() || line[0] == '#')
            continue;

        // Check for section header [Section]
        if (line[0] == '[' && line.back() == ']')
        {
            section = line.substr(1, line.size() - 2); // Remove brackets
        }
        else
        {
            // Parse key-value pair (key = value)
            std::istringstream is_line(line);
            std::string key, value;
            if (std::getline(is_line, key, '=') && std::getline(is_line, value))
            {
                key = trim(key);     // Trim key
                value = trim(value); // Trim value
                if (!section.empty())
                {
                    configData[section][key] = value;
                }
                else
                {
                    std::cerr << "Error: Key-value pair found outside of any section!" << std::endl;
                }
            }
        }
    }
}
