#pragma once

#include "Engine/vec3d.h"

namespace aabb
{
	struct Hitbox2d
	{
		Engine::vf2d pos{}, size{};

	public:
		constexpr Hitbox2d() noexcept = default;

		constexpr Hitbox2d(const Engine::vf2d pos, const Engine::vf2d size) noexcept
		{
			this->pos  = pos;
			this->size = size;
		}

	public:
		constexpr bool collides(const Engine::vf2d point) const noexcept
		{
			return point.x > pos.x && point.x < pos.x + size.x
				&& point.y > pos.y && point.y < pos.y + size.y;
		}

		constexpr bool collides(const Hitbox2d &other) const noexcept
		{
			return pos.x < other.pos.x + other.size.x && pos.x + size.x > other.pos.x
				&& pos.y < other.pos.y + other.size.y && pos.y + size.y > other.pos.y;
		}

		constexpr Engine::vf2d getOverlap(const Hitbox2d &other) const noexcept
		{
			auto abs_min = [](const float f1, const float f2)
			{
				return (std::abs(f1) < std::abs(f2)) ? f1 : f2;
			};

			const Engine::vf2d offset1 = (pos + size) - other.pos;
			const Engine::vf2d offset2 = pos - (other.pos + other.size);

			const float ox = abs_min(offset1.x, offset2.x);
			const float oy = abs_min(offset1.y, offset2.y);

			return Engine::vf2d(ox, oy);
		}
	};

	struct Hitbox3d
	{
		Engine::vf3d pos{}, size{};

	public:
		constexpr Hitbox3d() noexcept = default;

		constexpr Hitbox3d(const Engine::vf3d pos, const Engine::vf3d size) noexcept
		{
			this->pos = pos;
			this->size = size;
		}

	public:
		constexpr bool collides(const Engine::vf3d point) const noexcept
		{
			return point.x > pos.x && point.x < pos.x + 1.0f
				&& point.y > pos.y && point.y < pos.y + 1.0f
				&& point.z > pos.z && point.z < pos.z + 1.0f;
		}

		constexpr bool collides(const Hitbox3d & other) const noexcept
		{
			return pos.x < other.pos.x + other.size.x && pos.x + size.x > other.pos.x
				&& pos.y < other.pos.y + other.size.y && pos.y + size.y > other.pos.y
				&& pos.z < other.pos.z + other.size.z && pos.z + size.z > other.pos.z;
		}

		constexpr Engine::vf3d getOverlap(const Hitbox3d &other) const noexcept
		{
			auto abs_min = [](const float f1, const float f2)
			{
				return (std::abs(f1) < std::abs(f2)) ? f1 : f2;
			};

			const Engine::vf3d offset1 = (pos + size) - other.pos;
			const Engine::vf3d offset2 = pos - (other.pos + other.size);

			const float ox = abs_min(offset1.x, offset2.x);
			const float oy = abs_min(offset1.y, offset2.y);
			const float oz = abs_min(offset1.z, offset2.z);

			return Engine::vf3d(ox, oy, oz);
		}
	};
}
