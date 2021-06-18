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
						chunk.buildMesh(m_chunks.at(x + 1, z), m_chunks.at(x - 1, z), m_chunks.at(x, z + 1), m_chunks.at(x, z - 1));
					}
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
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
				if (chunk.isMeshBuilt() && !chunk.isMeshUploaded())
					chunk.uploadData();
			}
		}
	}

	void render(const BlockShader &shader) noexcept
	{
		for (size_t x = 1; x <= m_nInnerDiameter; x++)
		{
			for (size_t z = 1; z <= m_nInnerDiameter; z++)
			{
				// wtf: const Chunk &
				Chunk &chunk = m_chunks.at(x, z);
				if (chunk.isMeshUploaded())
					chunk.renderOpaque(shader);
			}
		}
	}
};
