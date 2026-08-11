#include "Storage.h"

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// ============================================================
// Waveshare ESP32-S3 Touch LCD
// SD Card SPI Pins
// ============================================================

static constexpr int SD_SCK  = 39;
static constexpr int SD_MISO = 40;
static constexpr int SD_MOSI = 38;
static constexpr int SD_CS   = 41;


// ============================================================
// Initialize SD Card
// ============================================================

bool Storage::begin()
{
    Serial.println();
    Serial.println("================================");
    Serial.println("Initializing SD Card...");
    Serial.println("================================");

    SPI.begin(
        SD_SCK,
        SD_MISO,
        SD_MOSI,
        SD_CS
    );

    if (!SD.begin(SD_CS))
    {
        Serial.println("SD Card Mount Failed");
        return false;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return false;
    }

    Serial.print("SD Card Type: ");

    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize =
        SD.cardSize() / (1024 * 1024);

    uint64_t totalSpace =
        SD.totalBytes() / (1024 * 1024);

    uint64_t usedSpace =
        SD.usedBytes() / (1024 * 1024);

    Serial.printf(
        "SD Card Size : %llu MB\n",
        cardSize
    );

    Serial.printf(
        "Total Space  : %llu MB\n",
        totalSpace
    );

    Serial.printf(
        "Used Space   : %llu MB\n",
        usedSpace
    );

    Serial.println("SD Card initialized successfully.");

    return true;
}


// ============================================================
// Check File / Directory
// ============================================================

bool Storage::exists(const char *path)
{
    return SD.exists(path);
}


// ============================================================
// List Root Directory
// ============================================================

void Storage::listRoot()
{
    Serial.println();
    Serial.println("========== SD Card ==========");

    File root = SD.open("/");

    if (!root)
    {
        Serial.println("Failed to open root directory.");
        return;
    }

    if (!root.isDirectory())
    {
        Serial.println("Root is not a directory.");
        root.close();
        return;
    }

    File file = root.openNextFile();

    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("[DIR]  ");
        }
        else
        {
            Serial.print("[FILE] ");
        }

        Serial.print(file.name());

        if (!file.isDirectory())
        {
            Serial.print("  ");
            Serial.print(file.size());
            Serial.print(" bytes");
        }

        Serial.println();

        file = root.openNextFile();
    }

    root.close();

    Serial.println("=============================");
}