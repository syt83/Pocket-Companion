#include "UI.h"

#include <lvgl.h>
#include <Arduino.h>
#include "../character/CharacterRenderer.h"

// ============================================================
// UI Objects
// ============================================================

static lv_obj_t *statusLabel = nullptr;
static lv_obj_t *messageLabel = nullptr;


// ============================================================
// Character Renderer
// ============================================================

static CharacterRenderer character;


// ============================================================
// Character State
// ============================================================

static UI::Expression currentExpression =
    UI::Expression::HAPPY;


// ============================================================
// Character Position
// ============================================================

static constexpr int32_t CHARACTER_Y = 35;


// ============================================================
// Character Position Update
// ============================================================

static void updateCharacterPosition()
{
    if (character.object() == nullptr)
        return;

    int32_t screenWidth =
        lv_obj_get_width(
            lv_scr_act()
        );

    int32_t characterWidth =
        character.width();

    int32_t x =
        (screenWidth - characterWidth) / 2;

    character.setPosition(
        x,
        CHARACTER_Y
    );
}


// ============================================================
// Expression
// ============================================================

void UI::setExpression(
    UI::Expression expression
)
{
    currentExpression = expression;

    const char *imagePath = nullptr;


    switch (expression)
    {
        // ----------------------------------------------------
        // HAPPY
        // ----------------------------------------------------

        case UI::Expression::HAPPY:

            imagePath =
                "/character/happy.rgb565";

            lv_label_set_text(
                messageLabel,
                "Hello!"
            );

            lv_label_set_text(
                statusLabel,
                "I'm happy :)"
            );

            break;


        // ----------------------------------------------------
        // WORRIED
        // ----------------------------------------------------

        case UI::Expression::WORRIED:

            imagePath =
                "/character/worried.rgb565";

            lv_label_set_text(
                messageLabel,
                "Hmm..."
            );

            lv_label_set_text(
                statusLabel,
                "What's going on?"
            );

            break;


        // ----------------------------------------------------
        // SAD
        // ----------------------------------------------------

        case UI::Expression::SAD:

            imagePath =
                "/character/sad.rgb565";

            lv_label_set_text(
                messageLabel,
                "Oh..."
            );

            lv_label_set_text(
                statusLabel,
                "I'm sad..."
            );

            break;


        // ----------------------------------------------------
        // SURPRISED
        // ----------------------------------------------------

        case UI::Expression::SURPRISED:

            imagePath =
                "/character/surprised.rgb565";

            lv_label_set_text(
                messageLabel,
                "Wow!"
            );

            lv_label_set_text(
                statusLabel,
                "That's surprising!"
            );

            break;
    }


    // --------------------------------------------------------
    // Load Character Image
    // --------------------------------------------------------

    if (imagePath != nullptr)
    {
        if (!character.loadImage(imagePath))
        {
            Serial.print(
                "[UI] Failed to load character: "
            );

            Serial.println(imagePath);
        }
        else
        {
            updateCharacterPosition();
        }
    }
}


// ============================================================
// Talk Button
// ============================================================

static void buttonEvent(
    lv_event_t *event
)
{
    static uint8_t state = 0;

    state++;

    if (state > 3)
        state = 0;


    switch (state)
    {
        case 0:

            UI::setExpression(
                UI::Expression::HAPPY
            );

            break;


        case 1:

            UI::setExpression(
                UI::Expression::WORRIED
            );

            break;


        case 2:

            UI::setExpression(
                UI::Expression::SAD
            );

            break;


        case 3:

            UI::setExpression(
                UI::Expression::SURPRISED
            );

            break;
    }
}


// ============================================================
// UI Initialization
// ============================================================

void UI::begin()
{
    lv_obj_t *screen =
        lv_scr_act();


    // --------------------------------------------------------
    // Background
    // --------------------------------------------------------

    lv_obj_set_style_bg_color(
        screen,
        lv_color_hex(0xF2F2F2),
        0
    );


    // --------------------------------------------------------
    // Title
    // --------------------------------------------------------

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


    // --------------------------------------------------------
    // Message
    // --------------------------------------------------------

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
        85
    );


    // --------------------------------------------------------
    // Status
    // --------------------------------------------------------

    statusLabel =
        lv_label_create(screen);

    lv_label_set_text(
        statusLabel,
        "I'm happy :)"
    );

    lv_obj_align(
        statusLabel,
        LV_ALIGN_CENTER,
        0,
        110
    );


    // --------------------------------------------------------
    // Character Renderer
    // --------------------------------------------------------

    if (!character.begin(screen))
    {
        Serial.println(
            "[UI] CharacterRenderer initialization failed."
        );

        return;
    }


    // --------------------------------------------------------
    // Initial Expression
    // --------------------------------------------------------

    UI::setExpression(
        UI::Expression::HAPPY
    );


    // --------------------------------------------------------
    // Talk Button
    // --------------------------------------------------------

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
        -10
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