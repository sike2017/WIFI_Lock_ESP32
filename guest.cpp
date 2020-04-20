#include "guest.h"
#include "mbedtls/aes.h"

void initGuestFormat(char *guestBytes, GuestFormat *guestFormat) {
    guestFormat->pNvsKey = guestBytes;
    guestFormat->pCipherLength = (int64_t *)(guestFormat->pNvsKey + NVS_KEY_MAX_SIZE);
    guestFormat->pCipherBytes = (unsigned char *)(guestFormat->pCipherLength + 1);
    memset(guestFormat->plainBytes, 0, LOADING_SIZE);
    guestFormat->pEndTimestamp = (int64_t *)(&guestFormat->plainBytes);
    guestFormat->pUserId = (UserId *)(guestFormat->pEndTimestamp + 1);
}

GeneralErr decrypt(char *src, Guest *dst) {
    Serial.println("in decrypt");
    unsigned char iv[16];
    memset(iv, 0, 16);
    User correct_user;
    User current_user;
    GuestFormat guestFormat;
    initGuestFormat(src, &guestFormat);
    if (*guestFormat.pCipherLength % USER_ID_SIZE) {
        Serial.println("GENERAL_ERR_AES_INVALID_INPUT_LENGTH");
        return GENERAL_ERR_AES_INVALID_INPUT_LENGTH;
    }
    if ( !CHECK_OK( getUserByKey(guestFormat.pNvsKey, &correct_user) ) ) {
        Serial.println("GENERAL_ERR_NVS_INVALID_KEY");
        return GENERAL_ERR_NVS_INVALID_KEY;
    }
    Serial.printf("cipherLength: %ld\n", *guestFormat.pCipherLength);
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    int result;
    result = mbedtls_aes_setkey_dec(&aes, (unsigned char *)(&correct_user.usf.id), 128);
    Serial.print("correct_user.usf.id: ");
    for (int j = 0; j < USER_ID_SIZE; j++) {
        Serial.printf("%02x", ((char *)(&correct_user.usf.id))[j]);
    }
    Serial.println();
    Serial.printf("result1: %d\n", result);
    result = mbedtls_aes_crypt_cbc( &aes,
                           MBEDTLS_AES_DECRYPT,
                           *guestFormat.pCipherLength,
                           iv,
                           guestFormat.pCipherBytes,
                           (unsigned char *)(&guestFormat.plainBytes)
                         );
    Serial.print("guestFormat.pCipherBytes: ");
    for (int k = 0; k < *guestFormat.pCipherLength; k ++) {
        Serial.printf("%02x", ((char *)(guestFormat.pCipherBytes))[k]);
    }
    Serial.println();
    Serial.printf("result2: %d\n", result);
    mbedtls_aes_free(&aes);
    dst->endTimestamp = *guestFormat.pEndTimestamp;
    Serial.print("endTimestamp: ");
    Serial.printf("%ld\n", dst->endTimestamp);
    memcpy(&(dst->user_id), guestFormat.pUserId, USER_ID_SIZE);
    memcpy(&(dst->user_nvs_key), guestFormat.pNvsKey, NVS_KEY_MAX_SIZE);
    Serial.print("dst->user_id: ");
    for (int i = 0; i < USER_ID_SIZE; i++) {
        Serial.printf("%02x", ((char *)(dst->user_id))[i]);
    }
    Serial.println();
    if ( CHECK_OK( getUserById(&(dst->user_id), &current_user) ) ) {
        Serial.println("GENERAL_OK");
        return GENERAL_OK;
    }
    Serial.println("GENERAL_USER_INVALID_ID");
    return GENERAL_USER_INVALID_ID;
}