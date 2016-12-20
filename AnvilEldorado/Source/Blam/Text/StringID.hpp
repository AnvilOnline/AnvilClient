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

		StringID(const uint32_t p_Value);
		StringID(const int32_t p_Set, const int32_t p_Index);
		StringID(const int32_t p_Length, const int32_t p_Set, const int32_t p_Index);

		int32_t GetLength() const;
		int32_t GetSet() const;
		int32_t GetIndex() const;

		bool operator==(const StringID &p_Other) const;
		bool operator!=(const StringID &p_Other) const;

		explicit operator uint32_t() const;
	};
}