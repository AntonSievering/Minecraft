#pragma once

#include "Panel.h"

namespace Engine
{
	namespace GUI
	{
		class Picture : public BasePanel
		{
		private:
			Decal m_decal{};

		public:
			Picture() noexcept = default;

			Picture(const Sprite2D &sprite, const vf2d &pos, const vf2d &size) noexcept
				: BasePanel(pos, size)
			{
				m_decal = Decal(sprite, pos, size);
			}

		public:
			void render(PixelEngine *engine, bool bActiveGUI, const KeyManager &keyManager) noexcept override
			{
				engine->DrawDecal(m_decal);
			}
		};
	}
}
