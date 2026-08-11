#include "UI.h"

#include <lvgl.h>

// ============================================================
// UI Objects
// ============================================================

static lv_obj_t *statusLabel;
static lv_obj_t *messageLabel;

static lv_obj_t *face;
static lv_obj_t *leftEye;
static lv_obj_t *rightEye;

// ============================================================
// Character State
// ============================================================

static UI::Expression currentExpression =
    UI::Expression::HAPPY;

static bool eyesClosed = false;
static int faceBaseY = 0;


// ============================================================
// Eye Control
// ============================================================

static void setEyesOpen()
{
    lv_obj_set_size(leftEye, 24, 24);
    lv_obj_set_size(rightEye, 24, 24);

    lv_obj_set_pos(leftEye, 35, 45);
    lv_obj_set_pos(rightEye, 90, 45);

    eyesClosed = false;
}

static void setEyesClosed()
{
    lv_obj_set_size(leftEye, 24, 4);
    lv_obj_set_size(rightEye, 24, 4);

    lv_obj_set_pos(leftEye, 35, 55);
    lv_obj_set_pos(rightEye, 90, 55);

    eyesClosed = true;
}

void UI::setExpression(
    UI::Expression expression
)
{
    currentExpression = expression;

    switch (expression)
    {
        case UI::Expression::HAPPY:

            lv_obj_set_style_bg_color(
                face,
                lv_color_hex(0xFFE08A),
                0
            );

            lv_label_set_text(
                statusLabel,
                "I'm happy :)"
            );

            break;


        case UI::Expression::WORRIED:

            lv_obj_set_style_bg_color(
                face,
                lv_color_hex(0xB8D8FF),
                0
            );

            lv_label_set_text(
                statusLabel,
                "What's going on?"
            );

            break;


        case UI::Expression::SAD:

            lv_obj_set_style_bg_color(
                face,
                lv_color_hex(0xAFC5E8),
                0
            );

            lv_label_set_text(
                statusLabel,
                "I'm sad..."
            );

            break;


        case UI::Expression::SURPRISED:

            lv_obj_set_style_bg_color(
                face,
                lv_color_hex(0xFFD1A9),
                0
            );

            lv_label_set_text(
                statusLabel,
                "Wow!"
            );

            break;
    }

    // 표정이 변경될 때 눈은 열린 상태에서 시작
    setEyesOpen();
}

// ============================================================
// Blink Animation
// ============================================================

static void blinkOpenCallback(lv_timer_t *timer)
{
    setEyesOpen();
}

static void blinkCallback(lv_timer_t *timer)
{
    if (!eyesClosed)
    {
        setEyesClosed();

        // 150ms 후 눈을 다시 뜬다.
        lv_timer_t *openTimer =
            lv_timer_create(
                blinkOpenCallback,
                150,
                nullptr
            );

        lv_timer_set_repeat_count(openTimer, 1);
    }
}


// ============================================================
// Character Idle Animation
// ============================================================

static void idleAnimationCallback(
    lv_timer_t *timer
)
{
    static int offset = 0;
    static int direction = 1;

    offset += direction;

    if (offset >= 3)
        direction = -1;

    if (offset <= -3)
        direction = 1;

    lv_obj_set_y(
        face,
        faceBaseY + offset
    );
}


// ============================================================
// Talk Button
// ============================================================

static void buttonEvent(lv_event_t *event)
{
    static int state = 0;

    state++;

    if (state > 3)
        state = 0;

    switch (state)
    {
        case 0:

            UI::setExpression(
                UI::Expression::HAPPY
            );

            lv_label_set_text(
                messageLabel,
                "Hello!"
            );

            break;


        case 1:

            UI::setExpression(
                UI::Expression::WORRIED
            );

            lv_label_set_text(
                messageLabel,
                "Hmm..."
            );

            break;


        case 2:

            UI::setExpression(
                UI::Expression::SAD
            );

            lv_label_set_text(
                messageLabel,
                "Oh..."
            );

            break;


        case 3:

            UI::setExpression(
                UI::Expression::SURPRISED
            );

            lv_label_set_text(
                messageLabel,
                "Wow!"
            );

            break;
    }
}


// ============================================================
// Create Eye
// ============================================================

static lv_obj_t *createEye(
    lv_obj_t *parent,
    int x,
    int y
)
{
    lv_obj_t *eye =
        lv_obj_create(parent);

    lv_obj_set_size(
        eye,
        24,
        24
    );

    lv_obj_set_pos(
        eye,
        x,
        y
    );

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
    // Character Face
    // --------------------------------------------------------

    face =
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
        -35
    );

    faceBaseY = lv_obj_get_y(face);

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


    // --------------------------------------------------------
    // Eyes
    // --------------------------------------------------------

    leftEye =
        createEye(
            face,
            35,
            45
        );

    rightEye =
        createEye(
            face,
            90,
            45
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
        75
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
        LV_ALIGN_BOTTOM_MID,
        0,
        85
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


    // --------------------------------------------------------
    // Automatic Blink
    // --------------------------------------------------------

    lv_timer_create(
        blinkCallback,
        3000,
        nullptr
    );


    // --------------------------------------------------------
    // Idle Animation
    // --------------------------------------------------------

    /*
     * 현재는 얼굴이 아주 조금씩 위아래로 움직이도록
     * 테스트한다.
     *
     * 이후 실제 캐릭터 이미지가 들어오면
     * 이 부분을 캐릭터 애니메이션 시스템으로 교체한다.
     */

    lv_timer_create(
        idleAnimationCallback,
        200,
        nullptr
    );

    UI::setExpression(
    UI::Expression::HAPPY
);
}