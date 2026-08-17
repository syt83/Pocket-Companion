#pragma once

#include <Arduino.h>

namespace WiFiManager
{
    void begin();

    bool connected();

    void turnOn();

    void turnOff();

    String currentSSID();

    String ipAddress();

    int scan(
        String *ssids,
        int maxNetworks
    );

    bool connect(
        const String &ssid,
        const String &password
    );
}