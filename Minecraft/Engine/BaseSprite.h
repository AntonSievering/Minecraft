#pragma once

#include "TextureId.h"

namespace Engine
{
	template <class ImageType, class ImageSizeType, GLenum target>
	class BaseSprite
	{
	protected:
		ImageType         m_image{};
		TextureId<target> m_textureId{};

	public:
		BaseSprite() noexcept = default;

	protected:
		virtual void internalUploadData() const noexcept = 0;

		void defaultTextureParameters(const GLint nMinFilter, const GLint nMagFilter) const noexcept
		{
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, nMinFilter);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, nMagFilter);
			glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		void uploadData(const GLint nMinFilter, const GLint nMagFilter) noexcept
		{
			m_textureId.create();
			m_textureId.bind();
			defaultTextureParameters(nMinFilter, nMagFilter);
			internalUploadData();
			m_textureId.unbind();
		}

	public:
		virtual Pixel *data() const noexcept = 0;
		virtual ImageSizeType size() const noexcept = 0;
		virtual Pixel getPixel(const ImageSizeType pixel) const noexcept = 0;

		ImageType getImage() const noexcept
		{
			return m_image;
		}

		TextureId<target> getTextureId() const noexcept
		{
			return m_textureId;
		}

	public:
		void reupload() const noexcept
		{
			m_textureId.bind();
			internalUploadData();
			m_textureId.unbind();
		}

		void bind() const noexcept
		{
			m_textureId.bind();
		}

		static void unbind() noexcept
		{
			m_textureId.unbind();
		}

		bool isValid() const noexcept
		{
			return m_textureId != 0;
		}
	};
}
