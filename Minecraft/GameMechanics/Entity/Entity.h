#pragma once

#include "../../Physics/Hitbox.h"

struct Entity
{
public:
	aabb::Hitbox3d hitbox{};
	Engine::vf3d vel{};
	bool bGrounded = false;

public:
	Entity() noexcept = default;

	Entity(const aabb::Hitbox3d &hitbox) noexcept
	{
		this->hitbox = hitbox;
	}

	Entity(const Engine::vf3d &pos, const Engine::vf3d &size) noexcept
	{
		hitbox = aabb::Hitbox3d(pos, size);
	}

public:
	virtual void update(float fElapsedTime) noexcept
	{
	}
};
