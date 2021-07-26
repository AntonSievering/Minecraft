#include "NoiseGenerator.h"

#include <iostream>

NoiseGenerator::NoiseGenerator() noexcept
{
	m_seed = 0;
	m_noiseParameters = { 0, 0, 0, 0, 0.0f };
}

NoiseGenerator::NoiseGenerator(const int32_t &seed) noexcept
{
	m_seed = seed;
	m_noiseParameters = { 6, 100, 100, -20, 0.4f };
}

void NoiseGenerator::setParameters(const NoiseParameters &params) noexcept
{
	m_noiseParameters = params;
}

NoiseGenerator::NoiseGenerator(const int32_t &seed, const NoiseParameters &params) noexcept
{
	m_seed = seed;
	m_noiseParameters = params;
}

// range: -1.0f to 1.0f
float NoiseGenerator::getNoise(int32_t n) const noexcept
{
	n += m_seed;
	n = (n << 13) ^ n;
	int newN = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;

	return 1.0f - ((float)newN / 1073741824.0f);
}

float NoiseGenerator::getNoise(const float &x, const float &z) const noexcept
{
	return getNoise(x + z * 57.0f);
}

float NoiseGenerator::lerp(const float &a, const float &b, const float &z) const noexcept
{
	float mu2 = (1.0f - std::cos(z * 3.14159f)) / 2.0f;
	return (a * (1.0f - mu2) + b * mu2);
}

float NoiseGenerator::noise(const float &x, const float &z) const noexcept
{
	float floorX = floor(x);
	float floorZ = floor(z);

	float s = getNoise(floorX,        floorZ);
	float t = getNoise(floorX + 1.0f, floorZ);
	float u = getNoise(floorX,        floorZ + 1.0f);
	float v = getNoise(floorX + 1.0f, floorZ + 1.0f);

	float rec1 = lerp(s,    t,    x - floorX);
	float rec2 = lerp(u,    v,    x - floorX);
	float rec3 = lerp(rec1, rec2, z - floorZ);

	return rec3;
}

float NoiseGenerator::getHeight(const int32_t &x, const int32_t &z, const int32_t &chunkX, const int32_t &chunkZ, const float &fFactor) const noexcept
{
	float fX = (x + (chunkX * 16)) * fFactor;
	float fZ = (z + (chunkZ * 16)) * fFactor;

	float totalValue = 0.0f;

	for (int o = 0; o < m_noiseParameters.nOctaves - 1; o++)
	{
		float frequency = pow(2.0f, o);
		float amplitude = pow(m_noiseParameters.fRoughness, o);
		
		totalValue += noise(fX * frequency / m_noiseParameters.nSmoothness, fZ * frequency / m_noiseParameters.nSmoothness) * amplitude;
	}

	float val = (((totalValue / 2.1f) + 1.2f) * m_noiseParameters.nAmplitude) + m_noiseParameters.nHeightOffset;

	return std::max(val, 0.0f);
}
