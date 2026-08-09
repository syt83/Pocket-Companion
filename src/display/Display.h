#pragma once

#include <Arduino.h>
#include <lvgl.h>

namespace Display
{
    bool begin();

    void update();

    uint16_t width();
    uint16_t height();
}