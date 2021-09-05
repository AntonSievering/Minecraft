#pragma once

#include "BaseImage.h"
#include "Array2D.h"
#include "vec2d.h"
#include <stb_image.h>
#include <stb_image_write.h>

#include <string>

namespace Engine
{
	class Image2D : public BaseImage<Array2D<Pixel>, vu2d>
	{
	public:
		Image2D() noexcept = default;

		Image2D(const std::string &sFilename) noexcept
		{
			int32_t nBitsPerPixel{}, w{}, h{};
			stbi_set_flip_vertically_on_load(true);
			Pixel *pData = (Pixel *)stbi_load(sFilename.c_str(), &w, &h, &nBitsPerPixel, 4);

			m_data = Array2D<Pixel>(pData, vu2d(std::max(w, 0), std::max(h, 0)));
		}

		Image2D(const vu2d size) noexcept
		{
			m_data = Array2D<Pixel>(size);
		}

		Image2D(Pixel *data, const vu2d size) noexcept
		{
			m_data = Array2D<Pixel>(data, size);
		}

	public:
		Pixel *data() const noexcept override
		{
			return m_data.data();
		}

		vu2d size() const noexcept override
		{
			return m_data.size();
		}

		uint32_t getWidth() const noexcept
		{
			return size().x;
		}

		uint32_t getHeight() const noexcept
		{
			return size().y;
		}

		Pixel getPixel(const vu2d index) const noexcept override
		{
			return m_data.at(index);
		}

		void setPixel(const vu2d index, const Pixel p) noexcept override
		{
			m_data.at(index) = p;
		}

	public:
		void save(const std::string &sFilename) const noexcept
		{
			stbi_flip_vertically_on_write(true);
			stbi_write_png(sFilename.c_str(), getWidth(), getHeight(), 4, data(), 4 * getWidth());
		}
	};
}
