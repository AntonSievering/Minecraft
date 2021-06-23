#pragma once

#include "Chunk.h"
#include "Engine/Array2D.h"
#include "BlockShader.h"
#include <thread>

class World
{
private:
	Engine::Array2D<Chunk> m_chunks{};
	size_t m_nInnerDiameter{}, m_nOuterDiameter{}, m_nRadius;
	std::vector<Engine::vu2d> m_vRenderPath1{}, m_vRenderPath2{}, m_vLoadPath{};
	Engine::vi2d m_vChunkOffset{};
	std::thread m_tUpdater;
	std::thread m_tLoader;

public:
	World() noexcept = default;

	World(const size_t radius, const Engine::vf3d around) noexcept
	{
		m_nRadius = radius;
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
			for (const Engine::vu2d i : m_vLoadPath)
			{
				Chunk *chunk = &m_chunks.at(i);

				if (!chunk->isDataLoaded())
				{
					for (int x = 1; x < 15; x++)
						for (int z = 1; z < 15; z++)
							for (int y = 0; y < 32; y++)
								chunk->setBlock(Engine::vu3d(x, y, z), Block(1, 0, 0));
						
					chunk->setDataLoaded();
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
					Chunk *chunk = &m_chunks.at(x, z);

					if (chunk->isDataLoaded() && !chunk->isMeshBuilt())
					{
						Chunk &north = m_chunks.at(x + 1, z);
						Chunk &south = m_chunks.at(x - 1, z);
						Chunk &east  = m_chunks.at(x, z + 1);
						Chunk &west  = m_chunks.at(x, z - 1);

						if (north.isDataLoaded() && south.isDataLoaded() && east.isDataLoaded() && west.isDataLoaded())
							chunk->buildMesh(north, south, east, west);
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

		auto spiralRound = [](std::vector<Engine::vu2d> &v, const uint32_t radius, const uint32_t offset, const Engine::vu2d vOffset) -> void
		{
			if (radius == 0)
			{
				v.push_back(Engine::vu2d(offset, offset) + vOffset);
				return;
			}

			// top row
			for (uint32_t x = 0; x <= 2 * radius; x++)
				v.push_back(Engine::vu2d(x + offset, offset) + vOffset);

			// bottom row
			for (uint32_t x = 0; x <= 2 * radius; x++)
				v.push_back(Engine::vu2d(x + offset, 2 * radius + offset) + vOffset);

			// left column
			for (uint32_t z = 1; z < 2 * radius; z++)
				v.push_back(Engine::vu2d(offset, z + offset) + vOffset);

			// right column
			for (uint32_t z = 1; z < 2 * radius; z++)
				v.push_back(Engine::vu2d(2 * radius + offset, z + offset) + vOffset);
		};

		auto circular = [spiralRound](std::vector<Engine::vu2d> &v, const uint32_t radius, const Engine::vu2d vOffset = Engine::vu2d(0, 0)) -> void
		{
			for (uint32_t offset = 0; offset <= radius; offset++)
				spiralRound(v, offset, radius - offset, vOffset);
		};

		circular(m_vRenderPath1, m_nRadius, Engine::vu2d(1, 1));
		circular(m_vLoadPath, m_nRadius + 1);

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

	Engine::vi2d worldToChunkSpace(const Engine::vi3d world) const noexcept
	{
		return worldToChunkSpace(world.xz());
	}

	Engine::vi2d worldToChunkSpace(Engine::vi2d world) const noexcept
	{
		if (world.x < 0) world.x = world.x - 15;
		if (world.y < 0) world.y = world.y - 15;
		return world / 16;
	}

	const Chunk &getChunkByArrayCoordinate(const Engine::vi2d arraySpace) const noexcept
	{
		return m_chunks.at(arraySpace);
	}

	const Chunk &getChunkByChunkCoordinate(const Engine::vi2d chunkSpace) const noexcept
	{
		const Engine::vi2d arraySpace = chunkToArraySpace(chunkSpace);
		return getChunkByArrayCoordinate(arraySpace);
	}

	static Engine::vu3d getChunkOffset(Engine::vi3d coordinate) noexcept
	{
		uint32_t x = (coordinate.x > 0) ? coordinate.x % 16 : 16 - (-coordinate.x % 16);
		uint32_t z = (coordinate.z > 0) ? coordinate.z % 16 : 16 - (-coordinate.z % 16);

		return Engine::vu3d(x, coordinate.y, z);
	}

	Block getBlock(const Engine::vi3d coordinate) const noexcept
	{
		const Engine::vi2d chunkSpace = worldToChunkSpace(coordinate);
		const Chunk &chunk = getChunkByChunkCoordinate(chunkSpace);
		
		return chunk.getBlock(getChunkOffset(coordinate));
	}

	void setBlock(const Engine::vi3d coordinate, const Block block) noexcept
	{
		const Engine::vi2d chunkSpace = worldToChunkSpace(coordinate);
		const Engine::vi2d arraySpace = chunkToArraySpace(chunkSpace);
		m_chunks.at(arraySpace).setBlock(getChunkOffset(coordinate), block);
	}

	void setBlock_Update(const Engine::vi3d coordinate, const Block block) noexcept
	{
		const Engine::vi2d vChunkSpace  = worldToChunkSpace(coordinate);
		const Engine::vi2d vArraySpace  = chunkToArraySpace(vChunkSpace);
		const Engine::vu3d vChunkOffset = getChunkOffset(coordinate);
		m_chunks.at(vArraySpace).setBlock(vChunkOffset, block);
		m_chunks.at(vArraySpace).updateLayer(coordinate.y);
	}

	void update(const float fElapsedTime) noexcept
	{
		// upload finished meshes
		for (size_t x = 0; x < m_nOuterDiameter; x++)
		{
			for (size_t z = 0; z < m_nOuterDiameter; z++)
			{
				Chunk *chunk = &m_chunks.at(x, z);
				if (!chunk->isMeshUploaded() && chunk->isMeshBuilt())
					chunk->uploadData();
			}
		}
	}

	void render(const BlockShader &shader) noexcept
	{
		for (const Engine::vu2d &index : m_vRenderPath1)
			m_chunks.at(index).renderOpaque(shader);
	}

	bool getSelectedBlock_DDA(const Engine::FPSCamera &camera, Engine::vi3d &selectedPos, Engine::vi3d &targetedPos) const noexcept
	{
		const Engine::vf3d vCameraPos = camera.getPosition();
		const Engine::vf3d vCameraLookAt = camera.getLookAt();
		
		auto square = [](const float val) -> float { return val * val; };

		const float sx = std::abs(1.0f / vCameraLookAt.x);
		const float sy = std::abs(1.0f / vCameraLookAt.y);
		const float sz = std::abs(1.0f / vCameraLookAt.z);

		int32_t mx{}, my{}, mz{};
		Engine::vf3d vTest{};
		selectedPos = (Engine::vi3d)(std::floor(vCameraPos));
		
		if (vCameraLookAt.x < 0.0f)
		{
			mx = -1;
			vTest.x = (vCameraPos.x - (float)selectedPos.x) * sx;
		}
		else
		{
			mx = 1;
			vTest.x = ((float)(selectedPos.x + 1) - vCameraPos.x) * sx;
		}

		if (vCameraLookAt.y < 0.0f)
		{
			my = -1;
			vTest.y = (vCameraPos.y - (float)selectedPos.y) * sy;
		}
		else
		{
			my = 1;
			vTest.y = ((float)(selectedPos.y + 1) - vCameraPos.y) * sy;
		}

		if (vCameraLookAt.z < 0.0f)
		{
			mz = -1;
			vTest.z = (vCameraPos.z - (float)selectedPos.z) * sz;
		}
		else
		{
			mz = 1;
			vTest.z = ((float)(selectedPos.z + 1) - vCameraPos.z) * sz;
		}

		float fDistance = 0.0f;
		while (fDistance < 4.0f)
		{
			int axis;
			if (vTest.x < vTest.y && vTest.x < vTest.z)
			{
				selectedPos.x += mx;
				fDistance = vTest.x;
				vTest.x += sx;
				axis = 0;
			}
			else if (vTest.y < vTest.x && vTest.y < vTest.z)
			{
				selectedPos.y += my;
				fDistance = vTest.y;
				vTest.y += sy;
				axis = 1;
			}
			else
			{
				selectedPos.z += mz;
				fDistance = vTest.z;
				vTest.z += sz;
				axis = 2;
			}

			if ((selectedPos.y < 0 && my == -1) || (selectedPos.y > 255 && my == 1))
				return false;

			if (getBlock(selectedPos).getId() > 0) 
			{
				targetedPos = selectedPos;

				if (axis == 0)
				{
					if (vCameraPos.x < selectedPos.x)
						targetedPos.x--;
					else
						targetedPos.x++;
				}
				else if (axis == 1)
				{
					if (vCameraPos.y < selectedPos.y)
						targetedPos.y--;
					else
						targetedPos.y++;
				}
				else
				{
					if (vCameraPos.z < selectedPos.z)
						targetedPos.z--;
					else
						targetedPos.z++;
				}

				return true;
			}
		}

		return false;
	}
};
