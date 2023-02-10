#include <Windows.h>

#include "csgo-sdk/VMatrix.h"
#include "csgo-sdk/Studio.h"
#include "csgo-sdk/IClientRenderable.h"
#include "csgo-sdk/csgo-Vector.h"

#include "variables.h"
#include "math.h"
#include "entity.h"

bool Ent::Valid() {
	if (!this)
		return false;
	if (this == Vars::local_ent)
		return false;
	if (this->Dormant())
		return false;
	if (this->Health() <= 0)
		return false;
	return true;
}

bool Ent::Visible() {
	vec3 local_eye_pos = Vars::local_ent->VecOrigin() + Vars::local_ent->VecViewOffset();

	static std::vector<int> bones = {
		8,	// Head.
		6,	// Chest.
		0,	// Belly.
		13, // Left hand.
		43, // Right hand.
		69, // T Left feet.
		72, // CT Right feet.
		76,	// T Right feet.
		79, // CT Left feet.
	};

	CTraceFilter trace_filter;
	trace_filter.pSkip = (void *)Vars::local_ent;

	// Head -> stomach.
	for (auto i = bones.begin(); i != bones.end(); i++) {
		CGameTrace trace;
		Ray_t ray;

		vec3 target_pos = this->BonesPos(*i);

		ray.Init(local_eye_pos, target_pos);
		Vars::engine_trace->TraceRay(ray, /*MASK_SHOT | CONTENTS_GRATE*//*0x4600400B*/0x46004009, &trace_filter, &trace);

		if (trace.hit_entity == this || trace.fraction > 0.97f) // trace.fraction somehow fixes problem with visible not working on previously killed entities. [?]
			return true;
	}

	return false;
}

bool Ent::Dormant() {
	return GetValue<bool>(Vars::m_bDormant);
}

int Ent::TeamNum() {
	return GetValue<int>(Vars::m_iTeamNum);
}

int Ent::Health() {
	return GetValue<int>(Vars::m_iHealth);
}

int Ent::Armor() {
	return GetValue<int>(Vars::m_ArmorValue);
}

vec3 Ent::VecOrigin() {
	return GetValue<vec3>(Vars::m_vecOrigin);
}

vec3 Ent::VecViewOffset() {
	return GetValue<vec3>(Vars::m_vecViewOffset);
}

vec3 Ent::BonesPos(int bone) {
	uintptr_t bone_base = GetValue<uintptr_t>(Vars::m_dwBoneMatrix);
	vec3 bone_pos;
	bone_pos.x = *(float *)(bone_base + 0x30 * bone + 0x0C);
	bone_pos.y = *(float *)(bone_base + 0x30 * bone + 0x1C);
	bone_pos.z = *(float *)(bone_base + 0x30 * bone + 0x2C);
	return bone_pos;
}

bool CompareDistance(Ent *e1, Ent *e2)
{
	float e1_distance = Distance(Vars::local_ent->VecOrigin(), e1->VecOrigin());
	float e2_distance = Distance(Vars::local_ent->VecOrigin(), e2->VecOrigin());
	return (e1_distance > e2_distance);
}

//matrix3x4_t Ent::GetBoneMatrix(int bone) {
//	matrix3x4_t matrix;
//
//	auto offset = *reinterpret_cast<uintptr_t *>(uintptr_t(this) + 0x26A8 /*m_dwBoneMatrix*/);
//	if (offset)
//		matrix = *reinterpret_cast<matrix3x4_t *>(offset + 0x30 * bone);
//
//	return matrix;
//}
//
//Vector Ent::GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix)
//{
//	studiohdr_t *hdr = g_pModelInfo->GetStudiomodel(this->GetModel());
//	mstudiohitboxset_t *set = hdr->GetHitboxSet(0);
//	mstudiobbox_t *hitbox = set->GetHitbox(Hitbox);
//
//	if (hitbox)
//	{
//		Vector vMin, vMax, vCenter, sCenter;
//		g_Math.VectorTransform(hitbox->min, Matrix[hitbox->bone], vMin);
//		g_Math.VectorTransform(hitbox->max, Matrix[hitbox->bone], vMax);
//		vCenter = (vMin + vMax) * 0.5;
//
//		return vCenter;
//	}
//
//	return Vector(0, 0, 0);
//}


//Vector Ent::GetBonePos(int bone) {
//	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
//	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
//		return boneMatrix[bone].at(3);
//	}
//	return Vector{};
//}