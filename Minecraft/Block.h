#pragma once

#include <cstdint>

constexpr uint16_t g_nIdMask            = 0b1111'1111'1100'0000;
constexpr uint16_t g_nIdShiftBits       = 6;
constexpr uint16_t g_nDataMask          = 0b0000'0000'0011'1100;
constexpr uint16_t g_nDataShiftBits     = 2;
constexpr uint16_t g_nRotationMask      = 0b0000'0000'0000'0011;
constexpr uint16_t g_nRotationShiftBits = 0;

class Block
{
	// bit 00,01 (02 bits): rotation
	// bit 02-05 (04 bits): data
	// bit 06-16 (10 bits): id
	uint16_t m_data;

public:
	Block() noexcept = default;

	explicit Block(const uint16_t data) noexcept
	{
		m_data = data;
	}

	Block(const uint16_t id, const uint16_t data, const uint16_t rotation) noexcept
	{
		setId(id);
		setData(data);
		setRotation(rotation);
	}

public:
	uint16_t getId() const noexcept
	{
		return (m_data & g_nIdMask) >> g_nIdShiftBits;
	}

	void setId(const uint16_t id) noexcept
	{
		m_data &= ~g_nIdMask;
		m_data |= (id << g_nIdShiftBits);
	}

	uint8_t getData() const noexcept
	{
		return (m_data & g_nDataMask) >> g_nDataShiftBits;
	}

	void setData(const uint16_t data) noexcept
	{
		m_data &= ~g_nDataMask;
		m_data |= (data << g_nDataShiftBits);
	}

	uint16_t getRotation() const noexcept
	{
		return (m_data & g_nRotationMask) >> g_nRotationShiftBits;
	}

	void setRotation(const uint16_t rotation) noexcept
	{
		m_data &= ~g_nRotationMask;
		m_data |= (rotation << g_nRotationShiftBits);
	}

	uint16_t data() const noexcept
	{
		return m_data;
	}
};
