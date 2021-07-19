#pragma once

#include "Hitbox.h"
#include "Entity.h"

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

	static inline void ResolveEntityCollision(Entity &entity, const aabb::Hitbox3d &sbox) noexcept
	{
		Engine::vf3d vOverlap = sbox.getOverlap(entity.hitbox);
		Engine::vf3d vAbsOverlap = std::abs(vOverlap);

		if (vAbsOverlap.x < vAbsOverlap.y && vAbsOverlap.x < vAbsOverlap.z)
		{
			entity.hitbox.pos.x += vOverlap.x;
			entity.vel.x = 0.0f;
		}
		else if (vAbsOverlap.y < vAbsOverlap.x && vAbsOverlap.y < vAbsOverlap.z)
		{
			entity.hitbox.pos.y += vOverlap.y;
			entity.vel.y = 0.0f;
			entity.bGrounded = true;
		}
		else
		{
			entity.hitbox.pos.z += vOverlap.z;
			entity.vel.z = 0.0f;
		}
	}
}
