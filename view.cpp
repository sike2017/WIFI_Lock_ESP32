#include "view.h"
#include "user.h"
#include "utility.h"
#include "lock.h"
#include <Arduino.h>

#include "guest.h"
#include "ntp_time.h"
#include "wifi_settings.h"

int isValid(UserId *id) {
    User user;
    return CHECK_OK( getUserById(id, &user) );
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
        memcpy(&(u.usf.key), u.key, strlen(u.key)+1);
        createUUID(u.usf.id, sizeof(UserId));
        setUser(&u);
        rf->header = RESPONSE_HEADER_OK;
        memcpy(rf->data, u.usf.id, sizeof(UserId));
        memcpy(&(rf->data[sizeof(u.usf.id)]), u.usf.key, strlen(u.key)+1);
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
            break;
        }
    }
    if (key == NULL) {
        return GENERAL_USER_FULL;
    }
    u.usf.state = USER_ENABLED;
    u.key = key;
    memcpy(&(u.usf.key), u.key, strlen(u.key)+1);
    createUUID((char *)(u.usf.id), sizeof(UserId));
    setUser(&u);
    rf->header = RESPONSE_HEADER_OK;
    memcpy(rf->data, u.usf.id, sizeof(UserId));
    memcpy(&(rf->data[sizeof(u.usf.id)]), u.usf.key, strlen(u.key)+1);
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
    if ( !CHECK_OK(getUserById(second, &u)) ) {
        return GENERAL_DEL_INVALID_ID;
    }
    delUser(u.key);
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

GeneralErr viewGuestOpenLock(CommandFormat *p, ResponseFormat *rf) {
    Serial.println("in viewGuestOpenLock");
    Guest guest;
    rf->header = RESPONSE_HEADER_FAIL;

    if ( !CHECK_OK( decrypt(p->data, &guest) ) ) {
        Serial.println("decrypt error");
        return GENERAL_FAIL;
    }
    int64_t now = getLocalTimestamp();
    if ( now > guest.endTimestamp ) {
        // over time
        Serial.println("over time");
        return GENERAL_FAIL;
    }
    rf->header = RESPONSE_HEADER_OK;

    openLock();
    return GENERAL_OK;
}

GeneralErr viewGuestCloseLock(CommandFormat *p, ResponseFormat *rf) {
    Serial.println("in viewGuestCloseLock");
    Guest guest;
    rf->header = RESPONSE_HEADER_FAIL;

    if ( !CHECK_OK( decrypt(p->data, &guest) ) ) {
        Serial.println("decrypt error");
        return GENERAL_FAIL;
    }
    int64_t now = getLocalTimestamp();
    if ( now > guest.endTimestamp ) {
        // over time
        Serial.println("over time");
        return GENERAL_FAIL;
    }
    rf->header = RESPONSE_HEADER_OK;

    closeLock();
    return GENERAL_OK;
}

GeneralErr viewSetWifi(CommandFormat *p, ResponseFormat *rf) {
    UserId *userId = (UserId *)(p->data);
    rf->header = RESPONSE_HEADER_FAIL;
    if (!isValid(userId)) {
        return GENERAL_FAIL;
    }
    char *ssid = (char *)(userId + 1);
    char *password;
    int zeroCount = 0;
    char *wifiData = ssid;
    for (int i = 0; i < (LOADING_SIZE - USER_ID_SIZE); i++) {
        if (zeroCount == 2) {
            break;
        }
        if (wifiData[i] == '\0') {
            if (zeroCount == 0) {
                password = &wifiData[i + 1];
            }
            zeroCount++;
        }
    }
    if (zeroCount != 2) {
        return GENERAL_FAIL;
    }
    WifiSetting wf;
    if (strlen(ssid) >= sizeof(wf.ssid)) {
        return GENERAL_FAIL;
    }
    if (strlen(password) >= sizeof(wf.password)) {
        return GENERAL_FAIL;
    }
    strcpy(wf.ssid, ssid);
    strcpy(wf.password, password);
    setWifiStaSetting(&wf);
    setWifiMode(WIFI_MODE_STA);
    ESP.restart();
}

GeneralErr viewVersion(CommandFormat *p, ResponseFormat *rf) {
    rf->header = RESPONSE_HEADER_OK;
    char *version = VERSION;
    memcpy(rf->data, version, (strlen(version) + 1));
    
    return GENERAL_OK;
}
