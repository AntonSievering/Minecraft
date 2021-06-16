#pragma once

#include "vec2d.h"


namespace Engine
{
	class TransformedView
	{
	private:
		Engine::vf2d m_vPanOffset{};
		float        m_fZoom = 1.0f;

	public:
		TransformedView() noexcept = default;

		TransformedView(const Engine::vf2d &vPanOffset, const float &fZoom)
		{
			m_vPanOffset = vPanOffset;
			m_fZoom = fZoom;
		}

	public:
		Engine::vf2d worldToScreen(const Engine::vf2d &pos) const noexcept
		{
			return (pos - m_vPanOffset) * m_fZoom;
		}

		Engine::vf2d screenToWorld(const Engine::vf2d &pos) const noexcept
		{
			return pos / m_fZoom + m_vPanOffset;
		}

		void zoom(const float &fFactor, const Engine::vf2d &vAround) noexcept
		{
			Engine::vf2d vMouseBeforeZoom = screenToWorld(vAround);
			m_fZoom *= fFactor;
			Engine::vf2d vMouseAfterZoom = screenToWorld(vAround);
			m_vPanOffset += vMouseBeforeZoom - vMouseAfterZoom;
		}

		void pan(const Engine::vf2d &vOffset) noexcept
		{
			m_vPanOffset -= vOffset / m_fZoom;
		}

	public:
		float getZoom() const noexcept
		{
			return m_fZoom;
		}

		void setZoom(const float &fZoom) noexcept
		{
			m_fZoom = fZoom;
		}

		Engine::vf2d getPanOffset() const noexcept
		{
			return m_vPanOffset;
		}

		void setPanOffset(const Engine::vf2d &vPanOffset) noexcept
		{
			m_vPanOffset = vPanOffset;
		}
	};
}
