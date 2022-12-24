#pragma once

typedef HRESULT(APIENTRY *tEndScene)(LPDIRECT3DDEVICE9 p_device);

// Create instance of tEndScene template.
inline tEndScene oEndScene = nullptr;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_p_device);