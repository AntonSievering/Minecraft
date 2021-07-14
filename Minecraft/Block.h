#pragma once

#include <cstdint>
#include "Facing.h"
#include "BlockId.h"

class Block
{
	BlockId m_blockId = BlockId::AIR;
	Facing m_facing = Facing::NORTH;

public:
	Block() noexcept = default;

	explicit Block(const BlockId id) noexcept
	{
		m_blockId = id;
	}

	Block(const BlockId id, const Facing facing) noexcept
	{
		m_blockId = id;
		m_facing = facing;
	}

public:
	BlockId getId() const noexcept
	{
		return m_blockId;
	}

	void setId(const BlockId id) noexcept
	{
		m_blockId = id;
	}

	Facing getFacing() const noexcept
	{
		return m_facing;
	}

	void setFacing(const Facing facing) noexcept
	{
		m_facing = facing;
	}

	bool isFullyOpaque() const noexcept
	{
		return m_blockId != BlockId::AIR;
	}
};
