#include "AIClient.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


static String serverUrl;


void AIClient::begin(
    const char *url
)
{
    serverUrl = url;

    Serial.print(
        "AI Server: "
    );

    Serial.println(
        serverUrl
    );
}


String AIClient::chat(
    const String &message
)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(
            "Wi-Fi not connected."
        );

        return "";
    }


    HTTPClient http;

    http.begin(
        serverUrl + "/chat"
    );

    http.addHeader(
        "Content-Type",
        "application/json"
    );


    JsonDocument request;

    request["message"] =
        message;


    String body;

    serializeJson(
        request,
        body
    );


    int httpCode =
        http.POST(body);


    if (httpCode <= 0)
    {
        Serial.print(
            "HTTP error: "
        );

        Serial.println(
            http.errorToString(
                httpCode
            )
        );

        http.end();

        return "";
    }


    if (httpCode != 200)
    {
        Serial.print(
            "Server returned: "
        );

        Serial.println(
            httpCode
        );

        http.end();

        return "";
    }


    String response =
        http.getString();

    http.end();


    JsonDocument json;

    DeserializationError error =
        deserializeJson(
            json,
            response
        );


    if (error)
    {
        Serial.print(
            "JSON parse failed: "
        );

        Serial.println(
            error.c_str()
        );

        return "";
    }


    return json["text"] |
           "";
}