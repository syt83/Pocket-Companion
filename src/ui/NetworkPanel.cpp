#include "NetworkPanel.h"

#include <Arduino.h>
#include <lvgl.h>

#include "../network/WiFiManager.h"


namespace
{
    // ========================================================
    // Main Network Panel
    // ========================================================

    lv_obj_t *panel = nullptr;

    lv_obj_t *wifiStatusLabel = nullptr;
    lv_obj_t *ssidLabel = nullptr;
    lv_obj_t *ipLabel = nullptr;

    lv_obj_t *scanButton = nullptr;
    lv_obj_t *wifiButton = nullptr;
    lv_obj_t *wifiButtonLabel = nullptr;

    // Scrollable Wi-Fi list
    lv_obj_t *networkList = nullptr;


    // ========================================================
    // Password Dialog
    // ========================================================

    lv_obj_t *passwordPanel = nullptr;
    lv_obj_t *passwordTitle = nullptr;
    lv_obj_t *passwordInput = nullptr;
    lv_obj_t *keyboard = nullptr;

    lv_obj_t *connectButton = nullptr;
    lv_obj_t *cancelButton = nullptr;


    // ========================================================
    // Selected Network
    // ========================================================

    String selectedSSID;


    // ========================================================
    // Constants
    // ========================================================

    constexpr int MAX_SCAN_RESULTS = 20;


    // ========================================================
    // Forward Declarations
    // ========================================================

    static void updateInfo();

    static void showPasswordDialog(
        const String &ssid
    );

    static void hidePasswordDialog();

    static void networkSelectedEvent(
        lv_event_t *event
    );

    static void networkListDeleteEvent(
        lv_event_t *event
    );


    // ========================================================
    // Update Current Wi-Fi Information
    // ========================================================

    static void updateInfo()
    {
        if (!panel)
            return;


        if (WiFiManager::connected())
        {
            lv_label_set_text(
                wifiStatusLabel,
                "Wi-Fi: ON"
            );


            String ssidText =
                "Connected: " +
                WiFiManager::currentSSID();


            lv_label_set_text(
                ssidLabel,
                ssidText.c_str()
            );


            String ipText =
                "IP: " +
                WiFiManager::ipAddress();


            lv_label_set_text(
                ipLabel,
                ipText.c_str()
            );


            lv_label_set_text(
                wifiButtonLabel,
                "Turn OFF"
            );
        }
        else
        {
            lv_label_set_text(
                wifiStatusLabel,
                "Wi-Fi: OFF"
            );


            lv_label_set_text(
                ssidLabel,
                "Connected: Not connected"
            );


            lv_label_set_text(
                ipLabel,
                "IP: 0.0.0.0"
            );


            lv_label_set_text(
                wifiButtonLabel,
                "Turn ON"
            );
        }
    }


    // ========================================================
    // Wi-Fi ON / OFF
    // ========================================================

    static void wifiButtonEvent(
        lv_event_t *event
    )
    {
        if (WiFiManager::connected())
        {
            WiFiManager::turnOff();
        }
        else
        {
            WiFiManager::turnOn();
        }

        updateInfo();
    }


    // ========================================================
    // Network List Cleanup
    // ========================================================

    static void networkListDeleteEvent(
        lv_event_t *event
    )
    {
        lv_obj_t *obj =
            lv_event_get_target(event);

        void *userData =
            lv_obj_get_user_data(obj);

        if (userData)
        {
            String *ssid =
                static_cast<String *>(userData);

            delete ssid;

            lv_obj_set_user_data(
                obj,
                nullptr
            );
        }
    }


    // ========================================================
    // Select Network
    // ========================================================

    static void networkSelectedEvent(
        lv_event_t *event
    )
    {
        lv_obj_t *target =
            lv_event_get_target(event);

        if (!target)
            return;


        void *userData =
            lv_obj_get_user_data(target);

        if (!userData)
            return;


        String *ssid =
            static_cast<String *>(userData);


        if (ssid->isEmpty())
            return;


        selectedSSID =
            *ssid;


        Serial.print(
            "[NetworkPanel] Selected SSID: "
        );

        Serial.println(
            selectedSSID
        );


        showPasswordDialog(
            selectedSSID
        );
    }


    // ========================================================
    // Scan Wi-Fi Networks
    // ========================================================

