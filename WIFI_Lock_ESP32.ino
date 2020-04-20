#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "controller.h"
#include "general_err.h"
#include "descriptor.h"
#include "motor.h"
#include "utility.h"
#include "ntp_time.h"
#include "wifi_settings.h"
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
    request = (DataPak *)malloc(sizeof(DataPak));
    response = (DataPak *)malloc(sizeof(DataPak));
    initDataPak(request);
    initDataPak(response);
    wifi_mode_t mode;
    if ( getWifiMode(&mode) == GENERAL_ERR_NVS_NOT_FOUND ) {
        mode = WIFI_MODE_AP;
        setWifiMode(mode);
    }
    WifiSetting wf;
    IPAddress myIP;
    switch (mode) {
    case WIFI_MODE_AP:
        strcpy(wf.ssid, SOFT_AP_SSID);
        strcpy(wf.password, SOFT_AP_PASSWORD);
        startSoftAP();
        myIP = WiFi.softAPIP();
        break;
    case WIFI_MODE_STA:
    {
        GeneralErr result;
        result = getWifiStaSetting(&wf);
        if (result == GENERAL_ERR_NVS_NOT_FOUND) {
            Serial.println("device was set to STA mode, but no wifi setting was stored in NVS");
            setWifiMode(WIFI_MODE_AP);
            ESP.restart(); // force abort here
        }
        GENERAL_ERROR_CHECK( result );
        result = startSTA(&wf);
        if (result == GENERAL_CONNECTION_TIME_OUT) {
            Serial.print("connection time out\n");
            result = getPrevWifiStaSetting(&wf);
            if (result == GENERAL_ERR_NVS_NOT_FOUND) {
                setWifiMode(WIFI_MODE_AP);
                ESP.restart();
            }
            Serial.println("connect to the previous wifi");
            result = startSTA(&wf);
            if (result == GENERAL_CONNECTION_TIME_OUT) {
                setWifiMode(WIFI_MODE_AP);
                ESP.restart();
            }
        }
        myIP = WiFi.localIP();
        break;
    }
    default:
        break;
    }
    Serial.print("SSID: ");
    Serial.println(wf.ssid);
    Serial.print("IP: ");
    Serial.println(myIP);
    updateLocalTime();
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
    // Serial.printf("timestamp: %ld\n", getTimestamp());
    delay(2000);
}
