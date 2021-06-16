#pragma once

#include "Image3D.h"
#include "BaseSprite.h"

namespace Engine
{
	class Sprite3D : public BaseSprite<Image3D, vu3d, GL_TEXTURE_3D>
	{
	public:
		Sprite3D() noexcept = default;

		Sprite3D(const Image3D &image) noexcept
		{
			m_image = image;
			uploadData(GL_NEAREST, GL_NEAREST);
		}

	private:
		void internalUploadData() const noexcept override
		{
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, m_image.size().x, m_image.size().y, m_image.size().z, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.data());
		}

	public:
		Pixel *data() const noexcept override
		{
			return m_image.data();
		}

		vu3d size() const noexcept override
		{
			return m_image.size();
		}

		uint32_t getWidth() const noexcept
		{
			return m_image.getWidth();
		}

		uint32_t getHeight() const noexcept
		{
			return m_image.getHeight();
		}

		uint32_t getDepth() const noexcept
		{
			return m_image.getDepth();
		}

		Pixel getPixel(const vu3d index) const noexcept override
		{
			return m_image.getPixel(index);
		}
	};
}
