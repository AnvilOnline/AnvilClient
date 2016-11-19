#include "BitStream.hpp"

namespace Blam::Network
{
	template<class T>
	int32_t CountBits(T p_Value)
	{
		int32_t result = 0;

		while (p_Value != 0)
		{
			p_Value >>= 1;
			result++;
		}

		return result;
	}

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

	template<class T>
	T BitStream::ReadUnsigned(const int32_t p_Bits)
	{
		return static_cast<T>(ReadBits(p_Bits));
	}

	template<class T>
	T BitStream::ReadUnsigned(const T p_Minimum, const T p_Maximum)
	{
		return static_cast<T>(ReadUnsigned<T>(CountBits(p_Maximum - p_Minimum)));
	}

	void BitStream::ReadBlock(const size_t bits, uint8_t *out)
	{
		reinterpret_cast<void(__thiscall *)(BitStream *, uint8_t *, size_t)>(0x558740)(this, out, bits);
	}

	template<class CharType, const size_t MaximumSize>
	bool BitStream::ReadString(CharType(&p_String)[MaximumSize])
	{
		// Length
		auto s_Length = ReadUnsigned<size_t>(0U, MaximumSize - 1);
		if (s_Length >= MaximumSize)
			return false;

		// String
		memset(&p_String, 0, MaximumSize * sizeof(CharType));
		ReadBlock(s_Length * sizeof(CharType) * 8, reinterpret_cast<uint8_t*>(&p_String));
		return true;
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

	template<class T>
	void BitStream::WriteUnsigned(const T p_Value, const int32_t p_Bits)
	{
		WriteBits(static_cast<uint64_t>(p_Value), p_Bits);
	}

	template<class T>
	void BitStream::WriteUnsigned(const T p_Value, const T p_Minimum, const T p_Maximum)
	{
		WriteUnsigned(p_Value - p_Minimum, CountBits(p_Maximum - p_Minimum));
	}

	void BitStream::WriteBlock(const size_t p_Bits, const uint8_t *p_Data)
	{
		reinterpret_cast<void(__thiscall *)(BitStream *, const uint8_t *, size_t, int)>(0x55A000)(this, p_Data, p_Bits, 0);
	}

	template<class CharType, const size_t MaximumSize>
	void BitStream::WriteString(const CharType(&p_String)[MaximumSize])
	{
		// Compute length
		size_t s_Length = 0;
		while (s_Length < MaximumSize - 1 && str[s_Length])
			s_Length++;

		// Write length
		WriteUnsigned<uint64_t>(s_Length, 0U, MaximumSize - 1);

		// Write string
		WriteBlock(s_Length * sizeof(CharType) * 8, reinterpret_cast<const uint8_t*>(&p_String));
	}
}