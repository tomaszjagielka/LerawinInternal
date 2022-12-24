#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "directx.h"

// Callback to be called for each toplevel window.
BOOL CALLBACK enumWind(HWND handle, LPARAM lp) {
	DWORD proc_id;
	GetWindowThreadProcessId(handle, &proc_id);
	static uintptr_t curr_proc_id = GetCurrentProcessId();
	if (curr_proc_id != proc_id)
		return 1;

	window = handle;
	return 0;
}

void GetClientSize() {
	RECT size;
	GetClientRect(window, &size);

	window_width = (float)size.right - (float)size.left;
	window_height = (float)size.bottom - (float)size.top;
}

HWND GetProcessWindow() {
	EnumWindows(enumWind, 0);
	GetClientSize();

	return window;
}

// Get vTable.
bool GetD3D9Device(void **p_table, size_t size) {
	if (!p_table)
		return false;

	IDirect3D9 *p_D3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!p_D3D)
		return false;

	IDirect3DDevice9 *p_dummy_device = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummy_dev_created = p_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &p_dummy_device);

	if (dummy_dev_created != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummy_dev_created = p_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &p_dummy_device);

		if (dummy_dev_created != S_OK) {
			p_D3D->Release();
			return false;
		}
	}

	memcpy(p_table, *(void ***)(p_dummy_device), size);
	p_dummy_device->Release();
	p_D3D->Release();
	return true;
}