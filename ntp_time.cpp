#include "ntp_time.h"
#include <WiFi.h>

const char* ntpServer = "pool.ntp.org";

void updateLocalTime() {
    Serial.println("updateLocalTime");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

int64_t getLocalTimestamp() {
    return time(NULL);
}
