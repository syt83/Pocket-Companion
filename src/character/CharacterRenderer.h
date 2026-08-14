#pragma once

#include <stdint.h>
#include <lvgl.h>

class CharacterRenderer
{
public:
    CharacterRenderer();

    bool begin(lv_obj_t *parent);

    bool loadImage(const char *path);

    void setPosition(int32_t x, int32_t y);

    lv_obj_t *object();

    uint16_t width() const;
    uint16_t height() const;

private:
    lv_obj_t *imageObject;

    uint16_t imageWidth;
    uint16_t imageHeight;

    lv_color_t *imageBuffer;
    size_t bufferSize;

    bool allocateBuffer(size_t pixelCount);

    bool loadRGB565(const char *path);
};