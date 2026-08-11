#pragma once

namespace Storage
{
    bool begin();

    bool exists(const char *path);

    void listRoot();
}