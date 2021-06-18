#pragma once

#include "Array1D.h"
#include "vec3d.h"

namespace Engine
{
	template <class T>
	class Array3D
	{
	private:
		Array1D<T> m_array{};
		vu3d       m_size{};

	public:
		Array3D() noexcept = default;

		Array3D(const vu3d size) noexcept
		{
			m_size = size;
			m_array = Array1D<T>((size_t)size.x * size.y * size.z);
		}

		Array3D(T *data, const vu3d size) noexcept
		{
			m_size = size;
			m_array = Array1D<T>((size_t)size.x * size.y * size.z);
		}

	private:
		size_t transform(const size_t x, const size_t y, const size_t z) const noexcept
		{
			return m_size.x * (m_size.y * z + y) + x;
		}

	public:
		T *data() const noexcept
		{
			return m_array.data();
		}

		vu3d size() const noexcept
		{
			return m_size;
		}

		uint32_t getWidth() const noexcept
		{
			return m_size.x;
		}

		uint32_t getHeight() const noexcept
		{
			return m_size.y;
		}

		uint32_t getDepth() const noexcept
		{
			return m_size.z;
		}

		T at(const size_t x, const size_t y, const size_t z) const noexcept
		{
			return m_array[transform(x, y, z)];
		}

		T at(const vu3d index) const noexcept
		{
			return at(index.x, index.y, index.z);
		}

		T &at(const size_t x, const size_t y, const size_t z) noexcept
		{
			return m_array[transform(x, y, z)];
		}

		T &at(const vu3d index) noexcept
		{
			return at(index.x, index.y, index.z);
		}
	};
}
