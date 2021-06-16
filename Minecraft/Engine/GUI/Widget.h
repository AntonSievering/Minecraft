#pragma once

#include "../Engine.h"
#include "../Defines.h"
#include "../vec2d.h"
#include "../KeyManager.h"

namespace Engine
{
	namespace GUI
	{
		struct Widget
		{
			vf2d pos{};
			vf2d size{};

		public:
			Widget() noexcept = default;

			Widget(const vf2d &p, const vf2d &s) noexcept
			{
				pos = p;
				size = s;
			}

		public:
			virtual void update(PixelEngine *engine) noexcept
			{
			}

			virtual void render(PixelEngine *engine, bool bActiveGUI, const KeyManager &keyManager) noexcept
			{
			}
		};
	}
}
