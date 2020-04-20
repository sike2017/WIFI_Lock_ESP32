#pragma once
#include <time.h>
#include <stdint.h>
extern const char* ntpServer;
const long  gmtOffset_sec = 8 * 60 * 60;
const int   daylightOffset_sec = 0;

void updateLocalTime();
int64_t getLocalTimestamp();