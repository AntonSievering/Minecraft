#pragma once

#include <memory>

namespace Engine
{
	namespace math
	{
		template <class To, class From>
		static constexpr inline To bit_cast(const From &value) noexcept
		{
#ifdef __cpp_lib_bit_cast
			return std::bit_cast<To>(value);
#else
			return (To)value;
#endif
		}
	}
}
