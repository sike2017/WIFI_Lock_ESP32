#pragma once
#include <Arduino.h>
#include "general_err.h"
#include "storage.h"
#include "descriptor.h"
typedef char UserId[USER_ID_SIZE];
typedef enum {
    USER_DISABLED = 0x10,
    USER_ENABLED = 0x11
} UserState;

typedef struct {
    UserState state;
    UserId id;
    char key[NVS_KEY_MAX_SIZE];
} UserStoreFormat;

typedef struct {
    UserStoreFormat usf;
    char *key;
} User;
#define USER_NUMBER 8
#define USER_00 "user_00"
#define USER_01 "user_01"
#define USER_02 "user_02"
#define USER_03 "user_03"
#define USER_04 "user_04"
#define USER_05 "user_05"
#define USER_06 "user_06"
#define USER_07 "user_07"
extern const char * USER_LIST[USER_NUMBER];
// Use NVS to add user information.
GeneralErr setUser(const User *pUser);
// Get User. Return GENERAL_OK if the user is found, return GENERAL_FAIL if failed.
GeneralErr getUserById(const UserId *p, User *dest);
// Get User. Return GENERAL_OK if the user is found, return GENERAL_FAIL if failed.
GeneralErr getUserByKey(const char *key, User *dest);
// Delete user.
GeneralErr delUser(char *key);
