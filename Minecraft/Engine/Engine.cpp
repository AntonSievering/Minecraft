#include "Engine.h"

namespace Engine
{
	bool PixelEngine::Construct(vi2d size, FullscreenMode fullscreen, RefreshRate refresh, int subSamples) noexcept
	{
		// Init SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, subSamples);

		// Create window & GLContext
		m_window = SDL_CreateWindow("C++ Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_OPENGL | (uint32_t)fullscreen);
		if (m_window == nullptr)
			return false;

		m_glContext = SDL_GL_CreateContext(m_window);
		SetRefreshRate(refresh);

		// get the screen size
		if (fullscreen != FullscreenMode::WINDOWED)
		{
			SDL_DisplayMode dm;
			SDL_GetCurrentDisplayMode(0, &dm);
			m_vScreenSize.x = dm.w;
			m_vScreenSize.y = dm.h;
		}
		else
		{
			m_vScreenSize = size;
		}
		m_fAspectRatio = (float)m_vScreenSize.x / (float)m_vScreenSize.y;

		// Init glew
		if (glewInit() != GLEW_OK)
			return false;

		// get some information about the user
		m_sOpenGLVersion = (const char *)glGetString(GL_VERSION);
		m_sGraficsCard = (const char *)glGetString(GL_RENDERER);
		m_sGraficsCardVendor = (const char *)glGetString(GL_VENDOR);

		m_mapKeys =
		{
			{ 0, Engine::Key::MOUSE_LEFT }, { 1, Engine::Key::MOUSE_RIGHT }, { 2, Engine::Key::MOUSE_MIDDLE },
			{ 97,  Engine::Key::A }, { 98,  Engine::Key::B }, { 99,  Engine::Key::C }, { 100, Engine::Key::D }, { 101, Engine::Key::E }, { 102, Engine::Key::F }, { 103, Engine::Key::G }, { 104, Engine::Key::H },	{ 105, Engine::Key::I },
			{ 106, Engine::Key::J }, { 107, Engine::Key::K }, { 108, Engine::Key::L }, { 109, Engine::Key::M }, { 110, Engine::Key::N }, { 111, Engine::Key::O }, { 112, Engine::Key::P }, { 113, Engine::Key::Q }, { 114, Engine::Key::R },
			{ 115, Engine::Key::S }, { 116, Engine::Key::T }, { 117, Engine::Key::U }, { 118, Engine::Key::V }, { 119, Engine::Key::W }, { 120, Engine::Key::X }, { 121, Engine::Key::Y }, { 122, Engine::Key::Z },

			{ 48,  Engine::Key::K0 }, { 49,  Engine::Key::K1 }, { 50,  Engine::Key::K2 }, { 51,  Engine::Key::K3 }, { 52,  Engine::Key::K4 }, { 53,  Engine::Key::K5 }, { 54,  Engine::Key::K6 }, { 55,  Engine::Key::K7 }, { 56,  Engine::Key::K8 }, { 57,  Engine::Key::K9 },

			{ 1073741882, Engine::Key::F1            }, { 1073741883, Engine::Key::F2          }, { 1073741884, Engine::Key::F3           }, { 1073741885, Engine::Key::F4        }, { 1073741886, Engine::Key::F5          }, { 1073741887, Engine::Key::F6  },
			{ 1073741889, Engine::Key::F7            }, { 1073741890, Engine::Key::F8          }, { 1073741891, Engine::Key::F9           }, { 1073741892, Engine::Key::F10       }, { 1073741893, Engine::Key::F11         }, { 1073741894, Engine::Key::F12 },

			{ 1073741907, Engine::Key::NP_ENABLE     }, { 1073741908, Engine::Key::NP_DIVIDE   }, { 1073741909,  Engine::Key::NP_MULITPLY }, { 1073741910,  Engine::Key::NP_MINUS },
			{ 1073741911, Engine::Key::NP_PLUS       }, { 1073741912, Engine::Key::NP_ENTER    }, { 1073741923,  Engine::Key::NP_DEL      },

			{ 27,         Engine::Key::ESCAPE        }, { 94,         Engine::Key::CIRCUMFLEX  }, { 9,           Engine::Key::TAB         }, { 1073741881, Engine::Key::CAPS_LOCK }, { 1073742049,  Engine::Key::LSHIFT     },
			{ 1073742048, Engine::Key::LCTRL         }, { 1073742051, Engine::Key::WINDOWS     }, { 1073742050,  Engine::Key::ALT         }, { 32,         Engine::Key::SPACE     }, { 1073742054,  Engine::Key::ALT_GR     },
			{ 1073742052, Engine::Key::RCTRL         }, { 1073742053, Engine::Key::RSHIFT      }, { 13,          Engine::Key::RETURN      }, { 8,          Engine::Key::BACKSPACE }, { 180,  Engine::Key::APOSTROPHE        },
			{ 223,        Engine::Key::QUESTION_MARK }, { 1073741903, Engine::Key::ARROW_RIGHT }, { 1073741904,  Engine::Key::ARROW_LEFT  }, { 1073741906, Engine::Key::ARROW_UP  }, { 1073741905,  Engine::Key::ARROW_DOWN }
		};

		// enable multisample
		glEnable(GL_MULTISAMPLE);

		// enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// culling, depth testing, counter-clock-wise triangles
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);

		// disable polygon smoothing
		glDisable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

		m_sSpriteShader = SpriteShader("Engine/Shader/sprite2d/spriteShader");
		m_pStringTables[(uint32_t)Resolution::LOW] = Sprite2D("Engine/assets/ascii-sheets/low.png", GL_NEAREST, GL_NEAREST);
		m_pStringTables[(uint32_t)Resolution::MEDIUM] = Sprite2D("Engine/assets/ascii-sheets/high.png", GL_LINEAR, GL_LINEAR); // Medium = High
		m_pStringTables[(uint32_t)Resolution::HIGH] = m_pStringTables[(uint32_t)Resolution::MEDIUM];

		// seed is a random idx in RAM
		nLehmerSeed = math::bit_cast<uint64_t>(m_window) & 0xFFFFFFFF;

		return true;
	}

