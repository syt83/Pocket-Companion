#include "Display.h"

#include <Arduino_GFX_Library.h>
#include "bsp_cst816.h"
#include <Wire.h>
#include "esp_heap_caps.h"

#define LCD_SCLK 39
#define LCD_MOSI 38
#define LCD_MISO 40
#define LCD_DC   42
#define LCD_RST  -1
#define LCD_CS   45
#define LCD_BL   1

#define TP_SDA   48
#define TP_SCL   47

#define LCD_ROTATION 1
#define LCD_H_RES 240
#define LCD_V_RES 320

#define BACKLIGHT_FREQ 5000

static Arduino_DataBus *bus =
    new Arduino_ESP32SPI(
        LCD_DC,
        LCD_CS,
        LCD_SCLK,
        LCD_MOSI,
        LCD_MISO
    );

static Arduino_GFX *gfx =
    new Arduino_ST7789(
        bus,
        LCD_RST,
        LCD_ROTATION,
        true,
        LCD_H_RES,
        LCD_V_RES
    );

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf = nullptr;
static lv_disp_drv_t disp_drv;

static uint32_t screenWidth;
static uint32_t screenHeight;
static uint32_t bufSize;

static void displayFlush(
    lv_disp_drv_t *drv,
    const lv_area_t *area,
    lv_color_t *color_p)
{
    lv_disp_flush_ready(drv);
}

static void touchRead(
    lv_indev_drv_t *drv,
    lv_indev_data_t *data)
{
    uint16_t x;
    uint16_t y;

    bsp_touch_read();

    if (bsp_touch_get_coordinates(&x, &y))
    {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

bool Display::begin()
{
    if (!gfx->begin())
    {
        Serial.println("LCD initialization failed!");
        return false;
    }

    gfx->fillScreen(BLACK);

    // LCD backlight
    analogWrite(LCD_BL, BACKLIGHT_FREQ);

    // Touch I2C
    Wire.begin(TP_SDA, TP_SCL);

    bsp_touch_init(
        &Wire,
        gfx->getRotation(),
        gfx->width(),
        gfx->height()
    );

    // LVGL
    lv_init();

    screenWidth = gfx->width();
    screenHeight = gfx->height();
    bufSize = screenWidth * screenHeight;

    disp_draw_buf =
        (lv_color_t *)heap_caps_malloc(
            bufSize * 2,
            MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT
        );

    if (!disp_draw_buf)
    {
        disp_draw_buf =
            (lv_color_t *)heap_caps_malloc(
                bufSize * 2,
                MALLOC_CAP_8BIT
            );
    }

    if (!disp_draw_buf)
    {
        Serial.println("LVGL buffer allocation failed!");
        return false;
    }

    lv_disp_draw_buf_init(
        &draw_buf,
        disp_draw_buf,
        nullptr,
        bufSize
    );

    // Display driver
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = displayFlush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.direct_mode = true;

    lv_disp_drv_register(&disp_drv);

    // Touch driver
    static lv_indev_drv_t indev_drv;

    lv_indev_drv_init(&indev_drv);

    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchRead;

    lv_indev_drv_register(&indev_drv);

    Serial.println("Display initialized.");

    return true;
}

void Display::update()
{
    lv_timer_handler();

#if (LV_COLOR_16_SWAP != 0)

    gfx->draw16bitBeRGBBitmap(
        0,
        0,
        (uint16_t *)disp_draw_buf,
        screenWidth,
        screenHeight
    );

#else

    gfx->draw16bitRGBBitmap(
        0,
        0,
        (uint16_t *)disp_draw_buf,
        screenWidth,
        screenHeight
    );

#endif

    delay(5);
}

uint16_t Display::width()
{
    return screenWidth;
}

uint16_t Display::height()
{
    return screenHeight;
}