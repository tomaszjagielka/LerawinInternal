#pragma once

#include <Windows.h>

#include "csgo-sdk/cdll_int.h"
#include "csgo-sdk/client_class.h"
#include "csgo-sdk/IClientEntityList.h"
#include "csgo-sdk/traceline.h"

#include "utilities.h"
#include "variables.h"
#include "entity.h"

namespace Vars {
	// Important.
	inline uintptr_t engine;
	inline uintptr_t client;

	// Interfaces.
	inline ClientClass *client_class;

	// Offsets.
	inline uintptr_t dwLocalPlayer;
	inline uintptr_t dwEntityList;
	inline uintptr_t dwViewMatrix;

	// Netvars.
	inline uintptr_t m_bDormant;
	inline uintptr_t m_iHealth;
	inline uintptr_t m_ArmorValue;
	inline uintptr_t m_aimPunchAngle;
	inline uintptr_t m_vecOrigin;
	inline uintptr_t m_iTeamNum;
	inline uintptr_t m_vecViewOffset;
	inline uintptr_t m_dwBoneMatrix;

	// Entity.
	inline Ent *local_ent;
	inline IClientEntityList *ent_list;
	inline IEngineTrace *engine_trace;

	// Others.
	inline float view_matrix[16];

	// Functions.
	void InitVars();
	void UpdateVars();
}