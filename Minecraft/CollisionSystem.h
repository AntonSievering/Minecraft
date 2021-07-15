#pragma once

#include "Hitbox.h"

namespace CollisionSystem
{
	static inline Engine::vf3d CreateCollisionOffsetVector(const aabb::Hitbox3d &dbox, const aabb::Hitbox3d &sbox) noexcept
	{
		Engine::vf3d vOverlap = sbox.getOverlap(dbox);
		return std::abs(vOverlap);
	}

	static inline void ResolveCollision(aabb::Hitbox3d &dbox, const aabb::Hitbox3d &sbox) noexcept
	{
		Engine::vf3d vOverlap = sbox.getOverlap(dbox);
		Engine::vf3d vAbsOverlap = std::abs(vOverlap);

		if (vAbsOverlap.x < vAbsOverlap.y && vAbsOverlap.x < vAbsOverlap.z)
			dbox.pos.x += vOverlap.x;
		else if (vAbsOverlap.y < vAbsOverlap.x && vAbsOverlap.y < vAbsOverlap.z)
			dbox.pos.y += vOverlap.y;
		else
			dbox.pos.z += vOverlap.z;
	}
}
