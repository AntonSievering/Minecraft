#pragma once

#include <cmath>
#include <cstdint>

struct NoiseParameters
{
	int32_t nOctaves;
	int32_t nAmplitude;
	int32_t nSmoothness;
	int32_t nHeightOffset;
	float fRoughness;
};

class NoiseGenerator
{
private:
	NoiseParameters m_noiseParameters;
	int32_t m_seed;

public:
	NoiseGenerator() noexcept;
	NoiseGenerator(const int32_t &seed) noexcept;
	NoiseGenerator(const int32_t &seed, const NoiseParameters &params) noexcept;

private:
	float getNoise(int32_t n) const noexcept;
	float getNoise(const float &x, const float &z) const noexcept;
	float noise(const float &x, const float &z) const noexcept;
	float lerp(const float &a, const float &b, const float &z) const noexcept;

public:
	float getHeight(const int32_t &x, const int32_t &z, const int32_t &chunkX, const int32_t &chunkZ, const float &fFactor = 1.0f) const noexcept;
	void setParameters(const NoiseParameters &params) noexcept;
};
