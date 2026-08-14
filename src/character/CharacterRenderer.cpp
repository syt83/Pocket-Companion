#include "CharacterRenderer.h"

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <stdlib.h>


CharacterRenderer::CharacterRenderer()
    : imageObject(nullptr),
      imageWidth(0),
      imageHeight(0),
      imageBuffer(nullptr),
      bufferSize(0)
{
}


bool CharacterRenderer::begin(lv_obj_t *parent)
{
    if (parent == nullptr)
    {
        Serial.println(
            "[CharacterRenderer] Parent object is null."
        );

        return false;
    }

    imageObject = lv_img_create(parent);

    if (imageObject == nullptr)
    {
        Serial.println(
            "[CharacterRenderer] Failed to create LVGL image."
        );

        return false;
    }

    return true;
}


bool CharacterRenderer::allocateBuffer(
    size_t pixelCount
)
{
    size_t requiredSize =
        pixelCount * sizeof(lv_color_t);

    if (imageBuffer != nullptr)
    {
        free(imageBuffer);

        imageBuffer = nullptr;
    }

    imageBuffer =
        static_cast<lv_color_t *>(
            heap_caps_malloc(
                requiredSize,
                MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT
            )
        );

    if (imageBuffer == nullptr)
    {
        Serial.println(
            "[CharacterRenderer] PSRAM allocation failed."
        );

        return false;
    }

    bufferSize = requiredSize;

    return true;
}


bool CharacterRenderer::loadRGB565(
    const char *path
)
{
    if (path == nullptr)
        return false;

    File file = SD.open(
        path,
        FILE_READ
    );

    if (!file)
    {
        Serial.print(
            "[CharacterRenderer] Failed to open: "
        );

        Serial.println(path);

        return false;
    }

    // --------------------------------------------------------
    // RGB565 파일 헤더
    //
    // uint16_t width
    // uint16_t height
    // 이후 pixel data
    // --------------------------------------------------------

    uint16_t width = 0;
    uint16_t height = 0;

    if (file.read(
            reinterpret_cast<uint8_t *>(&width),
            sizeof(width)
        ) != sizeof(width))
    {
        file.close();
        return false;
    }

    if (file.read(
            reinterpret_cast<uint8_t *>(&height),
            sizeof(height)
        ) != sizeof(height))
    {
        file.close();
        return false;
    }

    Serial.print(
        "[CharacterRenderer] Image size: "
    );

    Serial.print(width);
    Serial.print(" x ");
    Serial.println(height);


    // --------------------------------------------------------
    // 메모리 확보
    // --------------------------------------------------------

    size_t pixelCount =
        static_cast<size_t>(width) *
        static_cast<size_t>(height);

    if (!allocateBuffer(pixelCount))
    {
        file.close();
        return false;
    }


    // --------------------------------------------------------
    // Pixel data
    // --------------------------------------------------------

    size_t expectedBytes =
        pixelCount * sizeof(lv_color_t);

    size_t totalRead = 0;

    while (totalRead < expectedBytes)
    {
        size_t remaining =
            expectedBytes - totalRead;

        size_t chunk =
            remaining > 4096
                ? 4096
                : remaining;

        size_t readBytes =
            file.read(
                reinterpret_cast<uint8_t *>(
                    imageBuffer
                ) + totalRead,
                chunk
            );

        if (readBytes == 0)
        {
            Serial.println(
                "[CharacterRenderer] Unexpected EOF."
            );

            file.close();

            free(imageBuffer);
            imageBuffer = nullptr;

            return false;
        }

        totalRead += readBytes;
    }

    file.close();


    imageWidth = width;
    imageHeight = height;


    // --------------------------------------------------------
    // LVGL image descriptor
    // --------------------------------------------------------

    static lv_img_dsc_t imageDescriptor;

    imageDescriptor.header.cf =
        LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED;

    imageDescriptor.header.always_zero = 0;

    imageDescriptor.header.w =
        imageWidth;

    imageDescriptor.header.h =
        imageHeight;

    imageDescriptor.data_size =
        expectedBytes;

    imageDescriptor.data =
        reinterpret_cast<const uint8_t *>(
            imageBuffer
        );


    lv_img_set_src(
        imageObject,
        &imageDescriptor
    );


    Serial.println(
        "[CharacterRenderer] Image loaded."
    );

    return true;
}


bool CharacterRenderer::loadImage(
    const char *path
)
{
    return loadRGB565(path);
}


void CharacterRenderer::setPosition(
    int32_t x,
    int32_t y
)
{
    if (imageObject == nullptr)
        return;

    lv_obj_set_pos(
        imageObject,
        x,
        y
    );
}


lv_obj_t *CharacterRenderer::object()
{
    return imageObject;
}


uint16_t CharacterRenderer::width() const
{
    return imageWidth;
}


uint16_t CharacterRenderer::height() const
{
    return imageHeight;
}