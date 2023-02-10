#include <iostream>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "directx.h"
#include "tramphook.h"
#include "gamehooks.h"
#include "variables.h"
#include "entity.h"
#include "features/esp.h"

#define DEBUG_CONSOLE 0

DWORD WINAPI Main(HMODULE h_module) {
	// Needed for Hook and Unhook.
	void *d3d9_device[119];			// vTable of our DirectX device. We're gonna steal it.
	BYTE end_scene_bytes[7]{ 0 };	// Save original. Unhook the game replacing it.

	Vars::InitVars();

	// Hook DirectX9 device and store old gamehooks.
	// Get Dummy Device.
	if (GetD3D9Device(d3d9_device, sizeof(d3d9_device))) {
		// Steal 42-th function entry (EndScene).
		memcpy(end_scene_bytes, (char *)d3d9_device[42], 7);

		// We are using 42th function entry on a vTable and hkEndScene.
		// Replace the 42th function entry with our Entity function.
		oEndScene = (tEndScene)TrampHook((char *)d3d9_device[42], (char *)hkEndScene, 7);
	}

// Let'size initialize console after creating Dummy Device to avoid window size issues.
#if DEBUG_CONSOLE
	FILE *f;
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif

	while (!GetAsyncKeyState(VK_END)) {
		Vars::UpdateVars();
		Sleep(1);
	}

	// Unhook gamehooks.
	Patch((BYTE *)d3d9_device[42], end_scene_bytes, 7);

#if DEBUG_CONSOLE
	// Unhook console.
	if (f) fclose(f);
	FreeConsole();
#endif

	// Wait for game to adapt to it's original functions.
	Sleep(1000);

	// Uninject.
	FreeLibraryAndExitThread(h_module, 0);
}

BOOL APIENTRY DllMain(HMODULE h_module, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, h_module, 0, nullptr));
	return TRUE;
}