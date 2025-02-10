#include "battery_status.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

bool BatteryStatus::isLaptop()
{
    struct stat buffer;
    return (stat("/sys/class/power_supply", &buffer) == 0);
}

std::string BatteryStatus::getBatteryStatus()
{
    if (!isLaptop())
    {
        return "Not a laptop or no battery detected";
    }

    // Check for battery presence (BAT0 or BAT1 directory)
    std::string battery_path = "/sys/class/power_supply/BAT0/status";
    std::ifstream status_file(battery_path);

    if (!status_file.is_open())
    {
        battery_path = "/sys/class/power_supply/BAT1/status";
        status_file.open(battery_path);
    }

    if (!status_file.is_open())
    {
        return "Battery status unavailable";
    }

    std::string status;
    std::getline(status_file, status);
    status_file.close();

    return status;
}

int BatteryStatus::getBatteryPercentage()
{
    if (!isLaptop())
    {
        return -1; // Not a laptop or no battery detected
    }

    // Check for battery percentage (BAT0 or BAT1 directory)
    std::string battery_path = "/sys/class/power_supply/BAT0/capacity";
    std::ifstream capacity_file(battery_path);

    if (!capacity_file.is_open())
    {
        battery_path = "/sys/class/power_supply/BAT1/capacity";
        capacity_file.open(battery_path);
    }

    if (!capacity_file.is_open())
    {
        return -1; // Battery percentage unavailable
    }

    int percentage;
    capacity_file >> percentage;
    capacity_file.close();

    return percentage;
}

int BatteryStatus::getBatteryHealth()
{
    if (!isLaptop())
    {
        return -1; // Not a laptop or no battery detected
    }

    // Battery health info (full charge capacity)
    std::string battery_path = "/sys/class/power_supply/BAT0/charge_full";
    std::ifstream full_charge_file(battery_path);

    if (!full_charge_file.is_open())
    {
        battery_path = "/sys/class/power_supply/BAT1/charge_full";
        full_charge_file.open(battery_path);
    }

    if (!full_charge_file.is_open())
    {
        return -1; // Health info unavailable
    }

    int full_charge;
    full_charge_file >> full_charge;
    full_charge_file.close();

    return full_charge;
}

int BatteryStatus::getBatteryFullCharge()
{
    if (!isLaptop())
    {
        return -1; // Not a laptop or no battery detected
    }

    // Full charge capacity (BAT0 or BAT1)
    std::string battery_path = "/sys/class/power_supply/BAT0/charge_full";
    std::ifstream full_charge_file(battery_path);

    if (!full_charge_file.is_open())
    {
        battery_path = "/sys/class/power_supply/BAT1/charge_full";
        full_charge_file.open(battery_path);
    }

    if (!full_charge_file.is_open())
    {
        return -1; // Full charge capacity unavailable
    }

    int full_charge;
    full_charge_file >> full_charge;
    full_charge_file.close();

    return full_charge;
}

int BatteryStatus::getBatteryTimeRemaining()
{
    if (!isLaptop())
    {
        return -1; // Not a laptop or no battery detected
    }

    // Time remaining (if available)
    std::string battery_path = "/sys/class/power_supply/BAT0/time_to_empty";
    std::ifstream time_remaining_file(battery_path);

    if (!time_remaining_file.is_open())
    {
        battery_path = "/sys/class/power_supply/BAT1/time_to_empty";
        time_remaining_file.open(battery_path);
    }

    if (!time_remaining_file.is_open())
    {
        return -1; // Time remaining unavailable
    }

    int time_remaining;
    time_remaining_file >> time_remaining;
    time_remaining_file.close();

    return time_remaining;
}
