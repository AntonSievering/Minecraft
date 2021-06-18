#pragma once

#include <cstdint>
#include "Engine/Array3D.h"
#include "Block.h"
#include "Engine/VertexBuffer.h"
#include "Engine/IndexBuffer.h"
#include "FaceTemplate.h"
#include "ChunkDefines.h"
#include "BlockShader.h"

class Chunk
{
private:
	Engine::Array3D<Block> m_blocks{};
	Engine::vu3d m_vBaseCoordinate{};
	bool m_bDataLoaded   = false;
	bool m_bMeshBuilt    = false;
	bool m_bMeshUploaded = false;

public:
	std::vector<uint32_t> m_vIndices;
	std::vector<Engine::MinecraftVertex> m_vVertices;
	Engine::MinecraftVertexbuffer m_vertices{};
	Engine::IndexBuffer<uint32_t> m_indices{};

public:
	Chunk() noexcept = default;

	Chunk(const Engine::vu3d baseCoordinate) noexcept
	{
		m_vBaseCoordinate = baseCoordinate;
		m_blocks = Engine::Array3D<Block>(Engine::vu3d(g_nChunkWidth, g_nChunkHeight, g_nChunkWidth));
		m_vIndices.reserve(100000);
		m_vVertices.reserve(100000);
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
			&&index.y < g_nChunkHeight
			&&index.z < g_nChunkWidth;
	}

	bool isDataLoaded() const noexcept
	{
		return m_bDataLoaded;
	}

	void setDataLoaded() noexcept
	{
		m_bDataLoaded = true;
	}

	bool isMeshBuilt() const noexcept
	{
		return m_bMeshBuilt;
	}

	bool isMeshUploaded() const noexcept
	{
		return m_bMeshUploaded;
	}

	void buildMesh(const Chunk &north, const Chunk &south, const Chunk &east, const Chunk &west) noexcept
	{
		m_vIndices.clear();
		m_vVertices.clear();

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
					Engine::vu3d coordinate = { x, y, z };

					if (nBlockId > 0)
					{
						Block blockNorth = (x != 15) ? getBlock({ x + 1, y, z }) : north.getBlock({ 0, y, z });
						if (faceNeeded(blockNorth))
							FaceTemplate::FullBlock::fillNorth(m_vVertices, m_vIndices, coordinate, 0);

						Block blockSouth = (x != 0) ? getBlock({ x - 1, y, z }) : south.getBlock({ 15, y, z });
						if (faceNeeded(blockSouth))
							FaceTemplate::FullBlock::fillSouth(m_vVertices, m_vIndices, coordinate, 0);

						Block blockEast = (z != 15) ? getBlock({ x, y, z + 1 }) : east.getBlock({ x, y, 0 });
						if (faceNeeded(blockEast))
							FaceTemplate::FullBlock::fillEast(m_vVertices, m_vIndices, coordinate, 0);

						Block blockWest = (z != 0) ? getBlock({ x, y, z - 1 }) : west.getBlock({ x, y, 15 });
						if (faceNeeded(blockWest))
							FaceTemplate::FullBlock::fillWest(m_vVertices, m_vIndices, coordinate, 0);

						Block blockTop = (y != 255) ? getBlock({ x, y + 1, z }) : Block(0);
						if (faceNeeded(blockTop))
							FaceTemplate::FullBlock::fillTop(m_vVertices, m_vIndices, coordinate, 0);

						Block blockBottom = (y != 0) ? getBlock({ x, y - 1, z }) : Block(1, 0, 0);
						if (faceNeeded(blockBottom))
							FaceTemplate::FullBlock::fillBottom(m_vVertices, m_vIndices, coordinate, 0);
					}
				}
			}
		}

		m_bMeshBuilt = true;
	}

	void uploadData() noexcept
	{
		m_vertices = Engine::MinecraftVertexbuffer(m_vVertices);
		m_indices  = Engine::IndexBuffer<uint32_t>(m_vIndices);
		m_bMeshUploaded = true;
	}

	void renderOpaque(const BlockShader &shader) const noexcept
	{
		shader.bind();
		shader.setChunkBaseCoordinate(m_vBaseCoordinate);
		m_vertices.bind();
		m_indices.render();
	}
};
