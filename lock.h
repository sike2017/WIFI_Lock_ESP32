#pragma once
#include <nvs.h>
#include "general_err.h"
#include "storage.h"

typedef enum {
    LOCK_OPEN,
    LOCK_CLOSE
} LockState;

typedef struct {
    LockState state;
} LockStoreFormat;

typedef struct {
    LockStoreFormat lsf;
} Lock;

#define KEY_LOCK "lock"

// Close the lock. Always return GENERAL_OK.
GeneralErr closeLock();
// Open the lock. Always return GENERAL_OK.
GeneralErr openLock();
