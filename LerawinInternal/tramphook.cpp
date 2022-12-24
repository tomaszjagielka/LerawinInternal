#include <Windows.h>

// Hooking functions by rake and solair.

// Patch bytes function.
void Patch(BYTE *dst, BYTE *src, unsigned int size) {
	DWORD o_proc;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &o_proc);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, o_proc, &o_proc);
}

// Hook function.
bool Hook(char *src, char *dst, int len) {
	if (len < 5) return false;
	DWORD o_proc;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &o_proc);
	memset(src, 0x90, len);
	uintptr_t rel_addy = (uintptr_t)(dst - src - 5); // addy = address.
	*src = (char)0xE9;
	*(uintptr_t *)(src + 1) = (uintptr_t)rel_addy;
	VirtualProtect(src, len, o_proc, &o_proc);

	return true;
}

// Trampoline hook function.
char *TrampHook(char *src, char *dst, unsigned int len) {
	if (len < 5) return 0;
	char *gateway = (char *)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	uintptr_t jump_addy = (uintptr_t)(src - gateway - 5);
	*(gateway + len) = (char)0xE9;
	*(uintptr_t *)(gateway + len + 1) = jump_addy;
	if (Hook(src, dst, len)) {
		return gateway;
	}
	else {
		return nullptr;
	}
}