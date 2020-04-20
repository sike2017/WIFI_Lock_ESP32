#include "lock.h"
#include "motor.h"
#include <nvs.h>

void closeLock() {
    Lock lock;
    nvs_handle handle;
    size_t len = sizeof(LockStoreFormat);
    ESP_ERROR_CHECK( nvs_open(NVS_LOCK_PART, NVS_READWRITE, &handle) );
    lock.lsf.state = LOCK_CLOSE;
    ESP_ERROR_CHECK( nvs_set_blob(handle, KEY_LOCK, &(lock.lsf), len) );
    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);
    clockwise(ROL_STEPS);
    Serial.println("close lock");
}

void openLock() {
    Lock lock;
    nvs_handle handle;
    size_t len = sizeof(LockStoreFormat);
    ESP_ERROR_CHECK( nvs_open(NVS_LOCK_PART, NVS_READWRITE, &handle) );
    lock.lsf.state = LOCK_OPEN;
    ESP_ERROR_CHECK( nvs_set_blob(handle, KEY_LOCK, &(lock.lsf), len) );
    ESP_ERROR_CHECK( nvs_commit(handle) );
    nvs_close(handle);
    counterClockwise(ROL_STEPS);
    Serial.println("open lock");
}