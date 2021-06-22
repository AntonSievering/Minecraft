#pragma once

#include "Array1D.h"
#include "vec2d.h"

namespace Engine
{
	template <class T>
	class Array2D
	{
	private:
		Array1D<T> m_array{};
		vu2d m_size{};

	public:
		Array2D() noexcept = default;

		Array2D(const vu2d size) noexcept
		{
			m_size = size;
			m_array = Array1D<T>((size_t)size.x * size.y);
		}

		Array2D(T *data, const vu2d size) noexcept
		{
			m_size = size;
			m_array = Array1D<T>(data, (size_t)size.x * size.y);
		}

	private:
		size_t transform(const size_t x, const size_t y) const noexcept
		{
			return y * m_size.x + x;
		}

	public:
		T *data() noexcept
		{
			return m_array.data();
		}

		T *data() const noexcept
		{
			return m_array.data();
		}

		vu2d size() const noexcept
		{
			return m_size;
		}

		const T &at(const size_t x, const size_t y) const noexcept
		{
			return m_array[transform(x, y)];
		}

		const T &at(const vu2d index) const noexcept
		{
			return at(index.x, index.y);
		}

		T &at(const size_t x, const size_t y) noexcept
		{
			return m_array[transform(x, y)];
		}

		T &at(const vu2d index) noexcept
		{
			return at(index.x, index.y);
		}
	};
}
