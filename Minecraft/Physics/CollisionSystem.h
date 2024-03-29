#pragma once

#include "Hitbox.h"
#include "../GameMechanics/Entity/Entity.h"

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

		auto isNegative = [](const float value) -> bool
		{
			return value < 0.0f;
		};

		auto updateVelocity = [isNegative](float &vel, const float fOverlap) -> void
		{
			if (isNegative(vel) != isNegative(fOverlap))
				vel = 0.0f;
		};

		if (vAbsOverlap.y < vAbsOverlap.x && vAbsOverlap.y < vAbsOverlap.z)
		{
			entity.hitbox.pos.y += vOverlap.y;
			updateVelocity(entity.vel.y, vOverlap.y);

			if (vOverlap.y > 0.0f)
				entity.bGrounded = true;
		}
		else if (vAbsOverlap.x < vAbsOverlap.z)
		{
			entity.hitbox.pos.x += vOverlap.x;
			updateVelocity(entity.vel.x, vOverlap.x);
		}
		else
		{
			entity.hitbox.pos.z += vOverlap.z;
			updateVelocity(entity.vel.z, vOverlap.y);
		}
	}
}