    static void scanButtonEvent(
        lv_event_t *event
    )
    {
        if (!networkList)
            return;


        // Remove all previous result buttons.
        // Their DELETE callback frees the String user data.
        lv_obj_clean(
            networkList
        );


        lv_label_set_text(
            wifiStatusLabel,
            "Scanning..."
        );


        String networks[
            MAX_SCAN_RESULTS
        ];


        int count =
            WiFiManager::scan(
                networks,
                MAX_SCAN_RESULTS
            );


        if (count <= 0)
        {
            lv_obj_t *label =
                lv_label_create(
                    networkList
                );


            lv_label_set_text(
                label,
                "No networks found."
            );


            updateInfo();

            return;
        }


        for (int i = 0; i < count; i++)
        {
            if (networks[i].isEmpty())
                continue;


            // ------------------------------------------------
            // Create one button per Wi-Fi network
            // ------------------------------------------------

            lv_obj_t *networkButton =
                lv_btn_create(
                    networkList
                );


            lv_obj_set_width(
                networkButton,
                200
            );


            lv_obj_set_height(
                networkButton,
                34
            );


            // ------------------------------------------------
            // Store SSID in user data
            // ------------------------------------------------

            String *ssid =
                new String(
                    networks[i]
                );


            lv_obj_set_user_data(
                networkButton,
                ssid
            );


            lv_obj_add_event_cb(
                networkButton,
                networkSelectedEvent,
                LV_EVENT_CLICKED,
                nullptr
            );


            lv_obj_add_event_cb(
                networkButton,
                networkListDeleteEvent,
                LV_EVENT_DELETE,
                nullptr
            );


            // ------------------------------------------------
            // Label
            // ------------------------------------------------

            lv_obj_t *label =
                lv_label_create(
                    networkButton
                );


            lv_obj_set_width(
                label,
                180
            );


            lv_label_set_long_mode(
                label,
                LV_LABEL_LONG_DOT
            );


            lv_label_set_text(
                label,
                networks[i].c_str()
            );


            lv_obj_center(
                label
            );
        }


        updateInfo();
    }


    // ========================================================
    // Connect Button
    // ========================================================

    static void connectButtonEvent(
        lv_event_t *event
    )
    {
        if (selectedSSID.isEmpty())
            return;


        const char *password =
            lv_textarea_get_text(
                passwordInput
            );


        lv_label_set_text(
            passwordTitle,
            "Connecting..."
        );


        Serial.println();
        Serial.println(
            "[NetworkPanel] Connecting..."
        );

        Serial.print(
            "SSID: "
        );

        Serial.println(
            selectedSSID
        );


        bool success =
            WiFiManager::connect(
                selectedSSID,
                String(password)
            );


        if (success)
        {
            Serial.println(
                "[NetworkPanel] Connected."
            );


            hidePasswordDialog();


            updateInfo();
        }
        else
        {
            Serial.println(
                "[NetworkPanel] Connection failed."
            );


            lv_label_set_text(
                passwordTitle,
                "Connection failed"
            );
        }
    }


    // ========================================================
    // Cancel Button
    // ========================================================

    static void cancelButtonEvent(
        lv_event_t *event
    )
    {
        hidePasswordDialog();
    }


    // ========================================================
    // Create Password Dialog
    // ========================================================

