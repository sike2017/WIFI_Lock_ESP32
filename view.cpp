#include "view.h"
#include "user.h"
#include "utility.h"
#include "lock.h"
#include "descriptor.h"
#include <Arduino.h>

int isValid(UserId *id) {
    User user;
    return IS_OK( getUserById(id, &user) );
}

GeneralErr viewDisconnect(CommandFormat *p, ResponseFormat *rf) {
    return GENERAL_CONNECTION_DICONNECT;
}

GeneralErr viewPing(CommandFormat *p, ResponseFormat *rf) {
    rf->header = RESPONSE_HEADER_PING;
    memcpy(&(rf->data), &(p->data), sizeof(ResponseFormat::data));
    return GENERAL_OK;
}

GeneralErr viewInit(CommandFormat *p, ResponseFormat *rf) {
    if (!memcmp(&(p->data), DEFAULT_PASSWORD, strlen(DEFAULT_PASSWORD))) {
        User u;
        u.key = USER_00;
        createUUID(u.usf.id, sizeof(UserId));
        setUser(&u);
        rf->header = RESPONSE_HEADER_OK;
        memcpy(rf->data, u.usf.id, sizeof(UserId));
        for (int i = 1; i < USER_NUMBER; i++) {
            delUser((char *)(USER_LIST[i]));
        }
        return GENERAL_OK;
    }
    rf->header = RESPONSE_HEADER_FAIL;
    return GENERAL_USER_PASSWORD_ERROR;
}

GeneralErr viewAddUser(CommandFormat *p, ResponseFormat *rf) {
    int i;
    User u;
    char *key = NULL;
    rf->header = RESPONSE_HEADER_FAIL;
    if (!isValid((UserId *)(p->data))) {
        return GENERAL_USER_INVALID_ID;
    }
    for (i = 0; i < (sizeof(USER_LIST) / sizeof(char *)); i++) {
        key = (char *)USER_LIST[i];
        getUserByKey(key, &u);
        if (u.usf.state == USER_DISABLED) {
            Serial.printf("if key: %s\n", key);
            break;
        }
    }
    if (key == NULL) {
        return GENERAL_USER_FULL;
    }
    u.usf.state = USER_ENABLED;
    u.key = key;
    createUUID((char *)(u.usf.id), sizeof(UserId));
    setUser(&u);
    rf->header = RESPONSE_HEADER_OK;
    memcpy(rf->data, u.usf.id, sizeof(UserId));
    Serial.printf("add user:\nkey: %s\n", u.key);
    return GENERAL_OK;
}

GeneralErr viewDelUser(CommandFormat *p, ResponseFormat *rf) {
    UserId *first = (UserId *)(p->data);
    UserId *second = first + 1;
    User u;
    rf->header = RESPONSE_HEADER_FAIL;
    if (!isValid(first)) {
        return GENERAL_USER_INVALID_ID;
    }
    if ( !IS_OK(getUserById(second, &u)) ) {
        return GENERAL_DEL_INVALID_ID;
    }
    delUser(u.key);
    Serial.printf("del user:\nkey: %s\n", u.key);
    rf->header = RESPONSE_HEADER_OK;
    return GENERAL_OK;
}

GeneralErr viewOpenLock(CommandFormat *p, ResponseFormat *rf) {
    UserId *id = (UserId *)(p->data);
    if (isValid(id)) {
        openLock();
        rf->header = RESPONSE_HEADER_OK;
        return GENERAL_OK;
    }
    rf->header = RESPONSE_HEADER_FAIL;
    return GENERAL_FAIL;
}

GeneralErr viewCloseLock(CommandFormat *p, ResponseFormat *rf) {
    UserId *id = (UserId *)(p->data);
    if (isValid(id)) {
        closeLock();
        rf->header = RESPONSE_HEADER_OK;
        return GENERAL_OK;
    }

    rf->header = RESPONSE_HEADER_FAIL;
    return GENERAL_FAIL;
}

GeneralErr viewProcessInvalidCommand(CommandFormat *p, ResponseFormat *rf) {
    rf->header = RESPONSE_HEADER_INVALID;
    return GENERAL_OK;
}
