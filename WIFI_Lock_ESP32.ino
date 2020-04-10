#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "controller.h"
#include "general_err.h"
#include "descriptor.h"
#include "motor.h"
#include "utility.h"
const char *ssid = "ESP32_WiFi_AP";
const char *password = "1234567890";
int wifi_port = 2700;
WiFiServer server(wifi_port);

void initDataPak(DataPak *p) {
    memset(p, 0, sizeof(DataPak));
    p->reservedByte = p->data;
    p->loading = &(p->data[RESERVED_BYTES_SIZE]);
}

DataPak *request;
DataPak *response;
GeneralErr result;

void setup() {
    for (int i = 0; i < PIN_NUMBER; i++) {
        pinMode(PIN_ARRAY[i], OUTPUT);
    }
    Serial.begin(115200);
    Serial.println("configuring WiFi AP...");
    request = (DataPak *)malloc(sizeof(DataPak));
    response = (DataPak *)malloc(sizeof(DataPak));
    initDataPak(request);
    initDataPak(response);
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP SSID: ");
    Serial.println(ssid);
    Serial.print("AP IP: ");
    Serial.println(myIP);
    server.begin();

    result = GENERAL_OK;
    Serial.println("Server started");
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New Client");
        while (client.connected()) {
            if (client.available()) {
                Serial.println("recv data: ");
                Serial.println(request->data);
                client.readBytes((uint8_t *)(request->data), PACKAGE_SIZE);
                Serial.println("after read");
                result = controllerRun(request, response);
                Serial.println("send data: ");
                Serial.println(response->data);
                client.write(response->data, PACKAGE_SIZE);
                Serial.println("after write");
            }
        }
        if (result == GENERAL_CONNECTION_DICONNECT) {
            client.stop();
            Serial.println("Client Disconnected");
            result = GENERAL_OK;
        }
    }
    delay(2000);
}
