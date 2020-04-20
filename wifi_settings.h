#include "storage.h"
#include "general_err.h"
#include <WiFi.h>
#include "descriptor.h"
extern const char *SOFT_AP_SSID;
extern const char *SOFT_AP_PASSWORD;
extern const char *STA_NVS_KEY;
extern const char *PREV_STA_NVS_KEY;
typedef struct {
    char ssid[32];
    char password[64];
} WifiSetting;

GeneralErr setWifiStaSetting(WifiSetting *src);
GeneralErr getWifiStaSetting(WifiSetting *dst);
GeneralErr setWifiMode(wifi_mode_t mode);
GeneralErr getWifiMode(wifi_mode_t *dst);
GeneralErr startSTA(WifiSetting *src);
GeneralErr startSoftAP();
GeneralErr setPrevWifiStaSetting(WifiSetting *src);
GeneralErr getPrevWifiStaSetting(WifiSetting *dst);