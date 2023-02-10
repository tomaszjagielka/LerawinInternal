#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "csgo-sdk/vector.h"

#include "directx.h"
#include "math.h"

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
	static ID3DXLine *LineL;

	if (!LineL)
		D3DXCreateLine(p_device, &LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->SetAntialias(true);
	LineL->Draw(Line, 2, color);
}

void DrawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rect = { x, y, x + w, y + h };
	p_device->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawBorderBox(int x, int y, int w, int h, int thickness, D3DCOLOR color)
{
	DrawFilledRect(x, y, w, thickness, color); // Top horizontal line.
	DrawFilledRect(x, y, thickness, h, color); // Left vertical line.
	DrawFilledRect((x + w), y, thickness, h, color); // Right vertical line.
	DrawFilledRect(x, y + h, w + thickness, thickness, color); // Bottom horizontal line.
}

void DrawFilledRectOutlined(int x, int y, int w, int h, int outline_thickness, D3DCOLOR color, D3DCOLOR outline_color)
{
	DrawFilledRect(x, y, w, h, color);	// Box.
	if (outline_thickness)
		DrawBorderBox(x - outline_thickness, y - outline_thickness, w + outline_thickness, h + outline_thickness, outline_thickness, outline_color); // Outline.
}

void DrawBorderBoxOutlined(int x, int y, int w, int h, int thickness, int outline_thickness, D3DCOLOR color, D3DCOLOR outline_color)
{
	if (outline_thickness)
	{
		// Outline.
		DrawBorderBox(x - thickness / 2 - outline_thickness, y - thickness / 2 - outline_thickness, w + thickness + outline_thickness, h + thickness + outline_thickness, outline_thickness, outline_color);
		DrawBorderBox(x - thickness / 2 + thickness, y - thickness / 2 + thickness, w - thickness - outline_thickness, h - thickness - outline_thickness, outline_thickness, outline_color);
	}
	// Box.
	DrawBorderBox(x - thickness / 2, y - thickness / 2, w, h, thickness, color);
}