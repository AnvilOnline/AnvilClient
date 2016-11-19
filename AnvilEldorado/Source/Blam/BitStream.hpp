#pragma once
#include <cstdint>

namespace Blam
{
	struct BitStream
	{
		uint8_t *Start;
		uint8_t *End;
		uint32_t Size;
		uint32_t UnknownC;
		uint32_t Unknown10;
		void *Unknown14;
		uint32_t Unknown18;
		uint32_t Position;
		uint64_t Window;
		int WindowBitsUsed;
		uint8_t *CurrentPointer;
		uint32_t Unknown30;
		uint8_t Unknown34[100];
		uint32_t Unknown98;
		uint32_t Unknown9C;

		BitStream();

		int GetAvailableBits() const;
		bool BitsAvailable(const int32_t p_Bits) const;

		bool ReadBit();
		uint64_t ReadBits(const int32_t p_Bits);

		bool ReadBool();

		template<class T>
		T ReadUnsigned(const int32_t p_Bits);

		template<class T>
		T ReadUnsigned(const T p_Minimum, const T p_Maximum);

		void ReadBlock(const size_t p_Bits, uint8_t *p_Out);

		template<class CharType, const size_t MaximumSize>
		bool ReadString(CharType(&p_String)[MaximumSize]);

		void WriteBits(const uint64_t p_Value, const int32_t p_Bits);

		void WriteBool(const bool p_Value);

		template<class T>
		void WriteUnsigned(const T p_Value, const int32_t p_Bits);

		template<class T>
		void WriteUnsigned(const T p_Value, const T p_Minimum, const T p_Maximum);

		void WriteBlock(const size_t p_Bits, const uint8_t *p_Data);

		template<class CharType, const size_t MaximumSize>
		void WriteString(const CharType(&p_String)[MaximumSize]);
	};
}