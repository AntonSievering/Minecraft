#pragma once

#include "Pixel.h"

namespace Engine
{
	template <class ArrayType, class ArraySizeType>
	class BaseImage
	{
	protected:
		ArrayType m_data{};

	public:
		BaseImage() noexcept {}

	public:
		virtual Pixel *data() const noexcept = 0;
		virtual ArraySizeType size() const noexcept = 0;
		virtual Pixel getPixel(const ArraySizeType index) const noexcept = 0;
		virtual void setPixel(const ArraySizeType index, const Pixel p) noexcept = 0;
	};
}
