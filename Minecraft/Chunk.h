#pragma once

#include <cstdint>
#include "Engine/Array3D.h"
#include "Block.h"
#include "Engine/VertexBuffer.h"
#include "FaceTemplate.h"

constexpr size_t g_nChunkWidth = 16;
constexpr size_t g_nChunkHeight = 256;

class Chunk
{
private:
	Engine::Array3D<Block> m_blocks{};

public:
	Engine::MinecraftVertexbuffer m_vertices{};

public:
	Chunk() noexcept
	{
		m_blocks = Engine::Array3D<Block>(Engine::vu3d(g_nChunkWidth, g_nChunkHeight, g_nChunkWidth));
	}

public:
	Block getBlock(const Engine::vu3d index) const noexcept
	{
		return m_blocks.at(index);
	}

	void setBlock(const Engine::vu3d index, const Block block) noexcept
	{
		m_blocks.at(index) = block;
	}

	static bool indexExists(const Engine::vu3d index) noexcept
	{
		return index.x < g_nChunkWidth
			&& index.y < g_nChunkHeight
			&& index.z < g_nChunkWidth;
	}

	void buildMesh(const uint32_t nSlots, const Chunk &north, const Chunk &south, const Chunk &east, const Chunk &west) noexcept
	{
		std::vector<Engine::MinecraftVertex> vVertices{};

		auto faceNeeded = [](const Block block) -> bool
		{
			return block.getId() == 0;
		};

		for (uint32_t x = 0; x < g_nChunkWidth; x++)
		{
			for (uint32_t y = 0; y < g_nChunkHeight; y++)
			{
				for (uint32_t z = 0; z < g_nChunkWidth; z++)
				{
					uint16_t nBlockId = getBlock({ x, y, z }).getId();
					Engine::vf3d coordinate = Engine::vf3d((float)x, (float)y, (float)z);

					if (nBlockId > 0)
					{
						Block blockNorth = (x != 15) ? getBlock({ x + 1, y, z }) : north.getBlock({ 0, y, z });
						if (faceNeeded(blockNorth))
							FaceTemplate::FullBlock::fillNorth(vVertices, coordinate, nSlots, 0);

						Block blockSouth = (x != 0) ? getBlock({ x - 1, y, z }) : south.getBlock({ 15, y, z });
						if (faceNeeded(blockSouth))
							FaceTemplate::FullBlock::fillSouth(vVertices, coordinate, nSlots, 0);

						Block blockEast = (z != 15) ? getBlock({ x, y, z + 1 }) : east.getBlock({ x, y, 0 });
						if (faceNeeded(blockEast))
							FaceTemplate::FullBlock::fillEast(vVertices, coordinate, nSlots, 0);

						Block blockWest = (z != 0) ? getBlock({ x, y, z - 1 }) : west.getBlock({ x, y, 15 });
						if (faceNeeded(blockWest))
							FaceTemplate::FullBlock::fillWest(vVertices, coordinate, nSlots, 0);

						Block blockTop = (y != 255) ? getBlock({ x, y + 1, z }) : Block(0);
						if (faceNeeded(blockTop))
							FaceTemplate::FullBlock::fillTop(vVertices, coordinate, nSlots, 0);

						Block blockBottom = (y != 0) ? getBlock({ x, y - 1, z }) : Block(0);
						if (faceNeeded(blockBottom))
							FaceTemplate::FullBlock::fillBottom(vVertices, coordinate, nSlots, 0);
					}
				}
			}
		}

		m_vertices = Engine::MinecraftVertexbuffer(vVertices.data(), vVertices.size());
	}
};
