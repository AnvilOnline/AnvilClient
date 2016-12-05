#pragma once
#include <cstdint>
#include "Utils\Util.hpp"

namespace Blam::Data
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
		T ReadUnsigned(const int32_t p_Bits)
		{
			return static_cast<T>(ReadBits(p_Bits));
		}

		template<class T>
		T ReadUnsigned(const T p_Minimum, const T p_Maximum)
		{
			return static_cast<T>(ReadUnsigned<T>(AnvilCommon::Utils::Util::CountBits(p_Maximum - p_Minimum)));
		}

		void ReadBlock(const size_t p_Bits, uint8_t *p_Out);

		template<class CharType, const size_t MaximumSize>
		bool ReadString(CharType(&p_String)[MaximumSize])
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

		void WriteBits(const uint64_t p_Value, const int32_t p_Bits);

		void WriteBool(const bool p_Value);

		template<class T>
		void WriteUnsigned(const T p_Value, const int32_t p_Bits)
		{
			WriteBits(static_cast<uint64_t>(p_Value), p_Bits);
		}

		template<class T>
		void WriteUnsigned(const T p_Value, const T p_Minimum, const T p_Maximum)
		{
			WriteUnsigned(p_Value - p_Minimum, AnvilCommon::Utils::Util::CountBits(p_Maximum - p_Minimum));
		}

		void WriteBlock(const size_t p_Bits, const uint8_t *p_Data);

		template<class CharType, const size_t MaximumSize>
		void WriteString(const CharType(&p_String)[MaximumSize])
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
	};
}