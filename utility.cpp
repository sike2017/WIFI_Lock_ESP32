#include "utility.h"
#include <Arduino.h>

void createUUID(char *dest, size_t length) {
    // Parameter length must be multiples of 4 bytes.
    uint32_t r = esp_random();
    size_t r_len = sizeof(r);
    size_t len = length;
    uint32_t *cur = (uint32_t *)dest;
    while (len) {
        *cur = r;
        r = esp_random();
        cur++;
        len -= r_len;
    }
}
