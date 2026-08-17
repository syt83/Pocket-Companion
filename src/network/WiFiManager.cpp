#include "WiFiManager.h"

#include <WiFi.h>


namespace
{
    bool wifiEnabled = true;
}


// ============================================================
// Initialize Wi-Fi
// ============================================================

void WiFiManager::begin()
{
    wifiEnabled = true;

    WiFi.mode(WIFI_STA);

    Serial.println(
        "Wi-Fi initialized."
    );
}


// ============================================================
// Connection status
// ============================================================

bool WiFiManager::connected()
{
    return
        wifiEnabled &&
        WiFi.status() == WL_CONNECTED;
}


// ============================================================
// Turn ON
// ============================================================

void WiFiManager::turnOn()
{
    wifiEnabled = true;

    WiFi.mode(
        WIFI_STA
    );

    Serial.println(
        "Wi-Fi turned ON."
    );
}


// ============================================================
// Turn OFF
// ============================================================

void WiFiManager::turnOff()
{
    wifiEnabled = false;

    WiFi.disconnect(
        true,
        true
    );

    WiFi.mode(
        WIFI_OFF
    );

    Serial.println(
        "Wi-Fi turned OFF."
    );
}


// ============================================================
// Current SSID
// ============================================================

String WiFiManager::currentSSID()
{
    if (!connected())
        return "Not connected";

    return WiFi.SSID();
}


// ============================================================
// IP Address
// ============================================================

String WiFiManager::ipAddress()
{
    if (!connected())
        return "0.0.0.0";

    return WiFi.localIP().toString();
}


// ============================================================
// Scan
// ============================================================

int WiFiManager::scan(
    String *ssids,
    int maxNetworks
)
{
    if (!wifiEnabled)
        return 0;

    int count =
        WiFi.scanNetworks(
            false,
            true
        );

    if (count <= 0)
    {
        WiFi.scanDelete();
        return 0;
    }

    int limit =
        count < maxNetworks
            ? count
            : maxNetworks;

    for (int i = 0; i < limit; i++)
    {
        ssids[i] = WiFi.SSID(i);
    }

    WiFi.scanDelete();

    return limit;
}


// ============================================================
// Connect to selected network
// ============================================================

bool WiFiManager::connect(
    const String &ssid,
    const String &password
)
{
    if (!wifiEnabled)
        return false;

    Serial.print(
        "Connecting to: "
    );

    Serial.println(ssid);

    WiFi.mode(
        WIFI_STA
    );

    WiFi.begin(
        ssid.c_str(),
        password.c_str()
    );

    uint32_t start =
        millis();

    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - start < 15000
    )
    {
        delay(250);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(
            "Wi-Fi connection failed."
        );

        return false;
    }

    Serial.println(
        "Wi-Fi connected."
    );

    Serial.print(
        "SSID: "
    );

    Serial.println(
        WiFi.SSID()
    );

    Serial.print(
        "IP: "
    );

    Serial.println(
        WiFi.localIP()
    );

    return true;
}