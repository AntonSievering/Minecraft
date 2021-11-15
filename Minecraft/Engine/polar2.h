#pragma once

#include "vec2d.h"

namespace Engine
{
	template <class T>
	struct polar2
	{
		T r   = 0.0f;
		T phi = 0.0f;

	public:
		polar2() noexcept = default;

		polar2(const T r, const T phi) noexcept
		{
			this->r = r;
			this->phi = phi;
		}

		template <class Ty>
		explicit polar2(const vec2d<Ty> v)
		{
			fromCartesian(v);
		}

	private:
		template <class Ty>
		void fromCartesian(const Engine::vec2d<Ty> v) noexcept
		{
			r = v.mag();
			phi = std::atan2(v.y, v.x);
		}
	};
}
