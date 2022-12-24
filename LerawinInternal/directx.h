#pragma once
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Typedef for entity template. [?]
typedef HRESULT(APIENTRY *tEndScene)(LPDIRECT3DDEVICE9 p_device);
inline LPDIRECT3DDEVICE9 p_device;
bool GetD3D9Device(void **p_table, size_t size);

HWND GetProcessWindow();
void GetClientSize();
static HWND window;
inline float window_height, window_width;