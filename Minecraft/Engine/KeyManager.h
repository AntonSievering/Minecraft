#pragma once

#include <array>
#include "Defines.h"

namespace Engine
{
	class KeyManager
	{
		using KeyArray = std::array<HWKey, (size_t)Key::Count>;

	private:
		KeyArray m_vKeys{};

	public:
		KeyManager() noexcept = default;

		KeyManager(const KeyArray &keys) noexcept
		{
			setKeys(keys);
		}

	public:
		void setKeys(const KeyArray &keys) noexcept
		{
			m_vKeys = keys;
		}

		void resetKeys() noexcept
		{
			for (int i = 0; i < m_vKeys.size(); i++)
				m_vKeys.at(i) = HWKey();
		}

		HWKey getKey(const Key &key) const noexcept
		{
			return m_vKeys.at((uint32_t)key);
		}

		void updateKeyPress(const Key &index) noexcept
		{
			HWKey *key = &m_vKeys.at((uint32_t)index);

			key->bPressed = true;
			key->bHeld = true;
		}

		void updateKeyRelease(const Key &index) noexcept
		{
			HWKey *key = &m_vKeys.at((uint32_t)index);

			key->bReleased = true;
			key->bHeld = false;
		}

		void updateKeys() noexcept
		{
			for (uint32_t i = 0; i < m_vKeys.size(); i++)
			{
				HWKey *key = &m_vKeys.at(i);

				key->bPressed = false;
				key->bReleased = false;
			}
		}
	};
}
