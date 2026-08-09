#include "UI.h"

#include <lvgl.h>

static lv_obj_t *statusLabel;
static lv_obj_t *messageLabel;

static lv_obj_t *leftEye;
static lv_obj_t *rightEye;

static bool happy = true;

static void buttonEvent(lv_event_t *event)
{
    happy = !happy;

    if (happy)
    {
        lv_label_set_text(
            messageLabel,
            "Hello!"
        );

        lv_label_set_text(
            statusLabel,
            "I'm happy :)"
        );

        lv_obj_set_height(leftEye, 24);
        lv_obj_set_height(rightEye, 24);
    }
    else
    {
        lv_label_set_text(
            messageLabel,
            "Hmm..."
        );

        lv_label_set_text(
            statusLabel,
            "What's going on?"
        );

        lv_obj_set_height(leftEye, 4);
        lv_obj_set_height(rightEye, 4);
    }
}

static lv_obj_t *createEye(
    lv_obj_t *parent,
    int x,
    int y)
{
    lv_obj_t *eye = lv_obj_create(parent);

    lv_obj_set_size(eye, 24, 24);
    lv_obj_set_pos(eye, x, y);

    lv_obj_set_style_radius(
        eye,
        LV_RADIUS_CIRCLE,
        0
    );

    lv_obj_set_style_bg_color(
        eye,
        lv_color_black(),
        0
    );

    lv_obj_set_style_border_width(
        eye,
        0,
        0
    );

    return eye;
}

void UI::begin()
{
    lv_obj_t *screen = lv_scr_act();

    // Background
    lv_obj_set_style_bg_color(
        screen,
        lv_color_hex(0xF2F2F2),
        0
    );

    // Title
    lv_obj_t *title =
        lv_label_create(screen);

    lv_label_set_text(
        title,
        "Pocket Companion"
    );

    lv_obj_align(
        title,
        LV_ALIGN_TOP_MID,
        0,
        15
    );

    // Character face
    lv_obj_t *face =
        lv_obj_create(screen);

    lv_obj_set_size(
        face,
        150,
        150
    );

    lv_obj_align(
        face,
        LV_ALIGN_CENTER,
        0,
        -20
    );

    lv_obj_set_style_radius(
        face,
        LV_RADIUS_CIRCLE,
        0
    );

    lv_obj_set_style_bg_color(
        face,
        lv_color_hex(0xFFE08A),
        0
    );

    lv_obj_set_style_border_width(
        face,
        0,
        0
    );

    // Eyes
    leftEye = createEye(face, 35, 45);
    rightEye = createEye(face, 90, 45);

    // Message
    messageLabel =
        lv_label_create(screen);

    lv_label_set_text(
        messageLabel,
        "Hello!"
    );

    lv_obj_align(
        messageLabel,
        LV_ALIGN_CENTER,
        0,
        70
    );

    // Status
    statusLabel =
        lv_label_create(screen);

    lv_label_set_text(
        statusLabel,
        "I'm happy :)"
    );

    lv_obj_align(
        statusLabel,
        LV_ALIGN_BOTTOM_MID,
        0,
        -55
    );

    // Button
    lv_obj_t *button =
        lv_btn_create(screen);

    lv_obj_set_size(
        button,
        140,
        45
    );

    lv_obj_align(
        button,
        LV_ALIGN_BOTTOM_MID,
        0,
        -5
    );

    lv_obj_add_event_cb(
        button,
        buttonEvent,
        LV_EVENT_CLICKED,
        nullptr
    );

    lv_obj_t *buttonLabel =
        lv_label_create(button);

    lv_label_set_text(
        buttonLabel,
        "Talk"
    );

    lv_obj_center(buttonLabel);
}