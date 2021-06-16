#pragma once

#include "vec2d.h"
#include "Sprite.h"

namespace Engine
{
	class TextureAtlas
	{
	private:
		Sprite   sprite{};
		uint32_t nSlots{};
		vu2d     textureSize{};

	public:
		TextureAtlas() noexcept = default;
		
		TextureAtlas(const vu2d &textureSize, const uint32_t &nSlots, const GLint &min_filter = GL_LINEAR, const GLint &mag_filter = GL_NEAREST) noexcept
		{
			sprite = Sprite(vu2d(textureSize.x * nSlots, textureSize.y), min_filter, mag_filter);
			this->textureSize = textureSize;
			this->nSlots = nSlots;
		}

	public:
		void assign(const uint32_t &slotId, Sprite *texture) noexcept
		{
			if (slotId < nSlots)
			{
				uint32_t offset = slotId * textureSize.x;

				for (uint32_t y = 0; y < textureSize.y; y++)
					for (uint32_t x = 0; x < textureSize.x; x++)
						sprite.setPixel({ x + offset, y }, texture->getPixel({ x, y }));
			}
		}

		void finish() noexcept
		{
			sprite.reupload();
		}

		void bind() noexcept
		{
			sprite.bind();
		}

		Sprite getSprite() const noexcept
		{
			return sprite;
		}

		vf2d getTextureCoordinate(const uint32_t &slotId) const noexcept
		{
			return { (float)slotId / (float)nSlots + 0.0002f, (float)(slotId + 1.0f) / (float)nSlots - 0.0002f };
		}
	};
}
