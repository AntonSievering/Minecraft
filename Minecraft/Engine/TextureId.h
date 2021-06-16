#pragma once

#include "defines.h"

namespace Engine
{
	template <GLenum target>
	class TextureId
	{
		struct SharedResource
		{
			GLuint m_nTextureId{};

		public:
			SharedResource() noexcept = default;
			SharedResource(GLuint id) noexcept { m_nTextureId = id; }
			virtual ~SharedResource() noexcept { glDeleteTextures(1, &m_nTextureId); }
		};

	private:
		std::shared_ptr<SharedResource> m_shrTextureId;

	public:
		TextureId() noexcept
		{
			m_shrTextureId = std::make_shared<SharedResource>(SharedResource(0));
		}

		TextureId(const GLuint id) noexcept
		{
			setTextureId(id);
		}

	public:
		void create() noexcept
		{
			GLuint textureId{};
			glGenTextures(1, &textureId);
			setTextureId(textureId);
		}

		void setTextureId(const GLuint id) noexcept
		{
			m_shrTextureId = std::make_shared<SharedResource>(SharedResource(id));
		}

		GLuint getTextureId() const noexcept
		{
			return m_shrTextureId.get()->m_nTextureId;
		}

	public:
		operator GLuint() const noexcept
		{
			return getTextureId();
		}

		void bind() const noexcept
		{
			glBindTexture(target, getTextureId());
		}

		static void unbind() noexcept
		{
			glBindTexture(target, 0);
		}
	};
}
