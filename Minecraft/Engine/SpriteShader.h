#pragma once

#include "Shader.h"
#include "Pixel.h"

namespace Engine
{
	class SpriteShader
	{
	private:
		Engine::Shader m_shader{};
		int m_nColourUniformLocation = -1;
		int m_nPanOffsetUniformLocation = -1;
		int m_nZoomUniformLocation = -1;

	public:
		SpriteShader() noexcept = default;

		SpriteShader(const std::string &sPath) noexcept
		{
			m_shader = Engine::Shader(sPath);
			m_nColourUniformLocation = m_shader.getUniformLocation("u_color");
			m_nPanOffsetUniformLocation = m_shader.getUniformLocation("u_panOffset");
			m_nZoomUniformLocation = m_shader.getUniformLocation("u_fZoom");
			glUniform1i(m_shader.getUniformLocation("u_texture"), 0);
		}

	public:
		void setColour(const Pixel &px) const noexcept
		{
			const float r = (float)px.r / 255.0f;
			const float g = (float)px.g / 255.0f;
			const float b = (float)px.b / 255.0f;
			const float a = (float)px.a / 255.0f;
			m_shader.bind();
			glUniform4f(m_nColourUniformLocation, r, g, b, a);
			glActiveTexture(GL_TEXTURE0);
		}

		void setTransform(const vf2d &offset, const float &fZoom) const noexcept
		{
			bind();
			glUniform2f(m_nPanOffsetUniformLocation, offset.x, offset.y);
			glUniform1f(m_nZoomUniformLocation, fZoom);
		}

		void resetTranform() const noexcept
		{
			setTransform({ 0.0f, 0.0f }, 1.0f);
		}

		void bind() const noexcept
		{
			m_shader.bind();
		}

		static void unbind() noexcept
		{
			Shader::unbind();
		}
	};
}
