#pragma once

#include "Engine/Shader.h"

class BlockShader
{
private:
	Engine::Shader m_shader{};
	int m_nMVPLocation = -1;

public:
	BlockShader() noexcept = default;

	BlockShader(const std::string &sPath) noexcept
	{
		m_shader = Engine::Shader(sPath);
		m_nMVPLocation = m_shader.getUniformLocation("u_modelViewProj");
		glUniform1i(m_shader.getUniformLocation("u_texture"), 0);
	}

public:
	void setModelViewProjectionMatrix(const glm::mat4 &mvp) const noexcept
	{
		m_shader.bind();
		glUniformMatrix4fv(m_nMVPLocation, 1, GL_FALSE, &mvp[0][0]);
	}

	void bind() const noexcept
	{
		m_shader.bind();
	}

	static void unbind() noexcept
	{
		Engine::Shader::unbind();
	}
};
