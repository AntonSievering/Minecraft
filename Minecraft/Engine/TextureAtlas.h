#pragma once

#include "Sprite3D.h"

namespace Engine
{
	class TextureAtlas
	{
	private:
		uint32_t m_nSlots{};
		vu2d     m_vSlotSize;
		Image3D  m_txAtlas{};

	public:
		TextureAtlas() noexcept = default;

		TextureAtlas(const vu2d vSlotsSize, const uint32_t nSlots) noexcept
		{
			m_nSlots = nSlots;
			m_vSlotSize = vSlotsSize;
			m_txAtlas = Image3D(vu3d(m_vSlotSize.x, m_vSlotSize.y, m_nSlots));
		}

	public:
		void setSlice(const uint32_t index, const Image2D &image) noexcept
		{
			m_txAtlas.setSlice(index, image);
		}

		uint32_t getSlotCount() const noexcept
		{
			return m_nSlots;
		}

		vu2d getSlotSize() const noexcept
		{
			return m_vSlotSize;
		}

		Sprite3D createTexture() const noexcept
		{
			return Sprite3D(m_txAtlas);
		}
	};
}
