#include <Arduino.h>

#include "display/Display.h"
#include "ui/UI.h"

#include "storage/Storage.h"

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
}

void loop()
{
    Display::update();
}