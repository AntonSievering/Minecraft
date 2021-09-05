#include "Render/Game.h"

int main()
{
	Game game;
	if (game.Construct(Engine::vi2d(1280, 720), Engine::FullscreenMode::WINDOWED, Engine::RefreshRate::VSYNC, 1))
		game.Start();
}
