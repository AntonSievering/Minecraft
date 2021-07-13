#pragma once
#include <cstdint>
#include <cmath>
#include "vec2d.h"
#include "deps/glm/glm.hpp"

namespace Engine
{
	template <class T>
	struct vec3d
	{
	public:
		T x = T();
		T y = T();
		T z = T();

	public:
		constexpr inline vec3d()                                     noexcept { x = T(); y = T(); z = T(); }
		constexpr inline vec3d(const T _x, const T _y, const T _z)   noexcept { x =  _x; y =  _y; z =  _z; }
		constexpr inline vec3d(const T xyz)                          noexcept { x = xyz; y = xyz; z = xyz; }
		constexpr inline vec3d(const glm::vec3 &v)                   noexcept { x = v.x; y = v.y; z = v.z; }

	public:
		constexpr inline T      mag()                          const noexcept { return std::sqrt(x * x + y * y + z * z); }
		constexpr inline T      mag2()                         const noexcept { return           x * x + y * y + z * z; }

		constexpr inline vec3d  norm()                         const noexcept { T r = T(1) / mag(); return vec3d(x * r, y * r, z * r); }
		constexpr inline vec3d  normalize()                          noexcept { T r = T(1) / mag(); x *= r; y *= r; z *= r; return *this; }

		constexpr inline T      dot(const vec3d &rhs)          const noexcept { return x * rhs.x + y * rhs.y + z * rhs.z; }
		constexpr inline vec3d  cross(const vec3d &rhs)        const noexcept { return vec3d(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x); }

	public:
		constexpr inline vec2d<T>  xy()                        const noexcept { return vec2d<T>(x, y); }
		constexpr inline vec2d<T>  yz()                        const noexcept { return vec2d<T>(y, z); }
		constexpr inline vec2d<T>  xz()                        const noexcept { return vec2d<T>(x, z); }

	public:
		constexpr inline vec3d  operator +  (const vec3d &rhs) const noexcept { return vec3d(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z); }
		constexpr inline vec3d  operator -  (const vec3d &rhs) const noexcept { return vec3d(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z); }
		constexpr inline vec3d  operator *  (const T     &rhs) const noexcept { return vec3d(this->x * rhs, this->y * rhs, this->z * rhs); }
		constexpr inline vec3d  operator *  (const vec3d &rhs) const noexcept { return vec3d(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z); }
		constexpr inline vec3d  operator /  (const T     &rhs) const noexcept { return vec3d(this->x / rhs, this->y / rhs, this->z / rhs); }
		constexpr inline vec3d  operator /  (const vec3d &rhs) const noexcept { return vec3d(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z); }

		constexpr inline vec3d &operator += (const vec3d &rhs)       noexcept { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; return *this; }
		constexpr inline vec3d &operator -= (const vec3d &rhs)       noexcept { this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z; return *this; }
		constexpr inline vec3d &operator *= (const T     &rhs)       noexcept { this->x *= rhs;   this->y *= rhs;   this->z *= rhs;   return *this; }
		constexpr inline vec3d &operator /= (const T     &rhs)       noexcept { this->x /= rhs;   this->y /= rhs;   this->z /= rhs;   return *this; }

	public:
		constexpr inline bool   operator == (const vec3d &rhs) const noexcept { return rhs.x == x && rhs.y == y && rhs.z == z; }
		constexpr inline bool   operator != (const vec3d &rhs) const noexcept { return !operator==(rhs); }

	public:
		constexpr inline vec3d operator+()                     const noexcept { return *this; }
		constexpr inline vec3d operator-()                     const noexcept { return vec3d<T>(-x, -y, -z); }

	public:
		template <class _T>
		constexpr inline operator vec3d<_T>()                  const noexcept { return { static_cast<_T>(this->x), static_cast<_T>(this->y), static_cast<_T>(this->z) }; }
		constexpr inline operator glm::vec3()                  const noexcept { return glm::vec3(x, y, z); }

#ifndef NO_OSTREAM_OVERLOAD
	public:
		friend std::ostream &operator<<(std::ostream &os, const vec3d value) noexcept
		{
			os << value.x << " " << value.y << " " << value.z;
			return os;
		}
#endif // NO_OSTREAM_OVERLOAD
	};

	using vs3d = vec3d<short>;
	using vi3d = vec3d<int32_t>;
	using vu3d = vec3d<uint32_t>;
	using vf3d = vec3d<float>;
	using vd3d = vec3d<double>;
}

namespace std
{
	template <class T>
	static constexpr inline Engine::vec3d<T> max(const Engine::vec3d<T> lhs, const T rhs) noexcept
	{
		return Engine::vec3d<T>(std::max(lhs.x, rhs), std::max(lhs.y, rhs), std::max(lhs.z, rhs));
	}

	template <class T>
	static constexpr inline Engine::vec3d<T> max(const Engine::vec3d<T> lhs, const Engine::vec3d<T> rhs) noexcept
	{
		return Engine::vec3d<T>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
	}

	template <class T>
	static constexpr inline Engine::vec3d<T> min(const Engine::vec3d<T> lhs, const T rhs) noexcept
	{
		return Engine::vec3d<T>(std::min(lhs.x, rhs), std::min(lhs.y, rhs), std::min(lhs.z, rhs));
	}

	template <class T>
	static constexpr inline Engine::vec3d<T> min(const Engine::vec3d<T> lhs, const Engine::vec3d<T> rhs) noexcept
	{
		return Engine::vec3d<T>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
	}

	template <class T>
	static constexpr inline Engine::vec3d<T> floor(const Engine::vec3d<T> vec) noexcept
	{
		return Engine::vec3d<T>(std::floor(vec.x), std::floor(vec.y), std::floor(vec.z));
	}

	template <class T>
	static constexpr inline Engine::vec3d<T> ceil(const Engine::vec3d<T> vec) noexcept
	{
		return Engine::vec3d<T>(std::ceil(vec.x), std::ceil(vec.y), std::ceil(vec.z));
	}

	template <class T>
	static constexpr inline Engine::vec3d<T> round(const Engine::vec3d<T> vec) noexcept
	{
		return Engine::vec3d<T>(std::round(vec.x), std::round(vec.y), std::round(vec.z));
	}

	template <class T>
	static constexpr inline Engine::vec3d<T> abs(const Engine::vec3d<T> vec) noexcept
	{
		return Engine::vec3d<T>(std::abs(vec.x), std::abs(vec.y), std::abs(vec.z));
	}
}
