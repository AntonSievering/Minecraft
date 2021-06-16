#pragma once

#include "VertexBuffer.h"
#include "SpriteShader.h"
#include "TransformedView.h"
#include <string>
#include <vector>

namespace Engine
{
	class StringDecal
	{
		VertexbufferSprite2D m_vertexBuffer{};

	public:
		StringDecal() noexcept = default;

		StringDecal(const std::string &str, const Engine::vf2d &pos, const float &fScale) noexcept
		{
			setString(str, pos, fScale);
		}

	public:
		void setString(const std::string &sText, const Engine::vf2d &pos, const float &fScale) noexcept
		{
			std::vector<Vertex2D> vVertices;

			int i = 0;
			int xOffset = 0;
			int yOffset = 0;
			const float fBlockSizeX = fScale * 0.01f;
			const float fBlockSizeY = fScale * 0.02f;

			const float fTxOffset = 1.0f / 16.0f;

			for (const char &idx : sText)
			{
				// block position in the ascii sheet
				const int x = idx % 16;
				const int y = idx / 16;

				// top left position on the screen
				const vf2d pos_rel = pos + vf2d((float)xOffset * fBlockSizeX, (float)yOffset * fBlockSizeY);

				// position on the ascii sheet
				const float fFromX = (float)x / 16.0f;
				const float fToX = fFromX + fTxOffset;
				const float fFromY = 1.0f - (float)y / 16.0f;
				const float fToY = fFromY - fTxOffset;

				// set the position on the screen and the texture coordinates
				vVertices.push_back({ pos_rel.x,               pos_rel.y + fBlockSizeY, fFromX, fFromY });
				vVertices.push_back({ pos_rel.x,               pos_rel.y,               fFromX, fToY   });
				vVertices.push_back({ pos_rel.x + fBlockSizeX, pos_rel.y,               fToX,   fToY   });
				vVertices.push_back({ pos_rel.x,               pos_rel.y + fBlockSizeY, fFromX, fFromY });
				vVertices.push_back({ pos_rel.x + fBlockSizeX, pos_rel.y,               fToX,   fToY   });
				vVertices.push_back({ pos_rel.x + fBlockSizeX, pos_rel.y + fBlockSizeY, fToX,   fFromY });

				if (idx == '\n')
				{
					xOffset = 0;
					yOffset++;
				}
				else
				{
					xOffset++;
				}
				i++;
			}

			m_vertexBuffer = VertexbufferSprite2D(vVertices.data(), vVertices.size());
		}

		void render(const SpriteShader shader, const Pixel &col) const noexcept
		{
			shader.setColour(col);
			shader.resetTranform();
			m_vertexBuffer.render();
		}

		void renderWorld(const SpriteShader &shader, const TransformedView &tv, const Pixel &col) const noexcept
		{
			shader.setColour(col);
			shader.setTransform(tv.getPanOffset(), tv.getZoom());
			m_vertexBuffer.render();
		}

		static vf2d getStringSize(const std::string &sText) noexcept
		{
			uint32_t nLineFeeds = 1;
			uint32_t nLongest = 0;
			uint32_t nCurrent = 0;

			for (const char &c : sText)
			{
				if (c == '\n')
				{
					nLineFeeds++;

					if (nCurrent > nLongest)
						nLongest = nCurrent;
				}
				else
					nCurrent++;
			}

			if (nCurrent > nLongest)
				nLongest = nCurrent;

			return { 0.01f * nLongest, 0.02f * nLineFeeds };
		}
	};
}
