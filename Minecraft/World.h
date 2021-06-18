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
	Engine::vu2d m_vChunkOffset{};
	std::thread m_tUpdater;

public:
	World() noexcept = default;

	World(const size_t radius, const Engine::vf3d around) noexcept
	{
		m_nInnerDiameter = 2 * radius + 1;
		m_nOuterDiameter = m_nInnerDiameter + 2;
		m_chunks = Engine::Array2D<Chunk>(Engine::vu2d(m_nOuterDiameter, m_nOuterDiameter));
		m_tUpdater = std::thread(&World::chunkMeshUpdater, this);

		Engine::vu2d vCenterChunkCoords = worldToChunkSpace(around);
		m_vChunkOffset = vCenterChunkCoords - Engine::vu2d(radius + 1, radius + 1);

		for (uint32_t x = 0; x < m_nOuterDiameter; x++)
		{
			for (uint32_t z = 0; z < m_nOuterDiameter; z++)
			{
				m_chunks.at(x, z) = Chunk({ x, 0, z });
			}
		}

		Chunk &chunk = m_chunks.at(1, 1);
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
				for (int y = 0; y < 256; y++)
					chunk.setBlock(Engine::vu3d(x, y, z), Block(rand() % 2, 0, 0));
		chunk.setDataLoaded();
	}

	~World() noexcept
	{
		if (m_tUpdater.joinable())
			m_tUpdater.detach();
	}

private:
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
	Engine::vu2d chunkToArraySpace(const Engine::vu2d chunkSpace) const noexcept
	{
		return chunkSpace + m_vChunkOffset;
	}

	Engine::vu2d arrayToChunkSpace(const Engine::vu2d arraySpace) const noexcept
	{
		return arraySpace - m_vChunkOffset;
	}

	Engine::vu2d worldToChunkSpace(const Engine::vf3d world) const noexcept
	{
		return worldToChunkSpace(Engine::vf2d(world.x, world.z));
	}

	Engine::vu2d worldToChunkSpace(const Engine::vf2d world) const noexcept
	{
		return Engine::vu2d(std::floorf(world.x / 16.0f), std::floorf(world.y / 16.0f));
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
