#pragma once

#include <GL/glew.h>
#include <memory>
#include "Sprite2D.h"

namespace Engine
{
	class RenderTarget
	{
		struct SharedResource
		{
			GLuint m_nFrameBufferObject{};
			GLuint m_nRenderBufferObject{};

		public:
			SharedResource() noexcept = default;
			SharedResource(const GLuint &fbo, const GLuint &rbo) noexcept
			{
				m_nFrameBufferObject = fbo;
				m_nRenderBufferObject = rbo;
			}
			virtual ~SharedResource() noexcept
			{
				glDeleteFramebuffers(1, &m_nFrameBufferObject);
				glDeleteRenderbuffers(1, &m_nRenderBufferObject);
			}
		};

	private:
		std::shared_ptr<SharedResource> m_shrRenderInfoBuffer;
		Sprite2D                        m_texture;

	public:
		RenderTarget() noexcept = default;

		RenderTarget(const vu2d &size, const GLint &min_filter = GL_LINEAR, const GLint &mag_filter = GL_NEAREST)
		{
			GLuint fbo{}, rbo{};
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			m_texture = Sprite2D(size, min_filter, mag_filter);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.getTextureId(), 0);

			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			m_shrRenderInfoBuffer = std::make_shared<SharedResource>(fbo, rbo);
		}

	public:
		GLuint getFBO() const noexcept
		{
			return m_shrRenderInfoBuffer.get()->m_nFrameBufferObject;
		}

		GLuint getRBO() const noexcept
		{
			return m_shrRenderInfoBuffer.get()->m_nRenderBufferObject;
		}

		void bind() const noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, getFBO());
		}

		static void unbind() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void sync() noexcept
		{
			m_texture.bind();
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture.getImage().data());
		}

		Sprite2D getSprite() noexcept
		{
			return m_texture;
		}

		vu2d size() const noexcept
		{
			return m_texture.size();
		}
	};
}