    static void createPasswordDialog(
        lv_obj_t *screen
    )
    {
        passwordPanel =
            lv_obj_create(
                screen
            );


        lv_obj_set_size(
            passwordPanel,
            240,
            320
        );


        lv_obj_set_pos(
            passwordPanel,
            0,
            0
        );


        lv_obj_set_style_radius(
            passwordPanel,
            0,
            0
        );


        lv_obj_set_style_pad_all(
            passwordPanel,
            8,
            0
        );


        // ----------------------------------------------------
        // Title
        // ----------------------------------------------------

        passwordTitle =
            lv_label_create(
                passwordPanel
            );


        lv_obj_set_width(
            passwordTitle,
            220
        );


        lv_label_set_long_mode(
            passwordTitle,
            LV_LABEL_LONG_DOT
        );


        lv_label_set_text(
            passwordTitle,
            "Connect"
        );


        lv_obj_align(
            passwordTitle,
            LV_ALIGN_TOP_LEFT,
            0,
            0
        );


        // ----------------------------------------------------
        // Password Input
        // ----------------------------------------------------

        passwordInput =
            lv_textarea_create(
                passwordPanel
            );


        lv_obj_set_size(
            passwordInput,
            220,
            42
        );


        lv_obj_align(
            passwordInput,
            LV_ALIGN_TOP_LEFT,
            0,
            32
        );


        lv_textarea_set_one_line(
            passwordInput,
            true
        );


        lv_textarea_set_password_mode(
            passwordInput,
            true
        );


        lv_textarea_set_placeholder_text(
            passwordInput,
            "Password"
        );


        // ----------------------------------------------------
        // Cancel Button
        // ----------------------------------------------------

        cancelButton =
            lv_btn_create(
                passwordPanel
            );


        lv_obj_set_size(
            cancelButton,
            70,
            32
        );


        lv_obj_align(
            cancelButton,
            LV_ALIGN_TOP_LEFT,
            0,
            80
        );


        lv_obj_add_event_cb(
            cancelButton,
            cancelButtonEvent,
            LV_EVENT_CLICKED,
            nullptr
        );


        lv_obj_t *cancelLabel =
            lv_label_create(
                cancelButton
            );


        lv_label_set_text(
            cancelLabel,
            "Cancel"
        );


        lv_obj_center(
            cancelLabel
        );


        // ----------------------------------------------------
        // Connect Button
        // ----------------------------------------------------

        connectButton =
            lv_btn_create(
                passwordPanel
            );


        lv_obj_set_size(
            connectButton,
            90,
            32
        );


        lv_obj_align(
            connectButton,
            LV_ALIGN_TOP_RIGHT,
            0,
            80
        );


        lv_obj_add_event_cb(
            connectButton,
            connectButtonEvent,
            LV_EVENT_CLICKED,
            nullptr
        );


        lv_obj_t *connectLabel =
            lv_label_create(
                connectButton
            );


        lv_label_set_text(
            connectLabel,
            "Connect"
        );


        lv_obj_center(
            connectLabel
        );


        // ----------------------------------------------------
        // Keyboard
        // ----------------------------------------------------

        keyboard =
            lv_keyboard_create(
                passwordPanel
            );


        lv_obj_set_size(
            keyboard,
            224,
            190
        );


        lv_obj_align(
            keyboard,
            LV_ALIGN_BOTTOM_MID,
            0,
            0
        );


        lv_keyboard_set_textarea(
            keyboard,
            passwordInput
        );


        // Hide until an SSID is selected.
        lv_obj_add_flag(
            passwordPanel,
            LV_OBJ_FLAG_HIDDEN
        );
    }


    // ========================================================
    // Show Password Dialog
    // ========================================================

    static void showPasswordDialog(
        const String &ssid
    )
    {
        if (!passwordPanel)
            return;


        selectedSSID =
            ssid;


        String title =
            "Connect: " +
            ssid;


        lv_label_set_text(
            passwordTitle,
            title.c_str()
        );


        lv_textarea_set_text(
            passwordInput,
            ""
        );


        lv_textarea_set_placeholder_text(
            passwordInput,
            "Password"
        );


        lv_obj_clear_flag(
            passwordPanel,
            LV_OBJ_FLAG_HIDDEN
        );


        lv_obj_move_foreground(
            passwordPanel
        );


        // Put keyboard focus on the password field.
        lv_keyboard_set_textarea(
            keyboard,
            passwordInput
        );


        lv_textarea_set_cursor_pos(
            passwordInput,
            LV_TEXTAREA_CURSOR_LAST
        );
    }


    // ========================================================
    // Hide Password Dialog
    // ========================================================

    static void hidePasswordDialog()
    {
        if (!passwordPanel)
            return;


        lv_obj_add_flag(
            passwordPanel,
            LV_OBJ_FLAG_HIDDEN
        );


        // Clear password input.
        lv_textarea_set_text(
            passwordInput,
            ""
        );


        selectedSSID = "";
    }
}


// ============================================================
// Create Network Panel
// ============================================================

