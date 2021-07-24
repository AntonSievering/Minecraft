#pragma once

#include <string>
#include <iostream>

enum class GameModeType
{
	SURVIVAL = 0,
	CREATIVE,
	ADVENTURE,
	HARDCORE,
	SPECTATOR
};

class Gamemode
{
	GameModeType m_gameMode{};

public:
	Gamemode() noexcept = default;

	Gamemode(const GameModeType gamemode) noexcept
	{
		m_gameMode = gamemode;
	}

public:
	std::string getAsString() const noexcept
	{
		switch (m_gameMode)
		{
		case GameModeType::SURVIVAL:
			return "survival";
		case GameModeType::CREATIVE:
			return "creative";
		case GameModeType::ADVENTURE:
			return "adventure";
		case GameModeType::HARDCORE:
			return "hardcore";
		case GameModeType::SPECTATOR:
			return "spectator";
		}

		return "error";
	}

	GameModeType getType() const noexcept
	{
		return m_gameMode;
	}

public:
	bool canBreakBlocks() const noexcept
	{
		return m_gameMode == GameModeType::SURVIVAL || m_gameMode == GameModeType::CREATIVE || m_gameMode == GameModeType::HARDCORE;
	}

	bool canInsantlyBreakBlocks() const noexcept
	{
		return m_gameMode == GameModeType::CREATIVE;
	}

	bool canFly() const noexcept
	{
		return m_gameMode == GameModeType::CREATIVE || m_gameMode == GameModeType::SPECTATOR;
	}

	float flySpeedMultiplier() const noexcept
	{
		switch (m_gameMode)
		{
		case GameModeType::CREATIVE:
			return 1.0f;
		case GameModeType::SPECTATOR:
			return 2.0f;
		default:
			return 0.0f;
		}
	}

	float flyAccelerationMultiplier() const noexcept
	{
		return 2.0f;
	}

	float walkAccelerationMultiplier() const noexcept
	{
		return 15.0f;
	}

	float fallingAccelerationMultiplier() const noexcept
	{
		return 1.0f;
	}

	bool isInvincible() const noexcept
	{
		return m_gameMode == GameModeType::CREATIVE || m_gameMode == GameModeType::SPECTATOR;
	}

	bool canRespawn() const noexcept
	{
		return m_gameMode != GameModeType::HARDCORE;
	}
};
