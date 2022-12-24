#pragma once

void Patch(BYTE *dst, BYTE *src, unsigned int size);
char *TrampHook(char *src, char *dst, unsigned int len);
