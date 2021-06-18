#pragma once
#include "Sprite.h"
#include <map>

// Filesystem
#if _MSC_VER >= 1920 && _MSVC_LANG >= 201703L
#include <filesystem>
namespace fs = std::filesystem;
#else
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;
#endif

namespace Engine
{
	class SpriteManager
	{
		using SpriteMap = std::map<std::string, Engine::Sprite>;
		using SpriteEntry = std::pair<std::string, Sprite>;
		using DirectoryIterator = fs::directory_iterator;
		using DirectoryEntry = const fs::directory_entry;

	private:
		SpriteMap m_sprites;

	public:
		SpriteManager() noexcept = default;

	public:
		bool contains(const std::string &sFilename) const noexcept
		{
#if _MSVC_LANG >= 201705L
			return m_sprites.contains(sFilename);
#else
			for (const auto &entry : m_sprites)
				if (entry.first == sFilename)
					return true;

			return false;
#endif
		}

		void load(const std::string &sFilename) noexcept
		{
			if (!contains(sFilename))
				m_sprites.insert(std::make_pair(sFilename, Sprite(sFilename)));
		}

		Sprite query(const std::string &sFilename) const noexcept
		{
			if (contains(sFilename))
				return m_sprites.at(sFilename);

#ifdef _DEBUG
			std::cout << "file " << sFilename << " not loaded\n";
#endif

			return Sprite();
		}

		void loadDirectory(const std::string &sFilename) noexcept
		{
			auto replace = [](std::string sInput) -> std::string
			{
				for (char &c : sInput)
					if (c == '\\') c = '/';
				return sInput;
			};

			for (DirectoryEntry &entry : DirectoryIterator(sFilename))
				load(replace(entry.path().string()));
		}
	};
}
