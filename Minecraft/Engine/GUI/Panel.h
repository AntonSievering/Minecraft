#pragma once

#include "Widget.h"
#include "../Decal.h"

namespace Engine
{
	namespace GUI
	{
		class BasePanel : public Widget
		{
		public:
			BasePanel() noexcept = default;

			BasePanel(const vf2d &pos, const vf2d &size) noexcept
				: Widget(pos, size) {}
		};

		class Panel : public BasePanel
		{
		private:
			Decal m_decal{};

		public:
			Panel() noexcept = default;

			Panel(const vf2d &pos, const vf2d &size, const Pixel &px) noexcept
				: BasePanel(pos, size)
			{
				Image2D image = Image2D(Engine::vu2d(1, 1));
				image.setPixel(Engine::vu2d(0, 0), px);
				Sprite2D spr = Sprite2D(image, GL_NEAREST, GL_NEAREST);
				m_decal = Decal(spr, pos, size);
			}

		public:
			void render(PixelEngine *engine, bool bActiveGUI, const KeyManager &keyManager) noexcept override
			{
				engine->DrawDecal(m_decal);
			}
		};
	}
}
