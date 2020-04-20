#include "user.h"
#include <nvs.h>
#include <Arduino.h>

const char * USER_LIST[8] = {
    USER_00,
    USER_01,
    USER_02,
    USER_03,
    USER_04,
    USER_05,
    USER_06,
    USER_07
};

GeneralErr setUser(const User *p) {
    nvs_handle handle;
    ESP_ERROR_CHECK( nvs_open(NVS_USER_PART, NVS_READWRITE, &handle) );
    ESP_ERROR_CHECK( nvs_set_blob(handle, p->key, &p->usf, sizeof(UserStoreFormat)) );
    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);
    return GENERAL_OK;
}

GeneralErr getUserById(const UserId *p, User *dest) {
    nvs_handle handle;
    esp_err_t ret_val;
    ret_val = nvs_open(NVS_USER_PART, NVS_READONLY, &handle);
    if (ret_val == ESP_ERR_NVS_NOT_FOUND) {
        return GENERAL_ERR_NVS_NOT_FOUND;
    }
    size_t len = sizeof(dest->usf);
    char *key;
    int flag = 0;
    for (int i = 0; i < USER_NUMBER; i++) {
        key = (char *)USER_LIST[i];
        ESP_ERROR_CHECK( nvs_get_blob(handle, key, &dest->usf, &len) );
        if (!memcmp(p, dest->usf.id, sizeof(UserId))) {
            flag = 1;
            break;
        }
    }
    nvs_close(handle);
    if (!flag) {
        return GENERAL_FAIL;
    }
    dest->key = (char *)key;
    return GENERAL_OK;
}

GeneralErr getUserByKey(const char *key, User *dest) {
    nvs_handle handle;
    esp_err_t ret_val;
    ret_val = nvs_open(NVS_USER_PART, NVS_READONLY, &handle);
    if (ret_val == ESP_ERR_NVS_NOT_FOUND) {
        return GENERAL_ERR_NVS_NOT_FOUND;
    }
    size_t len = sizeof(dest->usf);
    ret_val = nvs_get_blob(handle, key, &dest->usf, &len);
    nvs_close(handle);
    if (ret_val == ESP_ERR_NVS_NOT_FOUND) {
        return GENERAL_ERR_NVS_NOT_FOUND;
    }
    dest->key = (char *)key;
    return GENERAL_OK;
}

GeneralErr delUser(char *key) {
    nvs_handle handle;
    ESP_ERROR_CHECK( nvs_open(NVS_USER_PART, NVS_READWRITE, &handle) );
    UserStoreFormat usf;
    usf.state = USER_DISABLED;
    memset(usf.id, 0, sizeof(UserId));
    ESP_ERROR_CHECK( nvs_set_blob(handle, key, &usf, sizeof(UserStoreFormat)) );
    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);
    return GENERAL_OK;
}
