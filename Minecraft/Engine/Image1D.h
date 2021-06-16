#pragma once

#include "BaseImage.h"
#include "Array1D.h"

namespace Engine
{
	class Image1D : public BaseImage<Array1D<Pixel>, uint32_t>
	{
	public:
		Image1D() noexcept = default;

		Image1D(const uint32_t size) noexcept
		{
			m_data = Array1D<Pixel>(size);
		}

	public:
		Pixel *data() const noexcept override
		{
			return m_data.data();
		}

		uint32_t size() const noexcept override
		{
			return m_data.size();
		}

		Pixel getPixel(const uint32_t index) const noexcept override
		{
			return m_data.at(index);
		}

		void setPixel(const uint32_t index, const Pixel p) noexcept override
		{
			m_data.at(index) = p;
		}
	};
}
