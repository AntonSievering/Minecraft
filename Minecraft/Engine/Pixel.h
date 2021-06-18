#pragma once
#include <cstdint>

namespace Engine
{
	struct Pixel
	{
	public:
		// RGBA or 32-bit unsigned int
		union
		{
			uint32_t v;
			struct
			{
				uint8_t r, g, b, a;
			};
		};

	public:
		Pixel()
		{
			v = 0;
		}

		explicit Pixel(const uint32_t &n)
		{
			v = n;
		}

		Pixel(int r, int g, int b, int a = 255)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

	public:
		bool operator == (const Pixel &rhs) const noexcept { return v == rhs.v; }
		bool operator != (const Pixel &rhs) const noexcept { return v != rhs.v; }
	};

	static Pixel PixelF(float r, float g, float b, float a = 1.0f)
	{
		Pixel col;
		col.r = static_cast<uint8_t>(255.0f * static_cast<float>(r));
		col.g = static_cast<uint8_t>(255.0f * static_cast<float>(g));
		col.b = static_cast<uint8_t>(255.0f * static_cast<float>(b));
		col.a = static_cast<uint8_t>(255.0f * static_cast<float>(a));

		return col;
	}
}
