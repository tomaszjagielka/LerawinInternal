#pragma once
#include <Windows.h>
#include <Psapi.h>

#include "dt_recv.h"
#include "client_class.h"

#include "variables.h"
#include "entity.h"
#include "utilities.h"

void *GetInterface(const char *dll_name, const char *interface_name);
intptr_t GetOffset(RecvTable *table, const char *table_name, const char *netvar_name);
intptr_t GetNetVarOffset(ClientClass *client_class, const char *table_name, const char *netvar_name, int offset);
uintptr_t PatternScan(uintptr_t module_address, const char *signature, int offset, int extra, bool relative);