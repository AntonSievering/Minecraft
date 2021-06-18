#pragma once

#define NOMINMAX
#include <cstdint>
#include <cmath>
#include <functional>

namespace Engine
{
	template <class T>
	struct vec2d
	{
	public:
		T x = T();
		T y = T();

	public:
		constexpr inline vec2d() {                                                   }
		constexpr inline vec2d(const T _x, const T _y) { x = _x; y = _y; }

	public:
		constexpr inline T      mag()                          const noexcept { return std::sqrt(x * x + y * y); }
		constexpr inline T      mag2()                         const noexcept { return           x * x + y * y; }

		constexpr inline vec2d  normalize()                    const noexcept { T r = 1 / mag(); return vec2d(x * r, y * r); }
		constexpr inline vec2d  perp()                         const noexcept { return vec2d(-y, x); }

		constexpr inline T      dot(const vec2d &rhs)        const noexcept { return this->x * rhs.x + this->y * rhs.y; }
		constexpr inline T      cross(const vec2d &rhs)        const noexcept { return vec2d(this->x * rhs.y - this->y * rhs.x); }

		constexpr inline vec2d  flip()                         const noexcept { return vec2d(y, x); }
		constexpr inline vec2d  rotate(const float &fAngle)    const noexcept { return vec2d(std::cos(fAngle) * x - std::sin(fAngle) * y, std::sin(fAngle) * x + std::cos(fAngle) * y); }
		constexpr inline vec2d  inv()                          const noexcept { return vec2d(1.0f / x, 1.0f / y); }

	public:
		constexpr inline vec2d  operator +  (const vec2d &rhs) const noexcept { return vec2d(this->x + rhs.x, this->y + rhs.y); }
		constexpr inline vec2d  operator -  (const vec2d &rhs) const noexcept { return vec2d(this->x - rhs.x, this->y - rhs.y); }
		constexpr inline vec2d  operator *  (const T &rhs) const noexcept { return vec2d(this->x * rhs, this->y * rhs); }
		constexpr inline vec2d  operator *  (const vec2d &rhs) const noexcept { return vec2d(this->x * rhs.x, this->y * rhs.y); }
		constexpr inline vec2d  operator /  (const T &rhs) const noexcept { return vec2d(this->x / rhs, this->y / rhs); }
		constexpr inline vec2d  operator /  (const vec2d &rhs) const noexcept { return vec2d(this->x / rhs.x, this->y / rhs.y); }
		constexpr inline vec2d  operator %  (const T &rhs) const noexcept { return vec2d(fmod(x, rhs), fmod(y, rhs)); }
		constexpr inline vec2d  operator %  (const vec2d &rhs) const noexcept { return vec2d(fmod(x, rhs.x), fmod(y, rhs.y)); }

		constexpr inline vec2d &operator += (const vec2d &rhs)       noexcept { this->x += rhs.x; this->y += rhs.y; return *this; }
		constexpr inline vec2d &operator -= (const vec2d &rhs)       noexcept { this->x -= rhs.x; this->y -= rhs.y; return *this; }
		constexpr inline vec2d &operator *= (const T &rhs)       noexcept { this->x *= rhs;   this->y *= rhs;   return *this; }
		constexpr inline vec2d &operator /= (const T &rhs)       noexcept { this->x /= rhs;   this->y /= rhs;   return *this; }

	public:
		constexpr inline bool   operator == (const vec2d &rhs) const noexcept { return rhs.x == x && rhs.y == y; }
		constexpr inline bool   operator != (const vec2d &rhs) const noexcept { return !operator==(rhs); }

	public:
		constexpr inline vec2d operator+()                     const noexcept { return vec2d<T>(+x, +y); }
		constexpr inline vec2d operator-()                     const noexcept { return vec2d<T>(-x, -y); }

	public:
		template <class _T>
		constexpr inline operator vec2d<_T>()                  const noexcept { return { static_cast<_T>(this->x), static_cast<_T>(this->y) }; }
	};

	using vi2d = vec2d<int32_t>;
	using vu2d = vec2d<uint32_t>;
	using vf2d = vec2d<float>;
	using vd2d = vec2d<double>;
}