	bool PixelEngine::SetRefreshRate(RefreshRate refreshRate) noexcept
	{
		return SDL_GL_SetSwapInterval((int)refreshRate) == 0;
	}

	bool PixelEngine::OnUserCreate() noexcept
	{
		return true;
	}

	bool PixelEngine::OnUserUpdate(float fElapsedTime) noexcept
	{
		return true;
	}

	bool PixelEngine::OnUserDestroy() noexcept
	{
		return true;
	}

	void PixelEngine::Start() noexcept
	{
		if (!OnUserCreate())
			return;

		Timer timer = Timer().start();
		m_fTimeSinceStart = 0.0f;
		m_fElapsedTime = 0.0f;
		m_nFPS = 0;

		while (true)
		{
			// Timing
			m_fTimeSinceStart += (m_fElapsedTime = timer.getElapsedTime());
			timer.start();

			bool bExit = !ScanHardware();
			UpdateInternals();
			bExit |= !OnUserUpdate(m_fElapsedTime);
			RenderInternals();

			if (bExit)
			{
				if (OnUserDestroy())
					return;
			}

			SDL_GL_SwapWindow(m_window);

			UpdateTitle();
		}
	}

	void PixelEngine::Clear(const Pixel &col, int bDepthBufferBit) noexcept
	{
		glClearColor((float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | bDepthBufferBit);
	}

	void PixelEngine::ClearDepthBuffer() noexcept
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	HWKey PixelEngine::GetKey(const Key &key) const noexcept
	{
		return m_vKeyManager.getKey(key);
	}

	vf2d PixelEngine::GetMouse() const noexcept
	{
		return m_vMousePos;
	}

	float PixelEngine::GetMouseX() const noexcept
	{
		return m_vMousePos.x;
	}

	float PixelEngine::GetMouseY() const noexcept
	{
		return m_vMousePos.y;
	}

	float PixelEngine::GetMouseWheel() const noexcept
	{
		return m_fWheelMovement;
	}

	uint64_t PixelEngine::GetFPS() const noexcept
	{
		return m_nFPS;
	}

	vf2d PixelEngine::GetRelativeMouse() const noexcept
	{
		return m_relativeMouseMovement;
	}

	float PixelEngine::GetRelativeMouseX() const noexcept
	{
		return m_relativeMouseMovement.x;
	}

	float PixelEngine::GetRelativeMouseY() const noexcept
	{
		return m_relativeMouseMovement.y;
	}

	void PixelEngine::HideMouse(const bool &bHide) noexcept
	{
		SDL_SetRelativeMouseMode((SDL_bool)bHide);
	}

	bool PixelEngine::GetHideMouseStatus() const noexcept
	{
		return SDL_GetRelativeMouseMode();
	}

	vi2d PixelEngine::GetScreenSize() const noexcept
	{
		return m_vScreenSize;
	}

	vf2d PixelEngine::toOpenGL(const vf2d &pos) const noexcept
	{
		return (((pos * 2) / m_vScreenSize) - vf2d(1.0f, 1.0f)) * vf2d(1.0f, -1.0f);
	}

	float PixelEngine::GetElapsedTime() const noexcept
	{
		return m_fElapsedTime;
	}

	float PixelEngine::GetTimeSinceStart() const noexcept
	{
		return m_fTimeSinceStart;
	}

	uint32_t PixelEngine::randuint() noexcept
	{
		return Lehmer32(nLehmerSeed);
	}

	uint32_t PixelEngine::randuint(const uint32_t &nLimit) noexcept
	{
		return randuint() % nLimit;
	}

	uint32_t PixelEngine::randuint(const uint32_t &nLowerLimit, const uint32_t &nUpperLimit) noexcept
	{
		return nLowerLimit + randuint(nUpperLimit - nLowerLimit);
	}

	int32_t PixelEngine::randint() noexcept
	{
		return math::bit_cast<int32_t>(randuint());
	}

	int32_t PixelEngine::randint(const int32_t &nLowerLimit, const int32_t &nUpperLimit) noexcept
	{
		return math::bit_cast<int32_t>(randuint(nLowerLimit, nUpperLimit));
	}

	float PixelEngine::random() noexcept
	{
		return Lehmer32f(nLehmerSeed);
	}

	float PixelEngine::random(const float &fLimit) noexcept
	{
		return std::fmod(random(), fLimit);
	}

	float PixelEngine::random(const float &fLowerLimit, const float &fUpperLimit) noexcept
	{
		return fLowerLimit + random(fUpperLimit - fLowerLimit);
	}

	void PixelEngine::SetRenderTarget(const RenderTarget &renderTarget) const noexcept
	{
		renderTarget.bind();
		glViewport(0, 0, renderTarget.size().x, renderTarget.size().y);
	}

	void PixelEngine::ResetRenderTarget() const noexcept
	{
		RenderTarget::unbind();
		glViewport(0, 0, m_vScreenSize.x, m_vScreenSize.y);
	}

	void PixelEngine::DrawSprite(const Sprite2D &sprite, const vf2d &pos, const vf2d &size, const Pixel &tint) noexcept
	{
		Decal(sprite, pos, size).render(m_sSpriteShader, tint);
	}

	void PixelEngine::DrawSpriteWorld(const Sprite2D &sprite, const TransformedView &tv, const vf2d &pos, const vf2d &size, const Pixel &tint) noexcept
	{
		Decal(sprite, pos, size).renderWorld(m_sSpriteShader, tv, tint);
	}

	void PixelEngine::DrawSprite(const Sprite2D &sprite, const vf2d &tl, const vf2d &tr, const vf2d &bl, const vf2d &br, const Pixel &tint) noexcept
	{
		Decal(sprite, tl, tr, bl, br).render(m_sSpriteShader, tint);
	}

	void PixelEngine::DrawDecal(const Decal &decal, const Pixel &tint) noexcept
	{
		decal.render(m_sSpriteShader, tint);
	}

	void PixelEngine::DrawDecalWorld(const Decal &decal, const TransformedView &tv, const Pixel &tint) noexcept
	{
		decal.renderWorld(m_sSpriteShader, tv, tint);
	}

	void PixelEngine::DrawString(const std::string &sText, const vf2d &pos, const float &fScale, const Resolution &res, const Pixel &col) const noexcept
	{
		m_pStringTables[(uint32_t)res].bind();
		StringDecal(sText, pos, fScale).render(m_sSpriteShader, col);
	}

	void PixelEngine::DrawStringWorld(const std::string &sText, const TransformedView &tv, const vf2d &pos, const float &fScale, const Resolution &res, const Pixel &col) const noexcept
	{
		m_pStringTables[(uint32_t)res].bind();
		StringDecal(sText, pos, fScale).renderWorld(m_sSpriteShader, tv, col);
	}

	void PixelEngine::DrawStringDecal(const StringDecal &decal, const Pixel &col, const Resolution &res) const noexcept
	{
		m_pStringTables[(uint32_t)res].bind();
		decal.render(m_sSpriteShader, col);
	}

	void PixelEngine::DrawStringDecalWorld(const StringDecal &decal, const TransformedView &tv, const Pixel &col, const Resolution &res) const noexcept
	{
	}

	float PixelEngine::getAspectRatio() const noexcept
	{
		return m_fAspectRatio;
	}

	vf2d PixelEngine::GetStringSize(const std::string &s) noexcept
	{
		return StringDecal::getStringSize(s);
	}

	void PixelEngine::Start2D() const noexcept
	{
		glDisable(GL_DEPTH_TEST);
	}

	void PixelEngine::End2D() const noexcept
	{
		glEnable(GL_DEPTH_TEST);
	}

#ifdef _WIN32
	void PixelEngine::HideConsole() noexcept
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	void PixelEngine::ShowConsole() noexcept
	{
		ShowWindow(GetConsoleWindow(), SW_SHOW);
	}

#elif __linux__
	// not yet supported
	void PixelEngine::HideConsole() noexcept
	{
	}

	// not yet supported
	void PixelEngine::ShowConsole() noexcept
	{
	}
#endif

	void PixelEngine::UpdateInternals() noexcept { }

	void PixelEngine::RenderInternals() noexcept { }

	bool PixelEngine::ScanHardware() noexcept
	{
		m_vKeyManager.updateKeys();

		// reset mouse wheel movement
		m_relativeMouseMovement = vf2d();
		m_fWheelMovement = 0.0f;

		auto mapContains = [&](const uint32_t &key) -> bool
		{
#ifdef MAP_HAS_CONTAINS
			return m_mapKeys.contains(key);
#else
			for (const auto &entry : m_mapKeys)
				if (entry.first == key)
					return true;
			return false;
#endif
		};

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return false;
			}
			// Key pressed
			else if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
			{
				uint32_t nKey = event.key.keysym.sym;

				if (mapContains(nKey))
				{
					Key key = m_mapKeys.at(nKey);
					m_vKeyManager.updateKeyPress(key);
				}
#ifdef DO_DEBUG_MESSAGES
				else
				{
					std::cout << "Key " << nKey << " is not implemented yet" << std::endl;
				}
#endif
			}
			// Key released
			else if (event.type == SDL_KEYUP)
			{
				uint32_t nKey = event.key.keysym.sym;

				if (mapContains(nKey))
				{
					Key key = m_mapKeys.at(nKey);
					m_vKeyManager.updateKeyRelease(key);
				}
#ifdef DO_DEBUG_MESSAGES
				else
				{
					std::cout << "Key " << nKey << " is not implemented yet" << std::endl;
				}
#endif
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
					m_vKeyManager.updateKeyPress(Key::MOUSE_LEFT);
					break;
				case SDL_BUTTON_RIGHT:
					m_vKeyManager.updateKeyPress(Key::MOUSE_RIGHT);
					break;
				case SDL_BUTTON_MIDDLE:
					m_vKeyManager.updateKeyPress(Key::MOUSE_MIDDLE);
					break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
					m_vKeyManager.updateKeyRelease(Key::MOUSE_LEFT);
					break;
				case SDL_BUTTON_RIGHT:
					m_vKeyManager.updateKeyRelease(Key::MOUSE_RIGHT);
					break;
				case SDL_BUTTON_MIDDLE:
					m_vKeyManager.updateKeyRelease(Key::MOUSE_MIDDLE);
					break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				m_relativeMouseMovement = { (float)event.motion.xrel, (float)event.motion.yrel };
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					m_fWheelMovement = 1.0f;
				else
					m_fWheelMovement = -1.0f;
			}
		}

		int x, y;
		SDL_GetMouseState(&x, &y);

		m_vMousePos = toOpenGL(vf2d(static_cast<const float>(x), static_cast<const float>(y)));

		return true;
	}

	void PixelEngine::UpdateTitle() noexcept
	{
		m_fTimeToSetTitle -= m_fElapsedTime;
		m_nFramesSinceUpdate++;

		if (m_fTimeToSetTitle <= 0.0f)
		{
			SDL_SetWindowTitle(m_window, ("C++ Engine - " + m_sAppName + " - " + std::to_string(m_nFramesSinceUpdate) + "fps").c_str());

			m_nFPS = m_nFramesSinceUpdate;
			m_nFramesSinceUpdate = 0;
			m_fTimeToSetTitle = m_fTimeUpdateTitle;
		}
	}
}