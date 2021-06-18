#pragma once

#include "vec2d.h"
#include "Sprite.h"
#include "TransformedView.h"

namespace Engine
{
	class IsometricView
	{
	private:
		Sprite           m_mask{};
		vf2d             m_vTileSizeWorld{};
		float            m_fAspectRatio = 1.0f;

	public:
		IsometricView() noexcept = default;

		IsometricView(const vf2d &vTileSizeWorld, const float &fAspectRatio) noexcept
		{
			m_mask = Sprite("Engine/assets/isometricMask.png");
			m_fAspectRatio = fAspectRatio;
			m_vTileSizeWorld = vTileSizeWorld;
		}

	private:
		vf2d arrayToWorld(const vf2d &array) const noexcept
		{
			float sx = (array.x - array.y) * 0.5f * m_vTileSizeWorld.x;
			float sy = (array.x + array.y) * 0.5f * m_vTileSizeWorld.y;
			return Engine::vf2d(sx, 1.0f - sy * m_fAspectRatio);
		}

	public:
		vi2d getSelectedCell(const vf2d &vMouse, const TransformedView &tv) const noexcept
		{
			vf2d vMouseWorld = tv.screenToWorld(vMouse);
			vf2d vInverted = vf2d(vMouseWorld.x, (1.0f - vMouseWorld.y) / m_fAspectRatio);

			vf2d vCell = { floor(vInverted.x / m_vTileSizeWorld.x), ceil(vInverted.y / m_vTileSizeWorld.y) };
			vi2d vSelected = { (int32_t)(vCell.x + vCell.y), (int32_t)(vCell.y - vCell.x) };

			vf2d vOffset = vInverted % m_vTileSizeWorld / m_vTileSizeWorld;
			vOffset.x = std::fmod(vOffset.x + 1.0f, 1.0f);
			vOffset.y = std::fmod(-vOffset.y + 1.0f, 1.0f);

			Pixel sample = m_mask.getImage().sampleNearest(vOffset);

			if (sample == Engine::RED)   vSelected.x--;
			if (sample == Engine::BLUE)  vSelected.x++;
			if (sample == Engine::GREEN) vSelected.y--;
			if (sample == Engine::CYAN)  vSelected.y++;

			return vSelected;
		}

		vf2d worldToScreen(const vf2d &world) const noexcept
		{
			return arrayToWorld(world);
		}

		vf2d worldToScreenSize(const vf2d &worldSize, const float &fTileHeight) const noexcept
		{
			return m_vTileSizeWorld * Engine::vf2d(1.0f, fTileHeight * m_fAspectRatio);
		}

		vf2d getTileSizeWorld() const noexcept
		{
			return m_vTileSizeWorld;
		}
	};
}
