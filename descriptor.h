#pragma once
#include <stdint.h>
#include <stddef.h>

#define PACKAGE_SIZE 128
static const char * const DEFAULT_PASSWORD =  "aabbccdd";
#define COMMAND_SIZE 1
#define RESPONSE_HEADER_SIZE 1
#define RESERVED_BYTES_SIZE 1
#define LOADING_SIZE (PACKAGE_SIZE - RESERVED_BYTES_SIZE)
#define CIPHERED_LENGTH_SIZE 8
#define USER_ID_SIZE 16
#define NVS_KEY_MAX_SIZE 16
#define WIFI_TIME_OUT 10000 // milliseconds
typedef struct {
    char data[PACKAGE_SIZE];
    char *reservedByte;
    char *loading;
} DataPak;
typedef enum {
    COMMAND_DISCONNECT = 0X01,
    COMMAND_OPEN_LOCK = 0X10,
    COMMAND_CLOSE_LOCK = 0X11,
    COMMAND_ADD_USER = 0x20,
    COMMAND_DEL_USER = 0X21,
    // COMMAND_RESET = 0x30,
    COMMAND_PING = 0x40,
    COMMAND_INIT= 0x50,
    COMMAND_GUEST_OPEN_LOCK = 0x60,
    COMMAND_GUEST_CLOSE_LOCK = 0x61,
    COMMAND_SET_WIFI = 0x70
} CommandEnum;

typedef struct {
    char command;
    char data[LOADING_SIZE - COMMAND_SIZE];
} CommandFormat;

typedef enum {
    RESPONSE_HEADER_OK = 0x10,
    RESPONSE_HEADER_FAIL = 0X11,
    RESPONSE_HEADER_PING = 0x20,
    // if receieved the invalid command, respond with RESPONSE_HEADER_INVALID
    RESPONSE_HEADER_INVALID = 0XFF
} ResponseHeaderEnum;

typedef struct {
    char header;
    char data[LOADING_SIZE - RESPONSE_HEADER_SIZE];
} ResponseFormat;
const char RESERVED_BYTES[RESERVED_BYTES_SIZE] = { 0 };