void NetworkPanel::create()
{
    if (panel)
        return;


    lv_obj_t *screen =
        lv_scr_act();


    // --------------------------------------------------------
    // Main Panel
    // --------------------------------------------------------

    panel =
        lv_obj_create(
            screen
        );


    lv_obj_set_size(
        panel,
        240,
        250
    );


    lv_obj_set_pos(
        panel,
        0,
        -250
    );


    lv_obj_set_style_radius(
        panel,
        0,
        0
    );


    lv_obj_set_style_pad_all(
        panel,
        8,
        0
    );


    // --------------------------------------------------------
    // Title
    // --------------------------------------------------------

    lv_obj_t *title =
        lv_label_create(
            panel
        );


    lv_label_set_text(
        title,
        "Network"
    );


    lv_obj_align(
        title,
        LV_ALIGN_TOP_LEFT,
        0,
        0
    );


    // --------------------------------------------------------
    // Wi-Fi Status
    // --------------------------------------------------------

    wifiStatusLabel =
        lv_label_create(
            panel
        );


    lv_obj_align(
        wifiStatusLabel,
        LV_ALIGN_TOP_LEFT,
        0,
        28
    );


    // --------------------------------------------------------
    // Current SSID
    // --------------------------------------------------------

    ssidLabel =
        lv_label_create(
            panel
        );


    lv_obj_set_width(
        ssidLabel,
        210
    );


    lv_label_set_long_mode(
        ssidLabel,
        LV_LABEL_LONG_DOT
    );


    lv_obj_align(
        ssidLabel,
        LV_ALIGN_TOP_LEFT,
        0,
        50
    );


    // --------------------------------------------------------
    // IP
    // --------------------------------------------------------

    ipLabel =
        lv_label_create(
            panel
        );


    lv_obj_align(
        ipLabel,
        LV_ALIGN_TOP_LEFT,
        0,
        70
    );


    // --------------------------------------------------------
    // Wi-Fi ON / OFF Button
    // --------------------------------------------------------

    wifiButton =
        lv_btn_create(
            panel
        );


    lv_obj_set_size(
        wifiButton,
        90,
        32
    );


    lv_obj_align(
        wifiButton,
        LV_ALIGN_TOP_RIGHT,
        0,
        20
    );


    lv_obj_add_event_cb(
        wifiButton,
        wifiButtonEvent,
        LV_EVENT_CLICKED,
        nullptr
    );


    wifiButtonLabel =
        lv_label_create(
            wifiButton
        );


    lv_obj_center(
        wifiButtonLabel
    );


    // --------------------------------------------------------
    // Scan Button
    // --------------------------------------------------------

    scanButton =
        lv_btn_create(
            panel
        );


    lv_obj_set_size(
        scanButton,
        120,
        32
    );


    lv_obj_align(
        scanButton,
        LV_ALIGN_TOP_LEFT,
        0,
        100
    );


    lv_obj_add_event_cb(
        scanButton,
        scanButtonEvent,
        LV_EVENT_CLICKED,
        nullptr
    );


    lv_obj_t *scanLabel =
        lv_label_create(
            scanButton
        );


    lv_label_set_text(
        scanLabel,
        "Scan Wi-Fi"
    );


    lv_obj_center(
        scanLabel
    );


    // --------------------------------------------------------
    // Scrollable Wi-Fi List
    // --------------------------------------------------------

    networkList =
        lv_obj_create(
            panel
        );


    lv_obj_set_size(
        networkList,
        220,
        105
    );


    lv_obj_align(
        networkList,
        LV_ALIGN_TOP_LEFT,
        0,
        135
    );


    lv_obj_set_style_pad_all(
        networkList,
        2,
        0
    );


    lv_obj_set_style_pad_row(
        networkList,
        2,
        0
    );


    // Vertical scrolling
    lv_obj_set_scroll_dir(
        networkList,
        LV_DIR_VER
    );


    lv_obj_set_scrollbar_mode(
        networkList,
        LV_SCROLLBAR_MODE_AUTO
    );


    // Flex layout:
    // buttons are automatically stacked vertically.
    lv_obj_set_flex_flow(
        networkList,
        LV_FLEX_FLOW_COLUMN
    );


    lv_obj_set_flex_align(
        networkList,
        LV_FLEX_ALIGN_START,
        LV_FLEX_ALIGN_START,
        LV_FLEX_ALIGN_START
    );


    // --------------------------------------------------------
    // Password Dialog
    // --------------------------------------------------------

    createPasswordDialog(
        screen
    );


    updateInfo();
}


// ============================================================
// Show Panel
// ============================================================

void NetworkPanel::show()
{
    if (!panel)
        create();


    updateInfo();


    // Put panel above the main UI.
    lv_obj_move_foreground(
        panel
    );


    // Show panel.
    lv_obj_set_y(
        panel,
        0
    );
}


// ============================================================
// Hide Panel
// ============================================================

void NetworkPanel::hide()
{
    if (!panel)
        return;


    if (passwordPanel)
    {
        hidePasswordDialog();
    }


    lv_obj_set_y(
        panel,
        -250
    );
}


// ============================================================
// Visibility
// ============================================================

bool NetworkPanel::visible()
{
    if (!panel)
        return false;


    return (
        lv_obj_get_y(panel) >= 0
    );
}