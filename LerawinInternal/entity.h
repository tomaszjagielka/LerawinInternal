#pragma once

#include "math.h"
#include "csgo-sdk/csgo-Vector.h"

#include "csgo-sdk/vector.h"
#include "csgo-sdk/VMatrix.h"

// Automatically pads remaining bytes.
//#define STR_MERGE_IMPL(a, b) a##b
//#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
//#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
//#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

class Ent {
private:
	template<class T>
	T GetValue(const int offset)
	{
		return *reinterpret_cast<T *>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
public:
	int test;
	bool Valid();
	bool Visible();
	bool Dormant();
	int Health();
	int Armor();
	int TeamNum();
	vec3 VecOrigin();
	vec3 VecViewOffset();
	vec3 BonesPos(int bone);
	//Vector HitboxPos(int Hitbox, matrix3x4_t *Matrix);
	//Vector GetBonePos(int bone);
	//matrix3x4_t GetBoneMatrix(int bone);
};

// Sort comparator.
bool CompareDistance(Ent *e1, Ent *e2);

// 0x16
//class EntListObj {
//public:
//	struct Ent* ent;	// 0x4
//	char padding[12];	// 0x12
//};
//
//class EntList {
//public:
//	EntListObj ents[32];
//};