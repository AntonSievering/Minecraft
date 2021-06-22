#pragma once

#include <cstdint>
#include <array>
#include <vector>
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
	Engine::vi3d m_vBaseCoordinate{};
	bool m_bDataLoaded   = false;
	bool m_bMeshBuilt    = false;
	bool m_bMeshUploaded = false;

public:
	std::array<bool, 16> m_vLayersToLoad{};
	std::array<size_t, 16> m_vLayerVerticesOffset{};
	std::array<size_t, 16> m_vLayerIndicesOffset{};
	std::vector<uint32_t> m_vIndices{};
	std::vector<Engine::MinecraftVertex> m_vVertices{};
	Engine::MinecraftVertexbuffer m_vertices{};
	Engine::IndexBuffer<uint32_t> m_indices{};

public:
	Chunk() noexcept = default;

	Chunk(const Engine::vi3d baseCoordinate) noexcept
	{
		m_vBaseCoordinate = baseCoordinate;
		m_blocks = Engine::Array3D<Block>(Engine::vu3d(g_nChunkWidth, g_nChunkHeight, g_nChunkWidth));

		for (uint16_t i = 0; i < 16; i++)
			m_vLayersToLoad[i] = true;
	}

private:
	void buildLayer(const uint16_t layer, const Chunk &north, const Chunk &south, const Chunk &east, const Chunk &west) noexcept
	{
		auto faceNeeded = [](const Block block) -> bool
		{
			return block.getId() == 0;
		};

		std::vector<Engine::MinecraftVertex> vVertices{};
		std::vector<uint32_t> vIndices{};

		for (uint32_t x = 0; x < g_nChunkWidth; x++)
		{
			for (uint32_t y = 16 * layer; y < 16 * (layer + 1); y++)
			{
				for (uint32_t z = 0; z < g_nChunkWidth; z++)
				{
					uint16_t nBlockId = getBlock({ x, y, z }).getId();
					Engine::vu3d coordinate = { x, y, z };

					if (nBlockId > 0)
					{
						Block blockNorth = (x != 15) ? getBlock({ x + 1, y, z }) : north.getBlock({ 0, y, z });
						if (faceNeeded(blockNorth))
							FaceTemplate::FullBlock::fillNorth(vVertices, vIndices, coordinate, 0);

						Block blockSouth = (x != 0) ? getBlock({ x - 1, y, z }) : south.getBlock({ 15, y, z });
						if (faceNeeded(blockSouth))
							FaceTemplate::FullBlock::fillSouth(vVertices, vIndices, coordinate, 0);

						Block blockEast = (z != 15) ? getBlock({ x, y, z + 1 }) : east.getBlock({ x, y, 0 });
						if (faceNeeded(blockEast))
							FaceTemplate::FullBlock::fillEast(vVertices, vIndices, coordinate, 0);

						Block blockWest = (z != 0) ? getBlock({ x, y, z - 1 }) : west.getBlock({ x, y, 15 });
						if (faceNeeded(blockWest))
							FaceTemplate::FullBlock::fillWest(vVertices, vIndices, coordinate, 0);

						Block blockTop = (y != 255) ? getBlock({ x, y + 1, z }) : Block(0);
						if (faceNeeded(blockTop))
							FaceTemplate::FullBlock::fillTop(vVertices, vIndices, coordinate, 0);

						Block blockBottom = (y != 0) ? getBlock({ x, y - 1, z }) : Block(1, 0, 0);
						if (faceNeeded(blockBottom))
							FaceTemplate::FullBlock::fillBottom(vVertices, vIndices, coordinate, 0);
					}
				}
			}
		}

		size_t nVerticesStart = 0, nIndicesStart = 0;
		for (int32_t i = 0; i < layer; i++)
		{
			nVerticesStart += m_vLayerVerticesOffset[i];
			nIndicesStart += m_vLayerIndicesOffset[i];
		}

		size_t nVerticesEnd = nVerticesStart + m_vLayerVerticesOffset[layer];
		size_t nIndicesEnd = nIndicesStart + m_vLayerIndicesOffset[layer];

		if (nVerticesEnd != nVerticesStart)
			m_vVertices.erase(m_vVertices.begin() + nVerticesStart, vVertices.begin() + nVerticesEnd);
		if (nIndicesEnd != nIndicesStart)
			m_vIndices.erase(m_vIndices.begin() + nIndicesStart, vIndices.begin() + nIndicesEnd);

		m_vVertices.insert(m_vVertices.end(), vVertices.begin(), vVertices.end());
		m_vIndices.insert(m_vIndices.end(), vIndices.begin(), vIndices.end());

		m_vLayerVerticesOffset[layer] = vVertices.size();
		m_vLayerIndicesOffset[layer] = vIndices.size();
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
		Engine::Timer timer = Engine::Timer().start();
		uint16_t nLayerCount = 0;

		for (uint16_t i = 0; i < 16; i++)
		{
			if (m_vLayersToLoad[i])
			{
				buildLayer(i, north, south, east, west);
				m_vLayersToLoad[i] = false;
			}
		}

		m_bMeshBuilt = true;

		std::cout << "mesh: " << int(1000.0f * timer.getElapsedTime()) << "ms\n";
	}

	void uploadData() noexcept
	{
		Engine::Timer timer = Engine::Timer().start();

		std::vector<uint32_t> vTotalIndices = m_vIndices;

		size_t nCumulatedStart = 0;
		for (uint16_t i = 0; i < 16; i++)
		{
			nCumulatedStart += m_vLayerIndicesOffset[i];
			const size_t offset = nCumulatedStart / 3 * 2;
			const size_t end = (i != 15) ? nCumulatedStart + m_vLayerIndicesOffset[i + 1] : vTotalIndices.size() - 1;

			for (size_t j = nCumulatedStart; j < end; j++)
				vTotalIndices.at(j) += offset;
		}

		std::cout << "upload: " << int(1000.0f * timer.getElapsedTime()) << "ms\n";

		m_vertices = Engine::MinecraftVertexbuffer(m_vVertices);
		m_indices  = Engine::IndexBuffer<uint32_t>(vTotalIndices);
		m_bMeshUploaded = true;
	}

	void renderOpaque(const BlockShader &shader) const noexcept
	{
		if (m_vIndices.size() > 0 && m_indices.isValid())
		{
			shader.bind();
			shader.setChunkBaseCoordinate(m_vBaseCoordinate);
			m_vertices.bind();
			m_indices.render();
		}
	}
};
