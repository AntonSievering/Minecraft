#pragma once

#include "BaseSprite.h"
#include "Image2D.h"
#include "TextureId.h"

namespace Engine
{
	class Sprite2D : public BaseSprite<Image2D, vu2d, GL_TEXTURE_2D>
	{
	public:
		Sprite2D() noexcept = default;

		Sprite2D(const Image2D &image, const GLint nMinFilter = GL_LINEAR, const GLint nMagFilter = GL_NEAREST) noexcept
		{
			m_image = image;
			uploadData(nMinFilter, nMagFilter);
		}

		Sprite2D(const vu2d &size, const GLint nMinFilter = GL_LINEAR, const GLint nMagFilter = GL_NEAREST) noexcept
		{
			m_image = Image2D(size);
			uploadData(nMinFilter, nMagFilter);
		}

		Sprite2D(const std::string &sFilename, const GLint nMinFilter = GL_LINEAR, const GLint nMagFilter = GL_NEAREST) noexcept
		{
			m_image = Image2D(sFilename);
			uploadData(nMinFilter, nMagFilter);
		}

	private:
		void internalUploadData() const noexcept override
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_image.getWidth(), m_image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.data());
		}

	public:
		Pixel *data() const noexcept override
		{
			return m_image.data();
		}

		vu2d size() const noexcept override
		{
			return m_image.size();
		}

		Pixel getPixel(const vu2d pos) const noexcept override
		{
			return m_image.getPixel(pos);
		}

		uint32_t getWidth() const noexcept
		{
			return m_image.getWidth();
		}

		uint32_t getHeight() const noexcept
		{
			return m_image.getHeight();
		}

		float getAspectRatio() const noexcept
		{
			return (float)m_image.getWidth() / (float)m_image.getHeight();
		}
	};
}
