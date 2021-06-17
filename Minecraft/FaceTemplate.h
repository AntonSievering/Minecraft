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
		using V_Type = Engine::MinecraftVertex;
		using I_Type = uint32_t;
		using vf3d   = Engine::vf3d;
		using vf2d   = Engine::vf2d;

		static void fillNorth(std::vector<V_Type> &v, std::vector<I_Type> &i, const vf3d pos, const uint32_t nSlots, const uint32_t nIndex) noexcept
		{
			float fZ = (float)nIndex / (float)nSlots;
			
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 2);
			
			v.push_back({ { pos.x + 1.0f, pos.y,        pos.z + 1.0f }, { 0.0f, 0.0f, fZ }, 0.85f });
			v.push_back({ { pos.x + 1.0f, pos.y,        pos.z        }, { 1.0f, 0.0f, fZ }, 0.85f });
			v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f }, { 0.0f, 1.0f, fZ }, 0.85f });
			v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z        }, { 1.0f, 1.0f, fZ }, 0.85f });
		}

		static void fillSouth(std::vector<V_Type> &v, std::vector<I_Type> &i, const vf3d pos, const uint32_t nSlots, const uint32_t nIndex) noexcept
		{
			float fZ = (float)nIndex / (float)nSlots;

			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 2);

			v.push_back({ { pos.x, pos.y,        pos.z        }, { 0.0f, 0.0f, fZ }, 0.85f });
			v.push_back({ { pos.x, pos.y,        pos.z + 1.0f }, { 1.0f, 0.0f, fZ }, 0.85f });
			v.push_back({ { pos.x, pos.y + 1.0f, pos.z + 1.0f }, { 1.0f, 1.0f, fZ }, 0.85f });
			v.push_back({ { pos.x, pos.y + 1.0f, pos.z        }, { 0.0f, 1.0f, fZ }, 0.85f });
		}

		static void fillEast(std::vector<V_Type> &v, std::vector<I_Type> &i, const vf3d pos, const uint32_t nSlots, const uint32_t nIndex) noexcept
		{
			float fZ = (float)nIndex / (float)nSlots;

			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 2);

			v.push_back({ { pos.x,        pos.y,        pos.z + 1.0f }, { 0.0f, 0.0f, fZ }, 0.85f });
			v.push_back({ { pos.x + 1.0f, pos.y,        pos.z + 1.0f }, { 1.0f, 0.0f, fZ }, 0.85f });
			v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f }, { 1.0f, 1.0f, fZ }, 0.85f });
			v.push_back({ { pos.x,        pos.y + 1.0f, pos.z + 1.0f }, { 0.0f, 1.0f, fZ }, 0.85f });
		}

		static void fillWest(std::vector<V_Type> &v, std::vector<I_Type> &i, const vf3d pos, const uint32_t nSlots, const uint32_t nIndex) noexcept
		{
			float fZ = (float)nIndex / (float)nSlots;
			
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 2);
			
			v.push_back({ { pos.x + 1.0f, pos.y,        pos.z }, { 0.0f, 0.0f, fZ }, 0.85f });
			v.push_back({ { pos.x,        pos.y,        pos.z }, { 1.0f, 0.0f, fZ }, 0.85f });
			v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z }, { 0.0f, 1.0f, fZ }, 0.85f });
			v.push_back({ { pos.x,        pos.y + 1.0f, pos.z }, { 1.0f, 1.0f, fZ }, 0.85f });
		}

		static void fillTop(std::vector<V_Type> &v, std::vector<I_Type> &i, const vf3d pos, const uint32_t nSlots, const uint32_t nIndex) noexcept
		{
			float fZ = (float)nIndex / (float)nSlots;

			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 2);

			v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z        }, { 1.0f, 0.0f, fZ }, 1.0f });
			v.push_back({ { pos.x,        pos.y + 1.0f, pos.z        }, { 0.0f, 0.0f, fZ }, 1.0f });
			v.push_back({ { pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f }, { 1.0f, 1.0f, fZ }, 1.0f });
			v.push_back({ { pos.x,        pos.y + 1.0f, pos.z + 1.0f }, { 0.0f, 1.0f, fZ }, 1.0f });
		}

		static void fillBottom(std::vector<V_Type> &v, std::vector<I_Type> &i, const vf3d pos, const uint32_t nSlots, const uint32_t nIndex) noexcept
		{
			float fZ = (float)nIndex / (float)nSlots;

			i.push_back(v.size() + 0);
			i.push_back(v.size() + 1);
			i.push_back(v.size() + 2);
			i.push_back(v.size() + 3);
			i.push_back(v.size() + 0);
			i.push_back(v.size() + 2);

			v.push_back({ { pos.x,        pos.y, pos.z        }, { 1.0f, 0.0f, fZ }, 0.7f });
			v.push_back({ { pos.x + 1.0f, pos.y, pos.z        }, { 0.0f, 0.0f, fZ }, 0.7f });
			v.push_back({ { pos.x + 1.0f, pos.y, pos.z + 1.0f }, { 0.0f, 1.0f, fZ }, 0.7f });
			v.push_back({ { pos.x,        pos.y, pos.z + 1.0f }, { 1.0f, 1.0f, fZ }, 0.7f });
		}
	}
}
