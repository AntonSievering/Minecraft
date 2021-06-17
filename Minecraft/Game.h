#pragma once

#include "Engine/GUIEngine.h"
#include "Engine/Camera.h"
#include "BlockShader.h"
#include "Chunk.h"
#include "Engine/TextureAtlas.h"

class Game : public Engine::GUIEngine
{
private:
	Engine::FloatingCamera camera;
	Engine::TextureAtlas   textureAtlas;
	Engine::Sprite3D       texture;
	BlockShader            shader;
	Chunk                  chunk;

public:
	bool OnUserCreate() noexcept override
	{
		camera = Engine::FloatingCamera(glm::radians(90.0f), (float)GetScreenSize().x, (float)GetScreenSize().y);
		shader = BlockShader("content/shader/blockShader");
		
		Engine::Image2D image = Engine::Image2D("content/sprites/blocks/stone.png");

		textureAtlas = Engine::TextureAtlas(image.size(), 1);
		textureAtlas.setSlice(0, image);
		texture = textureAtlas.createTexture();

		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
				for (int y = 0; y < 256; y++)
					chunk.setBlock(Engine::vu3d(x, y, z), Block(rand() % 2, 0, 0));
				
		Chunk c;
		Engine::Timer timer = Engine::Timer().start();
		
		chunk.buildMesh(c, c, c, c);

		std::cout << timer.getElapsedTime() << std::endl;

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

			if (GetKey(Engine::Key::W).bHeld)
				camera.moveFront(fElapsedTime);
			if (GetKey(Engine::Key::S).bHeld)
				camera.moveFront(-fElapsedTime);
			if (GetKey(Engine::Key::A).bHeld)
				camera.moveSideways(-fElapsedTime);
			if (GetKey(Engine::Key::D).bHeld)
				camera.moveSideways(fElapsedTime);
			if (GetKey(Engine::Key::SPACE).bHeld)
				camera.moveUp(fElapsedTime);
			if (GetKey(Engine::Key::LSHIFT).bHeld)
				camera.moveUp(-fElapsedTime);

			camera.update();
		}

		Clear(Engine::BLUE, GL_DEPTH_BUFFER_BIT);
		shader.setModelViewProjectionMatrix(camera.getViewProj());
		texture.bind();
		chunk.m_vertices.bind();
		chunk.m_indices.render();

		//std::cout << (Engine::vf3d)camera.getPosition() << std::endl;

		return true;
	}

	bool OnUserDestroy() noexcept override
	{
		return true;
	}
};
