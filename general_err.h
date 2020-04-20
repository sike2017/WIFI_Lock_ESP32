#pragma once
#include <esp_err.h>

#define GENERAL_OK ESP_OK
#define GENERAL_FAIL ESP_FAIL

#define GENERAL_ERR_NVS_BASE 0x10
#define GENERAL_ERR_NVS_NOT_FOUND (GENERAL_ERR_NVS_BASE + 0x01)
#define GENERAL_ERR_NVS_INVALID_KEY (GENERAL_ERR_NVS_BASE + 0x02)

#define GENERAL_ERR_USER_BASE 0x20
#define GENERAL_USER_INVALID_ID (GENERAL_ERR_USER_BASE + 0x01)
#define GENERAL_USER_PASSWORD_ERROR (GENERAL_ERR_USER_BASE + 0x02)
#define GENERAL_USER_FULL (GENERAL_ERR_USER_BASE + 0x03)
#define GENERAL_DEL_INVALID_ID (GENERAL_ERR_USER_BASE + 0x04)

#define GENERAL_CONNECTION_BASE 0x30
#define GENERAL_CONNECTION_DICONNECT (GENERAL_CONNECTION_BASE + 0x01)
#define GENERAL_CONNECTION_TIME_OUT (GENERAL_CONNECTION_BASE + 0x02)

#define GENERAL_ERR_AES_BASE 0x40
#define GENERAL_ERR_AES_INVALID_INPUT_LENGTH (GENERAL_ERR_AES_BASE + 0x01)

typedef int32_t GeneralErr;
#define CHECK_OK(X) ( X == GENERAL_OK )

/**
 * Macro which can be used to check the error code,
 * and terminate the program in case the code is not GENERAL_OK.
 * Prints the error code, error location, and the failed statement to serial output.
 *
 * Disabled if assertions are disabled.
 */
#ifdef NDEBUG
#define GENERAL_ERROR_CHECK(x) do {                                     \
        GeneralErr __err_rc = (x);                                      \
        (void) sizeof(__err_rc);                                        \
    } while(0);
#elif defined(CONFIG_OPTIMIZATION_ASSERTIONS_SILENT)
#define GENERAL_ERROR_CHECK(x) do {                                     \
        GeneralErr __err_rc = (x);                                      \
        if (__err_rc != GENERAL_OK) {                                   \
            abort();                                                    \
        }                                                               \
    } while(0);
#else
#define GENERAL_ERROR_CHECK(x) do {                                     \
        GeneralErr __err_rc = (x);                                      \
        if (__err_rc != GENERAL_OK) {                                   \
            _esp_error_check_failed(__err_rc, __FILE__, __LINE__,       \
                                    __ASSERT_FUNC, #x);                 \
        }                                                               \
    } while(0);
#endif
