#include <Windows.h>
#include <Psapi.h>

#include "csgo-sdk/cdll_int.h"
#include "csgo-sdk/client_class.h"
#include "csgo-sdk/IClientEntityList.h"
#include "csgo-sdk/traceline.h"

#include "utilities.h"
#include "directx.h"
#include "variables.h"

void Vars::InitVars() {
	// IMPORTANT.
	client = (uintptr_t)GetModuleHandle("client.dll");
	engine = (uintptr_t)GetModuleHandle("engine.dll");

	// INTERFACES.
	IBaseClientDLL *BaseClientDLL = (IBaseClientDLL *)GetInterface("client.dll", "VClient018");
	client_class = BaseClientDLL->GetAllClasses();
	ent_list = (IClientEntityList *)GetInterface("client.dll", "VClientEntityList003");
	engine_trace = (IEngineTrace *)GetInterface("engine.dll", "EngineTraceClient004");

	// NETVARS - hazedumper.
	dwLocalPlayer = PatternScan(client, "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF", 3, 4, true);
	dwEntityList = PatternScan(client, "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", 1, 0, true);
	dwViewMatrix = PatternScan(client, "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9", 3, 176, true);
	m_bDormant = PatternScan(client, "8A 81 ? ? ? ? C3 32 C0", 2, 8, false);
	m_iHealth = GetNetVarOffset(client_class, "DT_BasePlayer", "m_iHealth", 0);
	m_ArmorValue = GetNetVarOffset(client_class, "DT_CSPlayer", "m_ArmorValue", 0);
	m_aimPunchAngle = GetNetVarOffset(client_class, "DT_BasePlayer", "m_aimPunchAngle", 0);
	m_vecOrigin = GetNetVarOffset(client_class, "DT_BasePlayer", "m_vecOrigin", 0);
	m_iTeamNum = GetNetVarOffset(client_class, "DT_BasePlayer", "m_iTeamNum", 0);
	m_vecViewOffset = GetNetVarOffset(client_class, "DT_CSPlayer", "m_vecViewOffset[0]", 0);
	m_dwBoneMatrix = GetNetVarOffset(client_class, "DT_BaseAnimating", "m_nForceBone", 28);

	// GetClientEntity(1) doesn't always get correct entity in death match (local player).
	//local_ent = (Ent *)ent_list->GetClientEntity(1); // 1 is local_ent.
	local_ent = *(Ent **)(client + dwLocalPlayer);
}

void Vars::UpdateVars() {
	GetClientSize();
	//local_ent = (Ent *)ent_list->GetClientEntity(1);
	local_ent = *(Ent **)(client + dwLocalPlayer);
}