#pragma once
#include <stdint.h>
#include <stddef.h>

#define PACKAGE_SIZE 64
static const char * const DEFAULT_PASSWORD =  "aabbccdd";
#define COMMAND_SIZE 1
#define RESPONSE_HEADER_SIZE 1
#define RESERVED_BYTES_SIZE 1
#define LOADING_SIZE (PACKAGE_SIZE - RESERVED_BYTES_SIZE)

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
    COMMAND_RESET = 0x30,
    COMMAND_PING = 0x40,
    COMMAND_INIT= 0x50,
    COMMAND_GUEST_UNLOCK = 0x60,
    COMMAND_GUEST_LOCK = 0x61,
    COMMAND_GUEST_ADD = 0x70,
    COMMAND_INVALID = 0xFF,    // invalid command will be turned into COMMAND_INVALID
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