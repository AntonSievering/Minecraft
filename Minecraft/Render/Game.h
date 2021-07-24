#pragma once

#include "Engine/GUIEngine.h"
#include "Engine/Camera.h"
#include "BlockShader.h"
#include "World.h"
#include "Engine/TextureAtlas.h"
#include "BlockHighlight.h"
#include "GUI_Overlay.h"

class Game : public Engine::GUIEngine
{
private:
	Engine::FloatingCamera camera;
	Engine::TextureAtlas   textureAtlas;
	Engine::Sprite3D       texture;
	BlockShader            shader;
	BlockHighlight         highlight{};
	World                 *world;
	Gamemode               m_gameMode;
	Player                 player{};
	Engine::Timer          m_spaceDoubleClickTimer{};
	Engine::Timer          m_jumpOffsetTimer{};
	float                  m_fSprintOffset = 0.0f;
	float                  m_fSprintOffsetChangeDir = 1.0f;
	GUI_Overlay            m_guiOverlay;

public:
	bool OnUserCreate() noexcept override
	{
		m_gameMode = Gamemode(GameModeType::CREATIVE);
		player = Player("Anton", &m_gameMode, { 8.0f, 0.0f, 8.0f });

		camera = Engine::FloatingCamera(110.0f, (float)GetScreenSize().x, (float)GetScreenSize().y);
		camera.setPosition(player.hitbox.pos);
		camera.update();
		
		shader = BlockShader("Content/shader/blockShader");

		highlight = BlockHighlight("Content/shader/lineShader");
		m_guiOverlay = GUI_Overlay(1, GetScreenSize());

		textureAtlas = Engine::TextureAtlas(Engine::vu2d(16, 16), (uint32_t)FaceId::Count);
		for (uint32_t i = 0; i < (uint32_t)FaceId::Count; i++)
			textureAtlas.setSlice(i, Engine::Image2D("Content/sprites/blocks/" + FaceFilenames::getFilename((FaceId)i) + ".png"));
		texture = textureAtlas.createTexture();

		shader.setTextureHeight(textureAtlas.getSlotCount());

		world = new World(16, camera.getPosition());

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) noexcept override
	{
		if (GetKey(Engine::Key::MOUSE_LEFT).bPressed)
			HideMouse(true);
		if (GetKey(Engine::Key::ESCAPE).bPressed)
			HideMouse(false);

		Engine::vf3d vTargetMovement{};
		if (GetHideMouseStatus())
		{
			camera.onMouseMoved(GetRelativeMouse().x, GetRelativeMouse().y);

			if (!player.bGrounded)
				m_jumpOffsetTimer.start();

			if (GetKey(Engine::Key::Q).bHeld && m_fSprintOffset == 0.0f)
				m_fSprintOffsetChangeDir = 1.0f;

			m_fSprintOffset += m_fSprintOffsetChangeDir * fElapsedTime / 0.1f;
			m_fSprintOffset = std::clamp(m_fSprintOffset, 0.0f, 1.0f);
			camera.setFieldOfView(110.0f + m_fSprintOffset * m_fSprintOffset * 10.0f);

			float fFWSpeed = 4.5f * (1.0f + 0.2f * m_fSprintOffset);
			float fSWSpeed = 5.0f;
			float fUPSpeed = 5.0f;
			
			if (player.bFlying)
			{
				fFWSpeed *= 4;
				fSWSpeed *= 3;
				fUPSpeed *= 3;
			}

			if (GetKey(Engine::Key::W).bHeld && !GetKey(Engine::Key::S).bHeld)
				vTargetMovement += camera.getMoveFrontVector() * fFWSpeed;
			else
				m_fSprintOffsetChangeDir = -1.0f;
			if (GetKey(Engine::Key::S).bHeld && !GetKey(Engine::Key::W).bHeld)
				vTargetMovement -= camera.getMoveFrontVector() * fSWSpeed;

			if (GetKey(Engine::Key::A).bHeld && !GetKey(Engine::Key::D).bHeld)
				vTargetMovement -= camera.getMoveSidewaysVector() * fSWSpeed;
			if (GetKey(Engine::Key::D).bHeld && !GetKey(Engine::Key::A).bHeld)
				vTargetMovement += camera.getMoveSidewaysVector() * fSWSpeed;
			
			if (GetKey(Engine::Key::SPACE).bPressed)
			{
				if (m_spaceDoubleClickTimer.getElapsedTime() < 0.3f)
					player.toggleFlying();
				m_spaceDoubleClickTimer.start();
			}

			if (GetKey(Engine::Key::SPACE).bHeld)
			{
				if (player.bFlying && !GetKey(Engine::Key::LSHIFT).bHeld)
					vTargetMovement += camera.getMoveUpVector() * fUPSpeed;
				else if (m_jumpOffsetTimer.getElapsedTime() > 0.05f)
					player.jump(camera.getMoveFrontVector());
			}
			if (GetKey(Engine::Key::LSHIFT).bHeld)
			{
				if (m_gameMode.canFly())
					vTargetMovement -= camera.getMoveUpVector() * fUPSpeed;
			}
		}

		player.update(fElapsedTime, vTargetMovement);

		world->collideEntity(player);
		
		world->update(fElapsedTime, player.hitbox.pos);
		camera.setPosition(player.getEyePosition());
		camera.update();

		Clear(Engine::BLUE, GL_DEPTH_BUFFER_BIT);
		shader.setModelViewProjectionMatrix(camera.getViewProj());
		texture.bind();
		world->render(shader);

		Engine::vi3d vSelected, vTargeted;
		if (world->getSelectedBlock_DDA(camera, vSelected, vTargeted))
		{
			if (GetKey(Engine::Key::MOUSE_LEFT).bPressed)
			{
				world->setBlock_Update(vSelected, Block(BlockId::AIR));
			}
			else if (GetKey(Engine::Key::MOUSE_RIGHT).bPressed)
			{
				if (!aabb::Hitbox3d(vTargeted, Engine::vf3d(1.0f, 1.0f, 1.0f)).collides(player.hitbox))
					world->setBlock_Update(vTargeted, Block(BlockId::STONE));
			}
			highlight.render(camera, vSelected);
		}
		
		m_guiOverlay.render(m_sSpriteShader);

		return true;
	}

	bool OnUserDestroy() noexcept override
	{
		return true;
	}
};
