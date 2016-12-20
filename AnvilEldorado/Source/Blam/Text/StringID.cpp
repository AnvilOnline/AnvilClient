#include "StringID.hpp"

namespace Blam::Text
{
	const StringID StringID::Null = 0;

	StringID::StringID(const int32_t p_Length, const int32_t p_Set, const int32_t p_Index)
		: Value((uint32_t)(((p_Length & 0xFF) << 24) | ((p_Set & 0xFF) << 16) | (p_Index & 0xFFFF)))
	{
	}

	StringID::StringID(const int32_t p_Set, const int32_t p_Index)
		: StringID(0, p_Set, p_Index)
	{
	}

	StringID::StringID(const uint32_t p_Value)
		: Value(p_Value)
	{
	}

	int32_t StringID::GetLength() const
	{
		return (int32_t)((Value >> 24) & 0xFF);
	}

	int32_t StringID::GetSet() const
	{
		return (int32_t)((Value >> 16) & 0xFF);
	}

	int32_t StringID::GetIndex() const
	{
		return (int32_t)(Value & 0xFFFF);
	}

	bool StringID::operator==(const StringID &p_Other) const
	{
		return Value == p_Other.Value;
	}

	bool StringID::operator!=(const StringID &p_Other) const
	{
		return !(*this == p_Other);
	}

	StringID::operator uint32_t() const
	{
		return Value;
	}
}