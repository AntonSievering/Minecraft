#pragma once

#include "BaseImage.h"
#include "Array3D.h"
#include "vec3d.h"
#include "Sprite2D.h"

namespace Engine
{
	class Image3D : public BaseImage<Array3D<Pixel>, vu3d>
	{
	public:
		Image3D() noexcept = default;

		Image3D(const vu3d size) noexcept
		{
			m_data = Array3D<Pixel>(size);
		}

		Image3D(Pixel *data, const vu3d size) noexcept
		{
			m_data = Array3D<Pixel>(data, size);
		}

	public:
		Pixel *data() const noexcept override
		{
			return m_data.data();
		}

		vu3d size() const noexcept override
		{
			return m_data.size();
		}

		uint32_t getWidth() const noexcept
		{
			return m_data.getWidth();
		}

		uint32_t getHeight() const noexcept
		{
			return m_data.getHeight();
		}

		uint32_t getDepth() const noexcept
		{
			return m_data.getDepth();
		}

		Pixel getPixel(const vu3d index) const noexcept override
		{
			return m_data.at(index);
		}

		void setPixel(const vu3d index, const Pixel p) noexcept override
		{
			m_data.at(index) = p;
		}

		void setSlice(const uint32_t zIndex, const Image2D &image) noexcept
		{
			for (uint32_t x = 0; x < image.getWidth(); x++)
				for (uint32_t y = 0; y < image.getHeight(); y++)
					setPixel({ x, y, zIndex }, image.getPixel({ x, y }));
		}
	};
}
