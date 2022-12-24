#pragma once

//// 2D.
//struct Vec2 {
//	float x, y;
//};
//// 3D.
//struct Vec3 {
//	float x, y, z;
//};
//// 3D / RGBA.
//struct Vec4 {
//	float x, y, z, w;
//};

#define ABS(x) ((x < 0) ? (-x) : (x))

bool WorldToScreen(vec3 pos, vec2& screen);
float Distance(vec3 start, vec3 end);