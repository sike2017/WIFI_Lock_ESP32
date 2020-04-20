#include <string.h>
#include "controller.h"
#include "general_err.h"
#include "view.h"
#include "descriptor.h"
#include <Arduino.h>

const Connection CONNECTION_TABLE[CONNECTION_TABLE_SIZE] = {
    { COMMAND_DISCONNECT, viewDisconnect },
    { COMMAND_INIT, viewInit },
    { COMMAND_ADD_USER, viewAddUser },
    { COMMAND_DEL_USER, viewDelUser },
    { COMMAND_OPEN_LOCK, viewOpenLock },
    { COMMAND_CLOSE_LOCK, viewCloseLock },
    { COMMAND_PING, viewPing },
    { COMMAND_GUEST_OPEN_LOCK, viewGuestOpenLock },
    { COMMAND_GUEST_CLOSE_LOCK, viewGuestCloseLock },
    { COMMAND_SET_WIFI, viewSetWifi }
};

GeneralErr toDataPak(DataPak *dest, ResponseFormat *src, char *status) {
    dest->reservedByte = status;
    dest->loading[0] = src->header;
    memcpy(&(dest->loading[COMMAND_SIZE]), src->data, LOADING_SIZE);
    return GENERAL_OK;
}

GeneralErr toCommandFormat(CommandFormat *dest, DataPak *src, char *status) {
    dest->command = src->loading[0];
    memcpy(dest->data, &(src->loading[COMMAND_SIZE]), LOADING_SIZE - COMMAND_SIZE);
    return GENERAL_OK;
}

GeneralErr controllerRun(DataPak *request, DataPak *response) {
    CommandFormat cf;
    ResponseFormat rf;
    char status[RESERVED_BYTES_SIZE];
    GeneralErr result;
    toCommandFormat(&cf, request, status);
    for (int i = 0; i < CONNECTION_TABLE_SIZE; i++) {
        if (CONNECTION_TABLE[i].command == cf.command) {
            result = CONNECTION_TABLE[i].view(&cf, &rf);
            toDataPak(response, &rf, (char *) RESERVED_BYTES);
            return result;
        }
    }
    result = viewProcessInvalidCommand(&cf, &rf);
    toDataPak(response, &rf, (char *) RESERVED_BYTES);
    return result;
}

GeneralErr test() {return GENERAL_OK;}
