#pragma once

// link with opengl32.lib, SDL2.lib & glew32s.lib
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")

// STL
#include <string>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <fstream>
#include <chrono>
#include <array>
#include <map>

#include "Defines.h"
#include "Sprite2D.h"
#include "SpriteShader.h"
#include "StringDecal.h"
#include "util.h"
#include "Timer.h"
#include "RenderTarget.h"
#include "MathHelper.h"
#include "KeyManager.h"
#include "Decal.h"
#include "TransformedView.h"

#include "deps/stb_image.h"
#include "deps/stb_image_write.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#if defined(_DEBUG) || defined(DEBUG)
#define DO_DEBUG_MESSAGES
#endif

// Check for C++20
#if _MSVC_LANG >= 201705L || __cplusplus >= 201705L
#define MAP_HAS_CONTAINS
#endif

namespace Engine
{
	class PixelEngine
	{
	public:
		using clock = Timer::clock;
		using time_point = Timer::time_point;

	public:
		// SDL Window which OpenGL draws to
		SDL_Window *m_window = nullptr;
		SDL_GLContext m_glContext{};

		// Timing
		static constexpr float m_fTimeUpdateTitle = 1.0f;
		uint64_t               m_nFPS = 0;
		uint64_t               m_nFramesSinceUpdate = 0;
		float                  m_fTimeSinceStart = 0.0f;
		float                  m_fElapsedTime = 0.0f;
		float                  m_fTimeToSetTitle = 0.0f;

		// input
		KeyManager              m_vKeyManager{};
		std::map<uint32_t, Key> m_mapKeys{};
		float                   m_fWheelMovement = 0.0f;

		SpriteShader  m_sSpriteShader{};
		Sprite2D      m_pStringTables[3]{};

		uint32_t nLehmerSeed = 0;
		vf2d    m_relativeMouseMovement = { 0.0f, 0.0f };

	protected:
		// Grafics Card information:
		std::string m_sOpenGLVersion{};
		std::string m_sGraficsCard{};
		std::string m_sGraficsCardVendor{};

		vi2d        m_vScreenSize{};
		float       m_fAspectRatio = 0.0f;
		vf2d        m_vMousePos{};
		std::string m_sAppName = "[define m_sAppName to rename this title]";

	public:
		PixelEngine() noexcept {}

	protected:
		bool SetRefreshRate(RefreshRate refreshRate) noexcept;

	private:
		// Need to be overriden
		virtual bool OnUserCreate() noexcept;
		virtual bool OnUserUpdate(float fElapsedTime) noexcept;
		virtual bool OnUserDestroy() noexcept;

	public:
		bool     Construct(vi2d size, FullscreenMode fullscreen, RefreshRate refresh, int subSamples = 1) noexcept; // Prepares the Engine, returns true when succeeds
		void     Start() noexcept; // Starts the Engine

	public:
		virtual HWKey GetKey(const Key &key)  const noexcept; // Returns the key states of the requested Key
		vf2d     GetMouse()                   const noexcept; // Returns the x and y mouse position in OpenGL space
		float    GetMouseX()                  const noexcept; // Returns the x mouse position in OpenGL space
		float    GetMouseY()                  const noexcept; // Returns the y mouse position in OpenGL space
		float    GetMouseWheel()              const noexcept; // Returns the normalized mouse wheel movement
		uint64_t GetFPS()                     const noexcept; // Returns the current FPS
		vf2d     GetRelativeMouse()           const noexcept; // Returns the mouse movement since the last frame
		float    GetRelativeMouseX()          const noexcept; // Returns the relative x mouse movement
		float    GetRelativeMouseY()          const noexcept; // Returns the relative y mouse movement
		void     HideMouse(const bool &bHide)       noexcept; // Returns 1: hides the mouse, 0: shows it
		bool     GetHideMouseStatus()         const noexcept; // Returns whether the mouse is currently hided
		vi2d     GetScreenSize()              const noexcept; // Returns the size of the screen in pixels

		vf2d     toOpenGL(const vf2d &pos)    const noexcept; // Translates screen to OpenGL space

		float    GetElapsedTime()             const noexcept; // Returns the elapsed time
		float    GetTimeSinceStart()          const noexcept; // Returns the time since the start of the engine

		// random number generator
		uint32_t randuint() noexcept;
		uint32_t randuint(const uint32_t &nLimit) noexcept;
		uint32_t randuint(const uint32_t &nLowerLimit, const uint32_t &nUpperLimit) noexcept;
		int32_t randint() noexcept;
		int32_t randint(const int32_t &nLowerLimit, const int32_t &nUpperLimit) noexcept;
		float random() noexcept;
		float random(const float &fLimit) noexcept;
		float random(const float &fLowerLimit, const float &fUpperLimit) noexcept;

		void SetRenderTarget(const RenderTarget &renderTarget) const noexcept;
		void ResetRenderTarget() const noexcept;

		// 2D Routines
	public:
		void Clear(const Pixel &col, int bDepthBufferBit = 0) noexcept;
		void ClearDepthBuffer() noexcept;

		// Draw Routines for textured shapes
		void DrawSprite(const Sprite2D &sprite, const vf2d &pos, const vf2d &size, const Pixel &tint = WHITE) noexcept;
		void DrawSpriteWorld(const Sprite2D &sprite, const TransformedView &tv, const vf2d &pos, const vf2d &size, const Pixel &tint = WHITE) noexcept;
		void DrawSprite(const Sprite2D &sprite, const vf2d &tl, const vf2d &tr, const vf2d &bl, const vf2d &br, const Pixel &tint = WHITE) noexcept;
		void DrawDecal(const Decal &decal, const Pixel &tint = WHITE) noexcept;
		void DrawDecalWorld(const Decal &decal, const TransformedView &tv, const Pixel &tint = WHITE) noexcept;

		// Draw Routine for Text
		void DrawString(const std::string &sText, const vf2d &pos, const float &fScale, const Resolution &res, const Pixel &col = WHITE) const noexcept;
		void DrawStringWorld(const std::string &sText, const TransformedView &tv, const vf2d &pos, const float &fScale, const Resolution &res, const Pixel &col = WHITE) const noexcept;
		void DrawStringDecal(const StringDecal &decal, const Pixel &col = WHITE, const Resolution &res = Resolution::MEDIUM) const noexcept;
		void DrawStringDecalWorld(const StringDecal &decal, const TransformedView &tv, const Pixel &col = WHITE, const Resolution &res = Resolution::MEDIUM) const noexcept;

		float getAspectRatio() const noexcept;

		void Start2D() const noexcept;
		void End2D() const noexcept;

		// Show / Hide Console
	protected:
		static void HideConsole() noexcept;
		static void ShowConsole() noexcept;

	public:
		static vf2d GetStringSize(const std::string &s) noexcept;

	protected:
		virtual void UpdateInternals() noexcept;
		virtual void RenderInternals() noexcept;

	private:
		bool ScanHardware() noexcept;
		void UpdateTitle() noexcept;
	};
}
