#pragma once

#include "Engine/GUIEngine.h"
#include "Engine/Camera.h"
#include "BlockShader.h"
#include "World.h"
#include "Engine/TextureAtlas.h"
#include "BlockHighlight.h"

class Game : public Engine::GUIEngine
{
private:
	Engine::FloatingCamera camera;
	Engine::TextureAtlas   textureAtlas;
	Engine::Sprite3D       texture;
	BlockShader            shader;
	BlockHighlight         highlight{};
	World                 *world;

public:
	bool OnUserCreate() noexcept override
	{
		camera = Engine::FloatingCamera(glm::radians(90.0f), (float)GetScreenSize().x, (float)GetScreenSize().y);
		camera.setPosition(glm::vec3(8.0f, 0.0f, 8.0f));
		
		shader = BlockShader("content/shader/blockShader");

		highlight = BlockHighlight("content/shader/lineShader");

		Engine::Image2D image = Engine::Image2D("content/sprites/blocks/stone.png");

		textureAtlas = Engine::TextureAtlas(image.size(), 1);
		textureAtlas.setSlice(0, image);
		texture = textureAtlas.createTexture();

		world = new World(0, camera.getPosition());

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) noexcept override
	{
		if (GetKey(Engine::Key::MOUSE_LEFT).bPressed)
			HideMouse(true);
		if (GetKey(Engine::Key::ESCAPE).bPressed)
			HideMouse(false);

		if (GetHideMouseStatus())
		{
			camera.onMouseMoved(GetRelativeMouse().x, GetRelativeMouse().y);

			const float fDist = 10.0f * fElapsedTime;
			if (GetKey(Engine::Key::W).bHeld)
				camera.moveFront(fDist);
			if (GetKey(Engine::Key::S).bHeld)
				camera.moveFront(-fDist);
			if (GetKey(Engine::Key::A).bHeld)
				camera.moveSideways(-fDist);
			if (GetKey(Engine::Key::D).bHeld)
				camera.moveSideways(fDist);
			if (GetKey(Engine::Key::SPACE).bHeld)
				camera.moveUp(fDist);
			if (GetKey(Engine::Key::LSHIFT).bHeld)
				camera.moveUp(-fDist);

			camera.update();
		}

		world->update(fElapsedTime);

		Clear(Engine::BLUE, GL_DEPTH_BUFFER_BIT);
		shader.setModelViewProjectionMatrix(camera.getViewProj());
		texture.bind();
		world->render(shader);

		Engine::vi3d vSelected, vTargeted;
		if (world->getSelectedBlock_DDA(camera, vSelected, vTargeted))
		{
			highlight.render(camera, vSelected);
		}
		
		return true;
	}

	bool OnUserDestroy() noexcept override
	{
		return true;
	}
};
