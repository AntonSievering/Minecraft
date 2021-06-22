#pragma once
#include "Engine/Shader.h"
#include "Engine/VertexBuffer.h"
#include "Engine/Camera.h"
#include "Engine/IndexBuffer.h"

class BlockHighlight
{
private:
	Engine::VertexbufferLine vHighlightVertices{};
	Engine::IndexBuffer<uint32_t> vHighlightIndices{};
	Engine::Shader highlightShader{};
	int m_nMvpLocation = -1,
		m_nBaseCoordinateLocation = -1;

public:
	bool bDrawAllBoundingLines = false;

public:
	BlockHighlight() noexcept = default;

	BlockHighlight(const std::string &sFilename) noexcept
	{
		vHighlightVertices = Engine::VertexbufferLine(createVertices({ 8, 100, 8 }), 8);

		uint32_t indices[24] = {
			0, 4, 1, 5, 2, 6, 3, 7, // lines facing up
			0, 1, 1, 2, 2, 3, 0, 3, // bottom lines
			4, 5, 5, 6, 6, 7, 4, 7  // top lines
		};
		vHighlightIndices = Engine::IndexBuffer<uint32_t>(indices, 24);

		highlightShader = Engine::Shader(sFilename);
		m_nMvpLocation            = highlightShader.getUniformLocation("u_modelViewProj");
		m_nBaseCoordinateLocation = highlightShader.getUniformLocation("u_baseCoordinate");
		glUniform4f(highlightShader.getUniformLocation("u_color"), 0.0f, 0.0f, 0.0f, 0.75f);

		glLineWidth(1.5f);
		glEnable(GL_LINE_SMOOTH);
		glDepthFunc(GL_LEQUAL);
	}

private:
	static Engine::LineVertex *createVertices(const Engine::vf3d &coord)
	{
		Engine::LineVertex vertices[8] = {
			{ { 0.0f, 0.0f, 0.0f } },
			{ { 1.0f, 0.0f, 0.0f } },
			{ { 1.0f, 0.0f, 1.0f } },
			{ { 0.0f, 0.0f, 1.0f } },

			{ { 0.0f, 1.0f, 0.0f } },
			{ { 1.0f, 1.0f, 0.0f } },
			{ { 1.0f, 1.0f, 1.0f } },
			{ { 0.0f, 1.0f, 1.0f } }
		};

		return vertices;
	}

public:
	void render(const Engine::Camera &camera, const Engine::vi3d coord) noexcept
	{
		if (bDrawAllBoundingLines)
			glDisable(GL_DEPTH_TEST);

		highlightShader.bind();
		glUniform3f(m_nBaseCoordinateLocation, coord.x, coord.y, coord.z);
		glUniformMatrix4fv(m_nMvpLocation, 1, GL_FALSE, &camera.getViewProj()[0][0]);
		vHighlightVertices.bind();
		vHighlightIndices.render(GL_LINES);

		if (bDrawAllBoundingLines)
			glEnable(GL_DEPTH_TEST);
	}
};
