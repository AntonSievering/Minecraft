#pragma once

#include "Engine/GUIEngine.h"
#include "Engine/Camera.h"
#include "BlockShader.h"
#include "Chunk.h"
#include "Engine/Sprite3D.h"

class Game : public Engine::GUIEngine
{
private:
	Engine::FloatingCamera camera;
	Engine::Image3D        textureAtlas;
	Engine::Sprite3D       sprite3d;
	BlockShader            shader;
	Chunk                  chunk;

public:
	bool OnUserCreate() noexcept override
	{
		camera = Engine::FloatingCamera(glm::radians(90.0f), (float)GetScreenSize().x, (float)GetScreenSize().y);
		shader = BlockShader("content/shader/blockShader");
		
		Engine::Image2D image = Engine::Image2D("content/sprites/blocks/stone.png");

		uint16_t nSlots = 10;
		textureAtlas = Engine::Image3D(Engine::vu3d(image.getWidth(), image.getHeight(), nSlots));
		textureAtlas.setSlice(0, image);
		sprite3d = Engine::Sprite3D(textureAtlas);

		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
				for (int y = 0; y < 256; y++)
					chunk.setBlock(Engine::vu3d(x, y, z), Block(rand() % 2, 0, 0));

		Chunk c;
		chunk.buildMesh(nSlots, c, c, c, c);

		glDisable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

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
		sprite3d.bind();
		chunk.m_vertices.render();

		//std::cout << (Engine::vf3d)camera.getPosition() << std::endl;

		return true;
	}

	bool OnUserDestroy() noexcept override
	{
		return true;
	}
};
