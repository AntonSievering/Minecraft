#pragma once

#include "Chunk.h"
#include "Engine/Array2D.h"
#include "BlockShader.h"
#include <thread>
#include <mutex>

class World
{
private:
	Engine::Array2D<std::shared_ptr<Chunk>> m_chunks{};
	size_t m_nInnerDiameter{}, m_nOuterDiameter{}, m_nRadius;
	std::vector<Engine::vu2d> m_vRenderPath1{}, m_vRenderPath2{}, m_vLoadPath{};
	Engine::vi2d m_vChunkOffset{};
	Engine::vu2d m_vCenterArrayCoords{};
	std::thread m_tUpdater;
	std::thread m_tLoader;
	std::mutex m_arrayMutex;

public:
	World() noexcept = default;

	World(const size_t radius, const Engine::vf3d around) noexcept
	{
		m_nRadius = radius;
		m_nInnerDiameter = 2 * radius + 1;
		m_nOuterDiameter = m_nInnerDiameter + 2;
		
		m_chunks = Engine::Array2D<std::shared_ptr<Chunk>>(Engine::vu2d(m_nOuterDiameter, m_nOuterDiameter));
		Engine::vi2d vCenterChunkCoords = worldToChunkSpace(around);
		m_vChunkOffset = Engine::vi2d(m_nRadius + 1, m_nRadius + 1) - vCenterChunkCoords;
		m_vCenterArrayCoords = Engine::vu2d(m_nRadius + 1, m_nRadius + 1);

		for (uint32_t x = 0; x < m_nOuterDiameter; x++)
			for (uint32_t z = 0; z < m_nOuterDiameter; z++)
				createChunk({ x, z });

		setupUpdatePath();

		m_tUpdater = std::thread(&World::chunkMeshUpdater, this);
		m_tLoader  = std::thread(&World::chunkLoader,      this);
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
		RESET_LOADING_LOOP:
			for (const Engine::vu2d i : m_vLoadPath)
			{
				std::shared_ptr<Chunk> chunk;
				{
					std::scoped_lock lock(m_arrayMutex);
					chunk = m_chunks.at(i);
				}
				if (chunk == nullptr)
					continue;
				
				if (!chunk->isDataLoaded())
				{
					for (int x = 0; x < 16; x++)
						for (int z = 0; z < 16; z++)
							for (int y = 0; y < std::min(std::abs(chunk->getBaseCoordinate().x) / 16 * std::abs(chunk->getBaseCoordinate().z / 16), 255); y++)
								chunk->setBlock(Engine::vu3d(x, y, z), Block(1, 0, 0));
					
					chunk->setDataLoaded();

					goto RESET_LOADING_LOOP;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void chunkMeshUpdater() noexcept
	{
		while (true)
		{
		RESTART_MESH_UPLOADING_LOOP:
			for (const Engine::vu2d i : m_vRenderPath1)
			{
			RESTART_CURRENT_CHUNK_LOADING:
				
				std::shared_ptr<Chunk> chunk;
				Engine::vi2d vCachedChunkOffset;
				{
					std::scoped_lock lock(m_arrayMutex);
					vCachedChunkOffset = m_vChunkOffset;
					chunk = m_chunks.at(i);
				}

				if (chunk == nullptr)
					continue;

				if (chunk->isDataLoaded() && !chunk->isMeshBuilt())
				{
					std::shared_ptr<Chunk> north, south, east, west;
					{
						std::scoped_lock lock(m_arrayMutex);
						north = m_chunks.at(i.x + 1, i.y);
						south = m_chunks.at(i.x - 1, i.y);
						east = m_chunks.at(i.x, i.y + 1);
						west = m_chunks.at(i.x, i.y - 1);
					}

					if (m_vChunkOffset != vCachedChunkOffset)
						goto RESTART_CURRENT_CHUNK_LOADING;

					if (north == nullptr || south == nullptr || east == nullptr || west == nullptr)
						continue;

					if (north->isDataLoaded() && south->isDataLoaded() && east->isDataLoaded() && west->isDataLoaded())
						chunk->buildMesh(north.get(), south.get(), east.get(), west.get());

					goto RESTART_MESH_UPLOADING_LOOP;
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

		Engine::vi2d mid = m_vCenterArrayCoords;

		auto spiralRound = [mid](std::vector<Engine::vu2d> &v, const int radius, const Engine::vu2d vOffset) -> void
		{
			if (radius == 0)
			{
				v.push_back(mid);
				return;
			}

			for (int ox = -radius; ox <= radius; ox++)
			{
				v.push_back(Engine::vu2d(mid.x + ox, mid.y - radius));
				v.push_back(Engine::vu2d(mid.x + ox, mid.y + radius));
			}

			for (int oy = -radius + 1; oy <= radius - 1; oy++)
			{
				v.push_back(Engine::vu2d(mid.x - radius, mid.y + oy));
				v.push_back(Engine::vu2d(mid.x + radius, mid.y + oy));
			}
		};

		auto circular = [spiralRound](std::vector<Engine::vu2d> &v, const uint32_t radius, const Engine::vu2d vOffset = Engine::vu2d(0, 0)) -> void
		{
			for (int offset = 0; offset <= radius; offset++)
				spiralRound(v, offset, vOffset);
		};

		circular(m_vRenderPath1, m_nRadius, Engine::vu2d(1, 1));
		circular(m_vLoadPath, m_nRadius + 1);

		// reverse RenderPath1
		for (std::size_t i = 0; i < m_vRenderPath1.size(); i++)
			m_vRenderPath2.push_back(m_vRenderPath1.at(m_vRenderPath1.size() - 1 - i));
	}

	void createChunk(const Engine::vu2d arrayCoordinate) noexcept
	{
		Engine::vi2d a = arrayToChunkSpace(arrayCoordinate);
		m_chunks.at(arrayCoordinate) = std::make_shared<Chunk>(Engine::vi3d(16 * a.x, 0, 16 * a.y));
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

	Chunk *getChunkByArrayCoordinate(const Engine::vi2d arraySpace) noexcept
	{
		return m_chunks.at(arraySpace).get();
	}

	const Chunk *getChunkByArrayCoordinate(const Engine::vi2d arraySpace) const noexcept
	{
		return m_chunks.at(arraySpace).get();
	}

	Chunk *getChunkByChunkCoordinate(const Engine::vi2d chunkSpace) noexcept
	{
		const Engine::vi2d arraySpace = chunkToArraySpace(chunkSpace);
		return getChunkByArrayCoordinate(arraySpace);
	}

	const Chunk *getChunkByChunkCoordinate(const Engine::vi2d chunkSpace) const noexcept
	{
		const Engine::vi2d arraySpace = chunkToArraySpace(chunkSpace);
		return getChunkByArrayCoordinate(arraySpace);
	}

	static Engine::vu3d getChunkOffset(Engine::vi3d coordinate) noexcept
	{
		uint32_t x = (coordinate.x >= 0) ? coordinate.x % 16 : (16 - (-coordinate.x % 16)) % 16;
		uint32_t z = (coordinate.z >= 0) ? coordinate.z % 16 : (16 - (-coordinate.z % 16)) % 16;

		return Engine::vu3d(x, coordinate.y, z);
	}

	Block getBlock(const Engine::vi3d coordinate) const noexcept
	{
		const Engine::vi2d chunkSpace = worldToChunkSpace(coordinate);
		const Chunk *chunk = getChunkByChunkCoordinate(chunkSpace);
		
		return chunk->getBlock(getChunkOffset(coordinate));
	}

	void setBlock(const Engine::vi3d coordinate, const Block block) noexcept
	{
		const Engine::vi2d chunkSpace = worldToChunkSpace(coordinate);
		getChunkByChunkCoordinate(chunkSpace)->setBlock(getChunkOffset(coordinate), block);
	}

	void setBlock_Update(const Engine::vi3d coordinate, const Block block) noexcept
	{
		const Engine::vi2d vChunkSpace  = worldToChunkSpace(coordinate);
		const Engine::vi2d vArraySpace  = chunkToArraySpace(vChunkSpace);
		const Engine::vu3d vChunkOffset = getChunkOffset(coordinate);
		getChunkByArrayCoordinate(vArraySpace)->setBlock(vChunkOffset, block);
		getChunkByArrayCoordinate(vArraySpace)->updateLayer(coordinate.y);

		if (vChunkOffset.x == 0 && vArraySpace.x > 0)
			getChunkByArrayCoordinate(vArraySpace + Engine::vi2d(-1, 0))->updateLayer(coordinate.y);
		if (vChunkOffset.x == 15 && vArraySpace.x < m_chunks.size().x - 1)
			getChunkByArrayCoordinate(vArraySpace + Engine::vi2d(1, 0))->updateLayer(coordinate.y);
		if (vChunkOffset.z == 0 && vArraySpace.y > 0)
			getChunkByArrayCoordinate(vArraySpace + Engine::vi2d(0, -1))->updateLayer(coordinate.y);
		if (vChunkOffset.z == 15 && vArraySpace.y < m_chunks.size().y - 1)
			getChunkByArrayCoordinate(vArraySpace + Engine::vi2d(0, 1))->updateLayer(coordinate.y);
	}

	void update(const float fElapsedTime, const Engine::vf3d vPlayerPos) noexcept
	{
		// upload finished meshes
		for (size_t x = 0; x < m_nOuterDiameter; x++)
		{
			for (size_t z = 0; z < m_nOuterDiameter; z++)
			{
				Chunk *chunk = getChunkByArrayCoordinate(Engine::vi2d(x, z));
				if (!chunk->isMeshUploaded() && chunk->isMeshBuilt())
					chunk->uploadData();
			}
		}

		// update player pos
		Engine::vu2d vCurrentChunkArrayCoord = chunkToArraySpace(worldToChunkSpace(vPlayerPos));

		if (vCurrentChunkArrayCoord != m_vCenterArrayCoords)
		{
			Engine::vi2d offset = (Engine::vi2d)vCurrentChunkArrayCoord - (Engine::vi2d)m_vCenterArrayCoords;
			m_vChunkOffset -= offset;
			
			std::scoped_lock lock(m_arrayMutex);

			if (offset.x > 0)
			{
				for (int32_t z = 0; z < m_chunks.size().y; z++)
				{
					for (int32_t x = 0; x < m_chunks.size().x - offset.x; x++)
						m_chunks.at(Engine::vu2d(x, z)) = m_chunks.at(Engine::vu2d(x + offset.x, z));

					for (int32_t x = m_chunks.size().x - offset.x; x < m_chunks.size().x; x++)
						m_chunks.at(x, z) = nullptr;
				}
			}
			else if (offset.x < 0)
			{
				offset.x = -offset.x;

				for (int32_t z = 0; z < m_chunks.size().y; z++)
				{
					for (int32_t x = m_chunks.size().x - 1; x >= offset.x; x--)
						m_chunks.at(Engine::vu2d(x, z)) = m_chunks.at(Engine::vu2d(x - offset.x, z));
				
					for (int32_t x = offset.x - 1; x >= 0; x--)
						m_chunks.at(x, z) = nullptr;
				}
			}

			if (offset.y > 0)
			{
				for (int32_t x = 0; x < m_chunks.size().x; x++)
				{
					for (int32_t z = 0; z < m_chunks.size().y - offset.y; z++)
						m_chunks.at(Engine::vu2d(x, z)) = m_chunks.at(Engine::vu2d(x, z + offset.y));

					for (int32_t z = m_chunks.size().y - offset.y; z < m_chunks.size().y; z++)
						m_chunks.at(x, z) = nullptr;
				}
			}
			else if (offset.y < 0)
			{
				offset.y = -offset.y;
				
				for (int32_t x = 0; x < m_chunks.size().x; x++)
				{
					for (int32_t z = m_chunks.size().y - 1; z >= offset.y; z--)
						m_chunks.at(Engine::vu2d(x, z)) = m_chunks.at(Engine::vu2d(x, z - offset.y));

					for (int32_t z = offset.y - 1; z >= 0; z--)
						m_chunks.at(x, z) = nullptr;
				}
			}

			for (uint32_t x = 0; x < m_chunks.size().x; x++)
				for (uint32_t y = 0; y < m_chunks.size().y; y++)
					if (m_chunks.at({ x, y }) == nullptr)
						createChunk({ x, y });
		}
	}

	void render(const BlockShader &shader) noexcept
	{
		for (const Engine::vu2d &index : m_vRenderPath1)
			getChunkByArrayCoordinate(index)->renderOpaque(shader);
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
