#pragma once

#include <cstdint>
#include <memory>

#ifdef _DEBUG
#define _ARRAY_1D_DEBUG_
#endif // _DEBUG

#ifndef _ARRAY_1D_NO_ALLOCATION_CHECK_
#define _ARRAY_1D_ALLOCATION_CHECK_
#endif // _ARRAY_1D_NO_ALLOCATION_CHECK_

template <class T>
class Array1D
{
public:
	struct out_of_memory
	{
		size_t nMissingBytes{};
	};

	struct out_of_bounds
	{
		size_t nMaximum{};
		size_t nGiven{};
	};

private:
	std::shared_ptr<T> m_pData = nullptr;
	size_t             m_nSize = 0;

public:
	Array1D() noexcept = default;

	Array1D(const size_t nLength) noexcept(false)
	{
		m_pData = std::shared_ptr<T>(new T[nLength]{});
		m_nSize = nLength;

#ifdef _ARRAY_1D_ALLOCATION_CHECK_
		if (m_pData.get() == nullptr)
			throw out_of_memory{ nLength * sizeof(T) };
#endif
	}

	Array1D(T *data, const size_t nLength) noexcept
	{
		m_pData = std::shared_ptr<T>(data);
		m_nSize = nLength;
	}

public:
	T *data() noexcept
	{
		return m_pData.get();
	}

	T *data() const noexcept
	{
		return m_pData.get();
	}

	size_t size() const noexcept
	{
		return m_nSize;
	}

	T &operator[](const size_t index) noexcept(false)
	{
#ifdef _ARRAY_1D_DEBUG_
		if (index < m_nSize)
			return data()[index];
		throw out_of_bounds{ m_nSize, index };
#endif
		return data()[index];
	}

	T &at(const size_t index) noexcept
	{
		return operator[](index);
	}

	T operator[](const size_t index) const noexcept(false)
	{
#ifdef _ARRAY_1D_DEBUG_
		if (index < m_nSize)
			return data()[index];
		throw out_of_bounds{ m_nSize, index };
#endif
		return data()[index];
	}

	T at(const size_t index) const noexcept
	{
		return operator[](index);
	}
};
