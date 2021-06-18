#pragma once

#include "Chunk.h"
#include "Engine/Array2D.h"
#include "BlockShader.h"
#include <thread>

class World
{
private:
	Engine::Array2D<Chunk> m_chunks{};
	size_t m_nInnerDiameter{}, m_nOuterDiameter{};
	std::vector<Engine::vu2d> m_vRenderPath1{}, m_vRenderPath2{}, m_vLoadPath{};
	Engine::vi2d m_vChunkOffset{};
	std::thread m_tUpdater;
	std::thread m_tLoader;

public:
	World() noexcept = default;

	World(const size_t radius, const Engine::vf3d around) noexcept
	{
		m_nInnerDiameter = 2 * radius + 1;
		m_nOuterDiameter = m_nInnerDiameter + 2;
		
		m_chunks = Engine::Array2D<Chunk>(Engine::vu2d(m_nOuterDiameter, m_nOuterDiameter));
		Engine::vi2d vCenterChunkCoords = worldToChunkSpace(around);
		m_vChunkOffset = Engine::vi2d(radius + 1, radius + 1) - vCenterChunkCoords;

		for (uint32_t x = 0; x < m_nOuterDiameter; x++)
		{
			for (uint32_t z = 0; z < m_nOuterDiameter; z++)
			{
				Engine::vi2d a = arrayToChunkSpace(Engine::vi2d(x, z));
				m_chunks.at(x, z) = Chunk({ 16 * a.x, 0, 16 * a.y });
			}
		}

		setupUpdatePath();

		m_tUpdater = std::thread(&World::chunkMeshUpdater, this);
		m_tLoader = std::thread(&World::chunkLoader, this);
	}

	~World() noexcept
	{
		if (m_tUpdater.joinable())
			m_tUpdater.detach();

		if (m_tLoader.joinable())
			m_tLoader.detach();
	}

private:
	void chunkLoader() noexcept
	{
		while (true)
		{
			for (size_t x = 1; x <= m_nInnerDiameter; x++)
			{
				for (size_t z = 1; z <= m_nInnerDiameter; z++)
				{
					Chunk &chunk = m_chunks.at(x, z);

					if (!chunk.isDataLoaded())
					{
						for (int x = 0; x < 16; x++)
							for (int z = 0; z < 16; z++)
								for (int y = 0; y < 256; y++)
									chunk.setBlock(Engine::vu3d(x, y, z), Block(rand() % 2, 0, 0));
						chunk.setDataLoaded();
					}
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void chunkMeshUpdater() noexcept
	{
		while (true)
		{
			for (size_t x = 1; x <= m_nInnerDiameter; x++)
			{
				for (size_t z = 1; z <= m_nInnerDiameter; z++)
				{
					Chunk &chunk = m_chunks.at(x, z);

					if (chunk.isDataLoaded() && !chunk.isMeshBuilt())
					{
						Chunk &north = m_chunks.at(x + 1, z);
						Chunk &south = m_chunks.at(x - 1, z);
						Chunk &east  = m_chunks.at(x, z + 1);
						Chunk &west  = m_chunks.at(x, z - 1);

						if (north.isDataLoaded() && south.isDataLoaded() && east.isDataLoaded() && west.isDataLoaded())
							chunk.buildMesh(north, south, east, west);
					}
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void setupUpdatePath() noexcept
	{
		m_vRenderPath1.reserve(m_nInnerDiameter * m_nInnerDiameter);
		m_vRenderPath2.reserve(m_nInnerDiameter * m_nInnerDiameter);
		m_vLoadPath.reserve(m_nOuterDiameter * m_nOuterDiameter);

		auto spiralRound = [](std::vector<Engine::vu2d> &v, const uint32_t radius, const uint32_t offset) -> void
		{
			if (radius == 0)
			{
				v.push_back(Engine::vu2d(offset, offset));
				return;
			}

			// top row
			for (uint32_t x = 0; x <= 2 * radius; x++)
				v.push_back(Engine::vu2d(x + offset, offset));

			// bottom row
			for (uint32_t x = 0; x <= 2 * radius; x++)
				v.push_back(Engine::vu2d(x + offset, 2 * radius + offset));

			// left column
			for (uint32_t z = 1; z < 2 * radius; z++)
				v.push_back(Engine::vu2d(offset, z + offset));

			// right column
			for (uint32_t z = 1; z < 2 * radius; z++)
				v.push_back(Engine::vu2d(2 * radius + offset, z + offset));
		};

		auto circular = [spiralRound](std::vector<Engine::vu2d> &v, const uint32_t radius) -> void
		{
			for (uint32_t offset = 0; offset <= radius; offset++)
				spiralRound(v, offset, radius - offset);
		};

		circular(m_vRenderPath1, (m_nInnerDiameter + 1) / 2);
		circular(m_vLoadPath,   (m_nOuterDiameter + 1) / 2);

		// reverse RenderPath1
		for (std::size_t i = 0; i < m_vRenderPath1.size(); i++)
			m_vRenderPath2.push_back(m_vRenderPath1.at(m_vRenderPath1.size() - 1 - i));
	}

public:
	Engine::vi2d chunkToArraySpace(const Engine::vi2d chunkSpace) const noexcept
	{
		return chunkSpace + m_vChunkOffset;
	}

	Engine::vi2d arrayToChunkSpace(const Engine::vi2d arraySpace) const noexcept
	{
		return arraySpace - m_vChunkOffset;
	}

	Engine::vi2d worldToChunkSpace(const Engine::vf3d world) const noexcept
	{
		return worldToChunkSpace(Engine::vf2d(world.x, world.z));
	}

	Engine::vi2d worldToChunkSpace(const Engine::vf2d world) const noexcept
	{
		return Engine::vi2d(std::floorf(world.x / 16.0f), std::floorf(world.y / 16.0f));
	}

	void update(const float fElapsedTime) noexcept
	{
		// upload finished meshes
		for (size_t x = 0; x < m_nOuterDiameter; x++)
		{
			for (size_t z = 0; z < m_nOuterDiameter; z++)
			{
				Chunk &chunk = m_chunks.at(x, z);
				if (!chunk.isMeshUploaded() && chunk.isMeshBuilt())
					chunk.uploadData();
			}
		}
	}

	void render(const BlockShader &shader) noexcept
	{
		for (const Engine::vu2d &index : m_vRenderPath1)
		{
			Chunk &chunk = m_chunks.at(index);
			if (chunk.isMeshUploaded())
				chunk.renderOpaque(shader);
		}
	}
};
