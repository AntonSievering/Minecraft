#pragma once

#include "Engine/Decal.h"

class GUI_Overlay
{
private:
	Engine::Decal m_crosshair;

public:
	GUI_Overlay() noexcept = default;

	GUI_Overlay(int size, const Engine::vf2d vScreenSize) noexcept
	{
		const Engine::vf2d vPxSize = Engine::vf2d(2.0f, 2.0f) / vScreenSize;
		
		const Engine::vf2d vCrosshairSize = vPxSize * Engine::vf2d(16.0f, 16.0f);
		const Engine::vf2d vCrosshairPos = vCrosshairSize * -0.5f;
		m_crosshair = Engine::Decal(Engine::Sprite2D("content/sprites/gui/crosshair.png"), vCrosshairPos, vCrosshairSize);
	}

public:
	void render(const Engine::SpriteShader &shader) const noexcept
	{
		glDisable(GL_DEPTH_TEST);
		m_crosshair.render(shader);
		glEnable(GL_DEPTH_TEST);
	}
};
