#pragma once
#include "view.h"
#include "descriptor.h"

#define CONNECTION_TABLE_SIZE 32
typedef struct {
    CommandEnum command;
    View view;
} Connection;
extern const Connection CONNECTION_TABLE[CONNECTION_TABLE_SIZE];

GeneralErr toDataPak(DataPak *dest, ResponseFormat *src, char *status);
GeneralErr toCommandFormat(CommandFormat *dest, DataPak *src, char *status);
GeneralErr controllerRun(DataPak *request, DataPak *response);
GeneralErr test();