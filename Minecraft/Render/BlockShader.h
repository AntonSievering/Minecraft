#pragma once

#include "Engine/Shader.h"

class BlockShader
{
private:
	Engine::Shader m_shader{};
	int m_nMVPLocation = -1;
	int m_nChunkCoordinateLocation = -1;
	int m_nTextureHeightLocation = -1;

public:
	BlockShader() noexcept = default;

	BlockShader(const std::string &sPath) noexcept
	{
		m_shader = Engine::Shader(sPath);
		m_nMVPLocation = m_shader.getUniformLocation("u_modelViewProj");
		m_nChunkCoordinateLocation = m_shader.getUniformLocation("u_chunkBaseCoord");
		m_nTextureHeightLocation = m_shader.getUniformLocation("u_fTextureHeight");
		glUniform1i(m_shader.getUniformLocation("u_texture"), 0);
		m_shader.unbind();
	}

public:
	void setModelViewProjectionMatrix(const glm::mat4 &mvp) const noexcept
	{
		m_shader.bind();
		glUniformMatrix4fv(m_nMVPLocation, 1, GL_FALSE, &mvp[0][0]);
	}

	void setChunkBaseCoordinate(const Engine::vi3d coordinate) const noexcept
	{
		m_shader.bind();
		glUniform3f(m_nChunkCoordinateLocation, (float)coordinate.x, (float)coordinate.y, (float)coordinate.z);
	}

	void setTextureHeight(const uint32_t nHeight) const noexcept
	{
		m_shader.bind();
		glUniform1f(m_nTextureHeightLocation, (float)nHeight);
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
