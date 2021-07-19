#pragma once

#include "Entity.h"
#include "Gamemode.h"
#include "Physics.h"

constexpr Engine::vf3d g_vPlayerSize          = Engine::vf3d(0.7f, 1.8f, 0.7f);
constexpr Engine::vf3d g_vPlayerFeetEyeOffset = Engine::vf3d(0.5f * g_vPlayerSize.x, g_vPlayerSize.y - 0.10f, 0.5f * g_vPlayerSize.z);

struct Player : public Entity
{
	std::string sName{};
	bool bFlying = false;

private:
	const Gamemode *m_pGamemode = nullptr;

public:
	Player() noexcept = default;

	Player(const std::string &sName, const Gamemode *ptrGamemode, const Engine::vf3d &pos) noexcept
		: Entity(pos, g_vPlayerSize)
	{
		this->sName = sName;
		m_pGamemode = ptrGamemode;
		bFlying = ptrGamemode->canFly();
	}

public:
	void update(float fElapsedTime, Engine::vf3d vTargetVel) noexcept
	{
		if (!m_pGamemode->canFly() || bGrounded)
			bFlying = false;

		float fAccelerationFactor = 1.0f;

		if (bFlying)
		{
			fAccelerationFactor = m_pGamemode->flyAccelerationMultiplier();
		}
		else
		{
			if (bGrounded)
				fAccelerationFactor = m_pGamemode->walkAccelerationMultiplier();
			else
				fAccelerationFactor = m_pGamemode->fallingAccelerationMultiplier();

			vel.y -= fElapsedTime * 2.0f * g_fGravity;
			vTargetVel.y = vel.y;
		}

		float fScaling = fAccelerationFactor * fElapsedTime;
		vel = vTargetVel * fScaling + vel * (1.0f - fScaling);
		hitbox.pos += vel * fElapsedTime;

		bGrounded = false;
	}

	void jump() noexcept
	{
		if (bGrounded)
		{
			float fJumpHeight = 1.25f;
			float fJumpSpeed = 2.0f * std::sqrtf(g_fGravity * fJumpHeight);
			vel.y = fJumpSpeed;
		}
	}

	Engine::vf3d getEyePosition() const noexcept
	{
		return hitbox.pos + g_vPlayerFeetEyeOffset;
	}

	void toggleFlying() noexcept
	{
		bFlying = !bFlying;
	}
};
