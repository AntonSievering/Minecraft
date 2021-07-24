#pragma once

#include "LineShader.h"
#include "Engine/VertexBuffer.h"
#include "Engine/Camera.h"
#include "Engine/IndexBuffer.h"

class BlockHighlight
{
private:
	Engine::VertexbufferLine vHighlightVertices{};
	Engine::IndexBuffer<uint32_t> vHighlightIndices{};
	LineShader m_shader;

public:
	bool bDrawAllBoundingLines = false;

public:
	BlockHighlight() noexcept = default;

	BlockHighlight(const std::string &sFilename) noexcept
	{
		vHighlightVertices = Engine::VertexbufferLine(createVertices(), 8);

		uint32_t indices[24] = {
			0, 4, 1, 5, 2, 6, 3, 7, // lines facing up
			0, 1, 1, 2, 2, 3, 0, 3, // bottom lines
			4, 5, 5, 6, 6, 7, 4, 7  // top lines
		};
		vHighlightIndices = Engine::IndexBuffer<uint32_t>(indices, 24);

		m_shader = LineShader(sFilename);
		
		glLineWidth(1.5f);
		glEnable(GL_LINE_SMOOTH);
		glDepthFunc(GL_LEQUAL);
	}

private:
	static Engine::LineVertex *createVertices() noexcept
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

		m_shader.update(coord, camera.getViewProj());
		vHighlightVertices.bind();
		vHighlightIndices.render(GL_LINES);

		if (bDrawAllBoundingLines)
			glEnable(GL_DEPTH_TEST);
	}
};
