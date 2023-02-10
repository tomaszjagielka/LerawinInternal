#include <Windows.h>

#include "directx.h"
#include "features/esp.h"
#include "gamehooks.h"

// Hook function (hk prefix = hook).
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_p_device) {
	if (!p_device)
		p_device = o_p_device;

	ESP::Main();

	// Call original function, which we saved earlier.
	oEndScene(p_device);
}