#pragma once

#include <Arduino.h>

namespace AIClient
{
    void begin(
        const char *serverUrl
    );

    String chat(
        const String &message
    );
}