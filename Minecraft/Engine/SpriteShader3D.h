#pragma once

#include "Shader.h"
#include "deps/glm/glm.hpp"


namespace Engine
{
	class SpriteShader3D
	{
	private:
		Shader m_shader;
		int m_nMVPLocation = -1;

	public:
		SpriteShader3D() noexcept = default;

		SpriteShader3D(const std::string &sPath) noexcept
		{
			m_shader = Shader(sPath);
			
			bind();
			m_nMVPLocation = m_shader.getUniformLocation("u_modelViewProj");
			glUniform1i(m_shader.getUniformLocation("u_texture"), 0);
			unbind();
		}

	public:
		void bind() const noexcept
		{
			m_shader.bind();
		}

		static void unbind() noexcept
		{
			Shader::unbind();
		}

		void setMVP(const glm::mat4 &mvp) const noexcept
		{
			bind();
			glUniformMatrix4fv(m_nMVPLocation, 1, GL_FALSE, &mvp[0][0]);
		}
	};
}
