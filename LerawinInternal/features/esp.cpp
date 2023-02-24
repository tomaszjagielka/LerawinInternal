#include <algorithm>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "../directx.h"
#include "../features/esp.h"
#include "../drawing.h"
#include "../variables.h"
#include "../utilities.h"

void Crosshair() {
	DrawFilledRect(window_width / 2 - 2, window_height / 2 - 2, 4, 4, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void Snaplines(Ent *ent, vec2 ent_pos_2D) {
	D3DCOLOR color;
	if (ent->Visible())
		color = D3DCOLOR_ARGB(255, 255, 255, 0);
	else if (ent->TeamNum() == Vars::local_ent->TeamNum())
		color = D3DCOLOR_ARGB(255, 0, 0, 255);
	else
		color = D3DCOLOR_ARGB(255, 255, 0, 0);

	DrawLine(ent_pos_2D.x, ent_pos_2D.y, window_width / 2, window_height, 1, color);
		
	//static std::vector<int> bones = {
	//	8,	// Head.
	//	6,	// Chest.
	//	0,	// Belly.
	//	13, // Left hand.
	//	43, // Right hand.
	//	69, // T Left feet.
	//	72, // CT Right feet.
	//	76,	// T Right feet.
	//	79, // CT Left feet.
	//};

	//for (auto i = bones.begin(); i != bones.end(); i++) {
	//	vec3 targetbonepos = ent->BonesPos(*i);
	//	vec2 bonePos2D;
	//	if (WorldToScreen(targetbonepos, bonePos2D))
	//		DrawFilledRect(bonePos2D.x, bonePos2D.y, 5, 5, color);
	//}
}

void DrawEspBox2D(vec2 top, vec2 bot, int thickness, int outline_thickness, D3DCOLOR color, D3DCOLOR outline_color)
{
	int height = ABS(top.y - bot.y);

	vec2 tl, tr;
	tl.x = top.x - height / 4;
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;

	vec2 bl, br;
	bl.x = bot.x - height / 4; // bot.x (not top.x) draws dynamic box if using DrawLine.
	br.x = bot.x + height / 4; // bot.x
	bl.y = br.y = bot.y;

	//DrawLine(tl, tr, thickness, color);
	//DrawLine(bl, br, thickness, color);
	//DrawLine(tl, bl, thickness, color);
	//DrawLine(tr, br, thickness, color);

	// Minus, because I want to draw box from bl to tr.
	DrawBorderBoxOutlined(tl.x, bl.y, tr.x - tl.x, -(br.y - tr.y), thickness, outline_thickness, color, outline_color);
}

void Status(Ent *ent, vec2 ent_pos_2D, vec2 ent_head_2D, int box_thickness, int box_outline_thickness, int status_thickness, int status_outline_thickness, D3DCOLOR status_outline_color)
{
	int height = ABS(ent_pos_2D.y - ent_head_2D.y); // Height of ESP Box.
	//int delta_x = (ent_pos_2D.x - ent_head_2D.x); // Used in dynamic ESP Box.

	float health_perc = ent->Health() / 100.f;
	float armor_perc = ent->Armor() / 100.f;

	vec2 bot_health, top_health, bot_armor, top_armor;

	// HEIGHT.
	bot_health.y = bot_armor.y = ent_pos_2D.y + box_thickness / 2 + box_outline_thickness - status_outline_thickness;

	int health_height = height * health_perc;
	int armor_height = height * armor_perc;

	top_health.y = ent_head_2D.y + height - box_thickness / 2 - box_outline_thickness + status_outline_thickness - health_height;
	top_armor.y = ent_head_2D.y + height - box_thickness / 2 - box_outline_thickness + status_outline_thickness - armor_height;

	// WIDTH.
	top_health.x = bot_health.x = ent_pos_2D.x - height / 4 - box_thickness / 2 - box_outline_thickness - status_thickness - status_outline_thickness - 1;
	top_armor.x = bot_armor.x = ent_pos_2D.x + (height / 4) + box_outline_thickness * 2 + 2;

	DrawFilledRectOutlined(top_health.x, top_health.y, status_thickness, -(top_health.y - bot_health.y), status_outline_thickness, D3DCOLOR_ARGB(255, 0, 150, 0), status_outline_color);
	DrawFilledRectOutlined(top_armor.x, top_armor.y, status_thickness, -(top_armor.y - bot_armor.y), status_outline_thickness, D3DCOLOR_ARGB(255, 30, 144, 255), status_outline_color);
}

void Player2D(Ent *ent, vec2 ent_pos_2D, vec2 ent_head_2D, int box_thickness, int box_outline_thickness, int status_thickness, int status_outline_thickness) {
	// COLORS.
	D3DCOLOR box_color;
	D3DCOLOR box_outline_color = D3DCOLOR_ARGB(255, 0, 0, 0);
	D3DCOLOR status_outline_color = D3DCOLOR_ARGB(255, 0, 0, 0);

	if (ent->Visible())
		box_color = D3DCOLOR_ARGB(1, 255, 255, 0);
	else if (ent->TeamNum() == Vars::local_ent->TeamNum())
		box_color = D3DCOLOR_ARGB(1, 0, 0, 255);
	else
		box_color = D3DCOLOR_ARGB(1, 255, 0, 0);

	DrawEspBox2D(ent_pos_2D, ent_head_2D, box_thickness, box_outline_thickness, box_color, box_outline_color);
	//Status(ent, ent_pos_2D, ent_head_2D, box_thickness, box_outline_thickness, status_thickness, status_outline_thickness, status_outline_color);
}

void ESP::Main() {
	if (!Vars::local_ent)
		return;

	Update();

	std::vector<Ent*> players_by_distance;
	for (int i = 2; i <= 32; i++)
	{
		Ent *ent = (Ent *)Vars::ent_list->GetClientEntity(i);

		if (!ent->Valid())
			continue;

		if (ent->TeamNum() == Vars::local_ent->TeamNum())
			continue;

		players_by_distance.push_back(ent);
	}

	// Sometimes causes crash when unloading (can't increment vector iterator past end crash).
	sort(players_by_distance.begin(), players_by_distance.end());

	for (Ent *ent : players_by_distance) {
		vec2 ent_pos_2D;
		if (WorldToScreen(ent->VecOrigin(), ent_pos_2D))
		{
			//Snaplines(ent, ent_pos_2D);

			vec3 ent_head_3D = ent->BonesPos(8);
			ent_head_3D += 8; // Make box a bit higher.
			vec2 ent_head_2D;

			if (WorldToScreen(ent_head_3D, ent_head_2D))
			{
				//int box_thickness = 31;
				//int box_outline_thickness = 31;
				//int status_thickness = 21;
				//int status_outline_thickness = 16;
				int box_thickness = 2;
				int box_outline_thickness = 1;
				int status_thickness = 2;
				int status_outline_thickness = 1;

				Player2D(ent, ent_pos_2D, ent_head_2D, box_thickness, box_outline_thickness, status_thickness, status_outline_thickness);
			}
		}
	}

	//Crosshair();
}

void ESP::Update() {
	memcpy(&Vars::view_matrix, (PBYTE *)(Vars::client + Vars::dwViewMatrix), sizeof(Vars::view_matrix));
}