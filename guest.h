#pragma once
#include "user.h"

typedef struct {
    char *pNvsKey;
    int64_t *pCipherLength;
    unsigned char *pCipherBytes;
    unsigned char plainBytes[LOADING_SIZE];
    int64_t *pEndTimestamp;
    UserId *pUserId;
} GuestFormat;

typedef struct {
    char user_nvs_key[NVS_KEY_MAX_SIZE];
    int64_t endTimestamp;
    UserId user_id;
} Guest;

void initGuestFormat(char *guestBytes);
GeneralErr decrypt(char *src, Guest *dst);
