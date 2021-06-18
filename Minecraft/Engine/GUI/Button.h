#pragma once

#include "Label.h"
#include "../Decal.h"

namespace Engine
{
	namespace GUI
	{
		struct ButtonSprites
		{
			Sprite2D sprNormal;
			Sprite2D sprHovered;
			Sprite2D sprPressed;
		};

		struct ButtonDecals
		{
			Decal decNormal;
			Decal decHovered;
			Decal decPressed;
		};

		enum class Activation : uint8_t
		{
			PRESS,
			RELEASE
		};

		class Button : public Widget
		{
		private:
			ButtonDecals m_decals;
			Activation   m_activation;

		public:
			Button() noexcept = default;

			Button(const ButtonSprites &textures, const vf2d pos, const vf2d size, const Activation activation) noexcept
				: Widget(pos, size)
			{
				m_activation = activation;
				m_decals.decNormal = Decal(textures.sprNormal, pos, size);
				m_decals.decHovered = Decal(textures.sprHovered, pos, size);
				m_decals.decPressed = Decal(textures.sprPressed, pos, size);
			}

		public:
			bool isOver(const vf2d vMousePos) const noexcept
			{
				return pos.x <= vMousePos.x && pos.x + size.x >= vMousePos.x
					&& pos.y <= vMousePos.y && pos.y + size.y >= vMousePos.y;
			}

			bool isPressed(const vf2d vMousePos, const HWKey key) const noexcept
			{
				if (m_activation == Activation::PRESS)
					return key.bPressed && isOver(vMousePos);
				return key.bReleased && isOver(vMousePos);
			}

			void render(PixelEngine *engine, bool bActiveGUI, const KeyManager &keyManager) noexcept override
			{
				if (bActiveGUI && isOver(engine->GetMouse()))
				{
					if (keyManager.getKey(Key::MOUSE_LEFT).bHeld)
						engine->DrawDecal(m_decals.decPressed);
					else
						engine->DrawDecal(m_decals.decHovered);
				}
				else
				{
					engine->DrawDecal(m_decals.decNormal);
				}
			}
		};
	}
}
