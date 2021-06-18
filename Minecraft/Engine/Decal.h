#pragma once

#include "VertexBuffer.h"
#include "Sprite2D.h"
#include "SpriteShader.h"
#include "TransformedView.h"

namespace Engine
{
	class Decal
	{
	private:
		Sprite2D             m_sprTexture{};
		VertexbufferSprite2D m_vertexBuffer{};

	public:
		Decal() noexcept = default;

		Decal(const Sprite2D &sprTexture, const vf2d pos, const vf2d size) noexcept
		{
			Vertex2D vertices[6] = {
				{ pos.x,          pos.y + size.y, 0.0f, 1.0f },
				{ pos.x,          pos.y,          0.0f, 0.0f },
				{ pos.x + size.x, pos.y,          1.0f, 0.0f },
				{ pos.x,          pos.y + size.y, 0.0f, 1.0f },
				{ pos.x + size.x, pos.y,          1.0f, 0.0f },
				{ pos.x + size.x, pos.y + size.y, 1.0f, 1.0f }
			};

			m_sprTexture = sprTexture;
			m_vertexBuffer = VertexbufferSprite2D(vertices, 6);
		}

		Decal(const Sprite2D &sprTexture, const vf2d tl, const vf2d tr, const vf2d bl, const vf2d br) noexcept
		{
			Vertex2D vertices[6] = {
				{ tl.x, tl.y, 0.0f, 1.0f },
				{ bl.x, bl.y, 0.0f, 0.0f },
				{ br.x, br.y, 1.0f, 0.0f },
				{ tl.x, tl.y, 0.0f, 1.0f },
				{ br.x, br.y, 1.0f, 0.0f },
				{ tr.x, tr.y, 1.0f, 1.0f }
			};

			m_sprTexture = sprTexture;
			m_vertexBuffer = VertexbufferSprite2D(vertices, 6);
		}

		Decal(const Sprite2D &sprTexture, Vertex2D *vertices, const uint32_t &nVertices) noexcept
		{
			m_sprTexture = sprTexture;
			m_vertexBuffer = VertexbufferSprite2D(vertices, nVertices);
		}

	public:
		void render(const SpriteShader &spriteShader, const Pixel &tint = WHITE) const noexcept
		{
			spriteShader.setColour(tint);
			spriteShader.resetTranform();
			m_vertexBuffer.bind();
			m_sprTexture.bind();
			m_vertexBuffer.render();
		}

		void renderWorld(const SpriteShader &spriteShader, const TransformedView &tv, const Pixel &tint = WHITE) const noexcept
		{
			spriteShader.setTransform(tv.getPanOffset(), tv.getZoom());
			spriteShader.setColour(tint);
			m_vertexBuffer.bind();
			m_sprTexture.bind();
			m_vertexBuffer.render();
		}

		Sprite2D getSprite() const noexcept
		{
			return m_sprTexture;
		}
	};
}
