#pragma once

#include <cstdint>
#include <array>
#include <string>

enum class FaceId : uint16_t
{
	STONE = 0,
	DIRT,
	GRASS_SIDE,
	GRASS_TOP,
	
	Count
};

struct FaceFilenames
{
private:
	static inline const std::array<std::string, (uint16_t)FaceId::Count> data = {
		/* FaceId::STONE      */ "stone",
		/* FaceId::DIRT       */ "dirt",
		/* FaceId::GRASS_SIDE */ "grass_block_side",
		/* FaceId::GRASS_TOP  */ "grass_block_top"
	};

public:
	static std::string getFilename(const FaceId id) noexcept
	{
		return data[(uint32_t)id];
	}
};
