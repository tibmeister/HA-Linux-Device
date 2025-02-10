#ifndef BATTERY_STATUS_H
#define BATTERY_STATUS_H

#include <string>

class BatteryStatus
{
public:
    static bool isLaptop();
    static std::string getBatteryStatus();
    static int getBatteryPercentage();
    static int getBatteryHealth();
    static int getBatteryFullCharge();
    static int getBatteryTimeRemaining();
};

#endif // BATTERY_STATUS_H
