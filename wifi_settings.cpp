#include "wifi_settings.h"
#include <nvs.h>
#include <WiFi.h>

const char *SOFT_AP_SSID = "ESP32_WiFi_AP";
const char *SOFT_AP_PASSWORD = "1234567890";
const char *STA_NVS_KEY = "sta";
const char *WIFI_MODE_NVS_KEY = "mode";
const char *PREV_STA_NVS_KEY = "prev_sta";

GeneralErr setWifiStaSetting(WifiSetting *src) {
    nvs_handle handle;
    ESP_ERROR_CHECK( nvs_open(NVS_WIFI_PART, NVS_READWRITE, &handle) );
    WifiSetting prevSetting;
    size_t len = sizeof(prevSetting);
    esp_err_t ret_val = nvs_get_blob(handle, STA_NVS_KEY, &prevSetting, &len);
    if (ret_val == ESP_OK) {
        setPrevWifiStaSetting(&prevSetting);
    }
    ESP_ERROR_CHECK( nvs_set_blob(handle, STA_NVS_KEY, src, sizeof(WifiSetting)) );
    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);
    return GENERAL_OK;
}

GeneralErr getWifiStaSetting(WifiSetting *dst) {
    nvs_handle handle;
    ESP_ERROR_CHECK( nvs_open(NVS_WIFI_PART, NVS_READONLY, &handle) );
    size_t len = sizeof(WifiSetting);

    esp_err_t ret_val = nvs_get_blob(handle, STA_NVS_KEY, dst, &len);
    if (ret_val == ESP_ERR_NVS_NOT_FOUND) {
        return GENERAL_ERR_NVS_NOT_FOUND;
    }
    ESP_ERROR_CHECK( nvs_commit(handle) );

    nvs_close(handle);
    return GENERAL_OK;
}

GeneralErr setWifiMode(wifi_mode_t mode) {
    nvs_handle handle;
    ESP_ERROR_CHECK( nvs_open(NVS_WIFI_PART, NVS_READWRITE, &handle) );
    ESP_ERROR_CHECK( nvs_set_blob(handle, WIFI_MODE_NVS_KEY, &mode, sizeof(mode)) );
    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);
    return GENERAL_OK;
}

GeneralErr getWifiMode(wifi_mode_t *dst) {
    nvs_handle handle;
    esp_err_t ret_val;
    ret_val = nvs_open(NVS_WIFI_PART, NVS_READONLY, &handle);
    if (ret_val == ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK( nvs_open(NVS_WIFI_PART, NVS_READWRITE, &handle) );
    }
    size_t len = sizeof(*dst);
    ret_val = nvs_get_blob(handle, WIFI_MODE_NVS_KEY, dst, &len);
    
    if (ret_val == ESP_ERR_NVS_NOT_FOUND) {
        return GENERAL_ERR_NVS_NOT_FOUND;
    }
    ESP_ERROR_CHECK( nvs_commit(handle) );

    nvs_close(handle);
    return GENERAL_OK;
}

GeneralErr startSTA(WifiSetting *src) {
    Serial.println("configuring WiFi STA...");
    WiFi.begin(src->ssid, src->password);
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        if (counter > WIFI_TIME_OUT) {
            return GENERAL_CONNECTION_TIME_OUT;
        }
        counter += 500;
        delay(500);
        Serial.print(".");
    }
    return GENERAL_OK;
}

GeneralErr startSoftAP() {
    Serial.println("configuring WiFi AP...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(SOFT_AP_SSID, SOFT_AP_PASSWORD);

    return GENERAL_OK;
}

GeneralErr setPrevWifiStaSetting(WifiSetting *src) {
    nvs_handle handle;
    ESP_ERROR_CHECK( nvs_open(NVS_WIFI_PART, NVS_READWRITE, &handle) );
    ESP_ERROR_CHECK( nvs_set_blob(handle, PREV_STA_NVS_KEY, src, sizeof(WifiSetting)) );
    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);
    return GENERAL_OK;
}

GeneralErr getPrevWifiStaSetting(WifiSetting *dst) {
    nvs_handle handle;
    ESP_ERROR_CHECK( nvs_open(NVS_WIFI_PART, NVS_READWRITE, &handle) );
    size_t len = sizeof(WifiSetting);

    esp_err_t ret_val = nvs_get_blob(handle, PREV_STA_NVS_KEY, dst, &len);
    if (ret_val == ESP_ERR_NVS_NOT_FOUND) {
        return GENERAL_ERR_NVS_NOT_FOUND;
    }
    ESP_ERROR_CHECK( nvs_commit(handle) );

    nvs_close(handle);
    return GENERAL_OK;
}
