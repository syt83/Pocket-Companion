#include "LVGL_FS.h"

#include <Arduino.h>
#include <FS.h>
#include <SD.h>

#include <lvgl.h>


// ============================================================
// LVGL -> SD Card File System
// ============================================================

static void *fs_open(
    lv_fs_drv_t *drv,
    const char *path,
    lv_fs_mode_t mode
)
{
    Serial.println();
    Serial.println("[LVGL FS] OPEN");

    Serial.print("Path received: ");
    Serial.println(path);

    Serial.print("Mode: ");

    if (mode == LV_FS_MODE_WR)
        Serial.println("WRITE");
    else
        Serial.println("READ");


    File *file = new File();


    if (mode == LV_FS_MODE_WR)
    {
        *file = SD.open(path, FILE_WRITE);
    }
    else
    {
        *file = SD.open(path, FILE_READ);
    }


    if (!(*file))
    {
        Serial.println("[LVGL FS] FAILED TO OPEN FILE");

        delete file;

        return nullptr;
    }


    Serial.println("[LVGL FS] FILE OPENED");

    Serial.print("File size: ");
    Serial.println(file->size());


    return file;
}


static lv_fs_res_t fs_close(
    lv_fs_drv_t *drv,
    void *file_p
)
{
    File *file = static_cast<File *>(file_p);

    if (file)
    {
        file->close();
        delete file;
    }

    return LV_FS_RES_OK;
}


static lv_fs_res_t fs_read(
    lv_fs_drv_t *drv,
    void *file_p,
    void *buf,
    uint32_t btr,
    uint32_t *br
)
{
    File *file = static_cast<File *>(file_p);

    if (!file)
        return LV_FS_RES_FS_ERR;

    Serial.println();
    Serial.println("[LVGL FS] ===== READ =====");

    Serial.print("Position BEFORE: ");
    Serial.println(file->position());

    Serial.print("Requested: ");
    Serial.println(btr);

    size_t readBytes =
        file->read(
            static_cast<uint8_t *>(buf),
            btr
        );

    *br = readBytes;

    Serial.print("Position AFTER: ");
    Serial.println(file->position());

    Serial.print("Actually read: ");
    Serial.println(readBytes);

    if (readBytes > 0)
    {
        Serial.print("First bytes: ");

        uint8_t *data =
            static_cast<uint8_t *>(buf);

        uint32_t show =
            readBytes > 16 ? 16 : readBytes;

        for (uint32_t i = 0; i < show; i++)
        {
            if (data[i] < 0x10)
                Serial.print("0");

            Serial.print(data[i], HEX);
            Serial.print(" ");
        }

        Serial.println();
    }

    Serial.println("======================");

    return LV_FS_RES_OK;
}

static lv_fs_res_t fs_seek(
    lv_fs_drv_t *drv,
    void *file_p,
    uint32_t pos,
    lv_fs_whence_t whence
)
{
    File *file = static_cast<File *>(file_p);

    if (!file)
        return LV_FS_RES_FS_ERR;

    Serial.println();
    Serial.println("[LVGL FS] ===== SEEK =====");

    Serial.print("Current: ");
    Serial.println(file->position());

    Serial.print("pos: ");
    Serial.println(pos);

    Serial.print("whence: ");
    Serial.println((int)whence);

    uint32_t target;

    switch (whence)
    {
        case LV_FS_SEEK_SET:
            target = pos;
            break;

        case LV_FS_SEEK_CUR:
            target = file->position() + pos;
            break;

        case LV_FS_SEEK_END:
            target = file->size() + pos;
            break;

        default:
            Serial.println("INVALID WHENCE");
            return LV_FS_RES_FS_ERR;
    }

    Serial.print("Target: ");
    Serial.println(target);

    bool result = file->seek(target);

    Serial.print("Seek result: ");
    Serial.println(result ? "SUCCESS" : "FAILED");

    Serial.print("Position AFTER: ");
    Serial.println(file->position());

    Serial.println("======================");

    return result
        ? LV_FS_RES_OK
        : LV_FS_RES_FS_ERR;
}


static lv_fs_res_t fs_tell(
    lv_fs_drv_t *drv,
    void *file_p,
    uint32_t *pos_p
)
{
    File *file = static_cast<File *>(file_p);

    if (!file)
        return LV_FS_RES_FS_ERR;

    *pos_p = file->position();

    return LV_FS_RES_OK;
}


// ============================================================
// Register SD Card to LVGL
// ============================================================

void LVGL_FS::begin()
{
    static lv_fs_drv_t drv;

    lv_fs_drv_init(&drv);

    drv.letter = 'S';

    drv.open_cb  = fs_open;
    drv.close_cb = fs_close;
    drv.read_cb  = fs_read;
    drv.seek_cb  = fs_seek;
    drv.tell_cb = fs_tell;

    lv_fs_drv_register(&drv);

    Serial.println("LVGL SD File System registered.");
}