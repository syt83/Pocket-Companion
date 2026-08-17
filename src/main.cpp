#include <Arduino.h>

#include "display/Display.h"
#include "ui/UI.h"

#include "storage/Storage.h"

#include "network/WiFiManager.h"
#include "network/AIClient.h"

void setup()
{
    Serial.begin(115200);

    delay(500);

    Serial.println();
    Serial.println("==============================");
    Serial.println("   Pocket Companion");
    Serial.println("==============================");

    if (!Display::begin())
    {
        Serial.println("Display initialization failed.");
        return;
    }

    Storage::begin();
    Storage::listRoot();

    UI::begin();

    Serial.println("Pocket Companion started.");

    AIClient::begin(
        "http://10.200.69.232:800１" // 나중에 라즈베리 파이 ip로
    );

    WiFiManager::begin();
}

void loop()
{
    Display::update();
}