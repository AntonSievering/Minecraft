#pragma once

#include <vector>
#include "Engine/defines.h"

// NORTH  = +X
// SOUTH  = -X
// EAST   = +Z
// WEST   = -Z
// TOP    = +Y
// BOTTOM = -Y

namespace FaceTemplate
{
	namespace FullBlock
	{
		static void fillNorth(std::vector<Engine::MinecraftVertex> &v, std::vector<uint32_t> &i, const Engine::vu3d offset, const uint32_t nIndex) noexcept
		{
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 2);
			
			//v.push_back({ { pos.x + 1.0f, pos.y,        pos.z + 1.0f }, { 0.0f, 0.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x + 1.0f, pos.y,        pos.z        }, { 1.0f, 0.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f }, { 0.0f, 1.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z        }, { 1.0f, 1.0f, fZ }, 0.85f });
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y,     offset.z + 1, 1, 0, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y,     offset.z,     1, 1, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y + 1, offset.z + 1, 1, 2, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y + 1, offset.z,     1, 3, nIndex));
		}

		static void fillSouth(std::vector<Engine::MinecraftVertex> &v, std::vector<uint32_t> &i, const Engine::vu3d offset, const uint32_t nIndex) noexcept
		{
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 2);

			//v.push_back({ { pos.x, pos.y,        pos.z        }, { 0.0f, 0.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x, pos.y,        pos.z + 1.0f }, { 1.0f, 0.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x, pos.y + 1.0f, pos.z + 1.0f }, { 1.0f, 1.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x, pos.y + 1.0f, pos.z        }, { 0.0f, 1.0f, fZ }, 0.85f });
			v.push_back(Engine::MinecraftVertex(offset.x, offset.y,     offset.z,     1, 0, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x, offset.y,     offset.z + 1, 1, 1, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x, offset.y + 1, offset.z + 1, 1, 3, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x, offset.y + 1, offset.z,     1, 2, nIndex));
		}

		static void fillEast(std::vector<Engine::MinecraftVertex> &v, std::vector<uint32_t> &i, const Engine::vu3d offset, const uint32_t nIndex) noexcept
		{
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 2);

			//v.push_back({ { pos.x,        pos.y,        pos.z + 1.0f }, { 0.0f, 0.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x + 1.0f, pos.y,        pos.z + 1.0f }, { 1.0f, 0.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f }, { 1.0f, 1.0f, fZ }, 0.85f });
			//v.push_back({ { pos.x,        pos.y + 1.0f, pos.z + 1.0f }, { 0.0f, 1.0f, fZ }, 0.85f });
			v.push_back(Engine::MinecraftVertex(offset.x,     offset.y,     offset.z + 1, 1, 0, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y,     offset.z + 1, 1, 1, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y + 1, offset.z + 1, 1, 3, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x,     offset.y + 1, offset.z + 1, 1, 2, nIndex));
		}

		static void fillWest(std::vector<Engine::MinecraftVertex> &v, std::vector<uint32_t> &i, const Engine::vu3d offset, const uint32_t nIndex) noexcept
		{
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 2);
			
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y,     offset.z, 1, 0, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x,     offset.y,     offset.z, 1, 1, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y + 1, offset.z, 1, 2, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x,     offset.y + 1, offset.z, 1, 3, nIndex));
		}

		static inline void fillTop(std::vector<Engine::MinecraftVertex> &v, std::vector<uint32_t> &i, const Engine::vu3d offset, const uint32_t nIndex) noexcept
		{
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 2);

			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y + 1, offset.z,     0, 1, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x,     offset.y + 1, offset.z,     0, 0, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y + 1, offset.z + 1, 0, 3, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x,     offset.y + 1, offset.z + 1, 0, 2, nIndex));
		}

		static inline void fillBottom(std::vector<Engine::MinecraftVertex> &v, std::vector<uint32_t> &i, const Engine::vu3d offset, const uint32_t nIndex) noexcept
		{
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 2);

			v.push_back(Engine::MinecraftVertex(offset.x,     offset.y, offset.z,     2, 1, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y, offset.z,     2, 0, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x + 1, offset.y, offset.z + 1, 2, 2, nIndex));
			v.push_back(Engine::MinecraftVertex(offset.x,     offset.y, offset.z + 1, 2, 3, nIndex));
		}
	}
}
