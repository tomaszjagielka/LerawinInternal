#include "variables.h"
#include "directx.h"
#include "math.h"
#include "csgo-sdk/vector.h"

using namespace Vars;

bool WorldToScreen(vec3 pos, vec2 &screen) {
	vec4 clip_coords;
	clip_coords.x = pos.x * view_matrix[0] + pos.y * view_matrix[1] + pos.z * view_matrix[2] + view_matrix[3];
	clip_coords.y = pos.x * view_matrix[4] + pos.y * view_matrix[5] + pos.z * view_matrix[6] + view_matrix[7];
	//clip_coords.z = pos.x * view_matrix[8] + pos.y * view_matrix[9] + pos.z * view_matrix[10] + view_matrix[11];
	clip_coords.w = pos.x * view_matrix[12] + pos.y * view_matrix[13] + pos.z * view_matrix[14] + view_matrix[15];

	// TODO: Check if changing from 0 to 0.001f fixes ESP flickering all over the screen.
	if (clip_coords.w < 0.001f) return false;

	vec3 NDC;
	NDC.x = clip_coords.x / clip_coords.w;
	NDC.y = clip_coords.y / clip_coords.w;
	//NDC.z = clip_coords.z / clip_coords.w;

	screen.x = (window_width / 2 * NDC.x) + (NDC.x + window_width / 2);
	screen.y = -(window_height / 2 * NDC.y) + (NDC.y + window_height / 2);

	return true;
}

float Distance(vec3 start, vec3 end)
{
	return sqrt((start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y) + (start.z - end.z) * (start.z - end.z));
}