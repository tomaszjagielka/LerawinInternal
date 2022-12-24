#pragma once

#include "vector.h"

ID3DXLine *LineL;

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
void DrawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color);

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col);
void DrawFilledRectOutlined(int x, int y, int w, int h, int outline_thickness, D3DCOLOR color, D3DCOLOR outline_color);
void DrawBorderBoxOutlined(int x, int y, int w, int h, int thickness, int outline_thickness, D3DCOLOR color, D3DCOLOR outline_color);