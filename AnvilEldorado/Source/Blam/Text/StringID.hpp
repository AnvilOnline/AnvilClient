#pragma once
#include <cstdint>

namespace Blam::Text
{
	/// <summary>
	/// A constant ID representing a debug string.
	/// </summary>
	struct StringID
	{
		static const StringID Null;

		uint32_t Value;

		StringID(const int32_t p_Length, const int32_t p_Set, const int32_t p_Index)
			: Value((uint32_t)(((p_Length & 0xFF) << 24) | ((p_Set & 0xFF) << 16) | (p_Index & 0xFFFF)))
		{
		}

		StringID(const int32_t p_Set, const int32_t p_Index)
			: StringID(0, p_Set, p_Index)
		{
		}

		StringID(const uint32_t p_Value)
			: Value(p_Value)
		{
		}

		int32_t GetLength() const
		{
			return (int32_t)((Value >> 24) & 0xFF);
		}

		int32_t GetSet() const
		{
			return (int32_t)((Value >> 16) & 0xFF);
		}

		int32_t GetIndex() const
		{
			return (int32_t)(Value & 0xFFFF);
		}

		bool operator==(const StringID &p_Other) const
		{
			return Value == p_Other.Value;
		}

		bool operator!=(const StringID &p_Other) const
		{
			return !(*this == p_Other);
		}

		explicit operator uint32_t() const
		{
			return Value;
		}
	};
}