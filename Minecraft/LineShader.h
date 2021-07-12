#pragma once

#include "Engine/Shader.h"

class LineShader
{
	Engine::Shader m_shader;
	int m_nMvpLocation = -1,
		m_nBaseCoordinateLocation = -1;

public:
	LineShader() noexcept = default;

	LineShader(const std::string &sFilename) noexcept
	{
		m_shader = Engine::Shader(sFilename);
		m_nMvpLocation            = m_shader.getUniformLocation("u_modelViewProj");
		m_nBaseCoordinateLocation = m_shader.getUniformLocation("u_baseCoordinate");
		glUniform4f(m_shader.getUniformLocation("u_color"), 0.0f, 0.0f, 0.0f, 0.75f);
	}

public:
	void update(const Engine::vi3d coord, const glm::mat4 &mvp) noexcept
	{
		m_shader.bind();
		glUniform3f(m_nBaseCoordinateLocation, coord.x, coord.y, coord.z);
		glUniformMatrix4fv(m_nMvpLocation, 1, GL_FALSE, &mvp[0][0]);
	}
};
