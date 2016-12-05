#include "BitStream.hpp"

namespace Blam::Data
{
	BitStream::BitStream()
		: Start(nullptr), End(nullptr), Unknown14(nullptr), CurrentPointer(nullptr)
	{
	}

	int BitStream::GetAvailableBits() const
	{
		return sizeof(Window) * 8 - WindowBitsUsed;
	}

	bool BitStream::BitsAvailable(const int32_t p_Bits) const
	{
		return p_Bits <= GetAvailableBits();
	}

	bool BitStream::ReadBit()
	{
		return reinterpret_cast<bool(__thiscall *)(BitStream *)>(0x558570)(this);
	}

	uint64_t BitStream::ReadBits(const int32_t p_Bits)
	{
		if (p_Bits <= 32)
		{
			return reinterpret_cast<uint32_t(__thiscall *)(BitStream *, int)>(0x5589A0)(this, p_Bits);
		}
		
		return reinterpret_cast<uint64_t(__thiscall *)(BitStream *, int)>(0x559160)(this, p_Bits);
	}

	bool BitStream::ReadBool()
	{
		return ReadBit();
	}

	void BitStream::ReadBlock(const size_t bits, uint8_t *out)
	{
		reinterpret_cast<void(__thiscall *)(BitStream *, uint8_t *, size_t)>(0x558740)(this, out, bits);
	}

	void BitStream::WriteBits(const uint64_t p_Value, const int32_t p_Bits)
	{
		if (BitsAvailable(p_Bits))
		{
			Position += p_Bits;
			WindowBitsUsed += p_Bits;
			Window <<= p_Bits;
			Window |= p_Value & (0xFFFFFFFFFFFFFFFF >> (64 - p_Bits));
		}
		else
		{
			return reinterpret_cast<void(__thiscall *)(BitStream *, uint64_t, int)>(0x559EB0)(this, p_Value, p_Bits);
		}
	}

	void BitStream::WriteBool(const bool p_Value)
	{
		WriteUnsigned(p_Value, 1);
	}

	void BitStream::WriteBlock(const size_t p_Bits, const uint8_t *p_Data)
	{
		reinterpret_cast<void(__thiscall *)(BitStream *, const uint8_t *, size_t, int)>(0x55A000)(this, p_Data, p_Bits, 0);
	}
}