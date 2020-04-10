
#include "view.h"
#include "descriptor.h"

#define CONNECTION_TABLE_SIZE 32
typedef struct {
    CommandEnum command;
    View view;
} Connection;
const Connection CONNECTION_TABLE[CONNECTION_TABLE_SIZE] = {
    { COMMAND_DISCONNECT, viewDisconnect },
    { COMMAND_INIT, viewInit },
    { COMMAND_ADD_USER, viewAddUser },
    { COMMAND_DEL_USER, viewDelUser },
    { COMMAND_OPEN_LOCK, viewOpenLock },
    { COMMAND_CLOSE_LOCK, viewCloseLock },
    { COMMAND_PING, viewPing },
    { COMMAND_INVALID, viewProcessInvalidCommand }
};

GeneralErr toDataPak(DataPak *dest, ResponseFormat *src, char *status);
GeneralErr toCommandFormat(CommandFormat *dest, DataPak *src, char *status);
GeneralErr controllerRun(DataPak *request, DataPak *response);
GeneralErr test();