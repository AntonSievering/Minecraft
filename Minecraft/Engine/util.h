#pragma once
#include <cstdint>

constexpr static uint32_t Lehmer32_Kernel(const uint32_t &nSeed)
{
	uint64_t temp = (uint64_t)nSeed * 0x4a39b70d;
	uint32_t m1   = (uint32_t)((temp >> 32) ^ temp);
	temp          = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2   = (uint32_t)((temp >> 32) ^ temp);
	return m2;
}

// range: 0 -> (1 << 32) - 1
constexpr static uint32_t Lehmer32(uint32_t &nSeed)
{
	nSeed += 0xe120fc15;
	return Lehmer32_Kernel(nSeed);
}

constexpr static float Lehmer32f_Kernel(const uint32_t &nSeed)
{
	return (float)Lehmer32_Kernel(nSeed) / (float)UINT32_MAX;
}

// range: 0.0f -> 1.0f
constexpr static float Lehmer32f(uint32_t &nSeed)
{
	return (float)Lehmer32(nSeed) / (float)UINT32_MAX;
}
