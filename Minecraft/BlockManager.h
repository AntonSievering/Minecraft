#pragma once

#include "BlockId.h"
#include "FaceId.h"
#include <array>
#include <string>

enum class RotationAxis
{
	NONE = 0,
	XY,
	XYZ
};

struct BlockFaces
{
	// assuming block facing == north
	// without rotation
	FaceId north, south, east, west, top, bottom;
};

struct BlockEntry
{
	BlockFaces faces;
	RotationAxis axis;
};

static struct BlockManager
{
private:
	static inline const std::array<BlockEntry, (size_t)BlockId::Count> data = {
		/* BlockId::AIR   */ BlockEntry{ {}, RotationAxis::NONE },
		/* BlockId::STONE */ BlockEntry{ { FaceId::STONE,      FaceId::STONE,      FaceId::STONE,      FaceId::STONE,      FaceId::STONE,     FaceId::STONE }, RotationAxis::NONE },
		/* BlockId::DIRT  */ BlockEntry{ { FaceId::DIRT,       FaceId::DIRT,       FaceId::DIRT,       FaceId::DIRT,       FaceId::DIRT,      FaceId::DIRT  }, RotationAxis::NONE },
		/* BlockId::GRASS */ BlockEntry{ { FaceId::GRASS_SIDE, FaceId::GRASS_SIDE, FaceId::GRASS_SIDE, FaceId::GRASS_SIDE, FaceId::GRASS_TOP, FaceId::DIRT  }, RotationAxis::NONE }
	};

public:
	static inline BlockEntry getBlockEntry(const BlockId id) noexcept
	{
		return data[(size_t)id];
	}
};
