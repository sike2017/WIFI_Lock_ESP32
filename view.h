#pragma once

#include "lock.h"
#include "descriptor.h"

#include "guest.h"

typedef GeneralErr (*View)(CommandFormat *, ResponseFormat *);
GeneralErr viewDisconnect(CommandFormat *p, ResponseFormat *rf);
GeneralErr viewPing(CommandFormat *p, ResponseFormat *rf);
// Init with the default password.
GeneralErr viewInit(CommandFormat *p, ResponseFormat *rf);
// Add user. Data part of the CommandFormat: UserId.
GeneralErr viewAddUser(CommandFormat *p, ResponseFormat *rf);
// Delete user. Data part of the CommandFormat: UserId, UserId.
GeneralErr viewDelUser(CommandFormat *p, ResponseFormat *rf);
// Open lock. Data part of the CommandFormat: UserId.
GeneralErr viewOpenLock(CommandFormat *p, ResponseFormat *rf);
// Close lock. Data part of the COmmandFormat: UserId.
GeneralErr viewCloseLock(CommandFormat *p, ResponseFormat *rf);
// Invalid command process.
GeneralErr viewProcessInvalidCommand(CommandFormat *p, ResponseFormat *rf);
// Open lock from a guest.
GeneralErr viewGuestOpenLock(CommandFormat *p, ResponseFormat *rf);
// Close lock from a guest.
GeneralErr viewGuestCloseLock(CommandFormat *p, ResponseFormat *rf);
// Set WiFi.
GeneralErr viewSetWifi(CommandFormat *p, ResponseFormat *rf);
