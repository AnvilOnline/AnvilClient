#pragma once
#include <cstdint>

namespace Blam::Data
{
	// A unique handle used to refer to data.
	struct DatumIndex
	{
		// Represents a null datum index.
		static const DatumIndex Null;

		typedef uint16_t TSalt;  // Type of a salt value
		typedef uint16_t TIndex; // Type of an index value

								 // Creates a null datum index.
		DatumIndex() : Value(0xFFFFFFFF) { }

		// Creates a datum index from a handle.
		DatumIndex(uint32_t handle) : Value(handle) { }

		// Creates a datum index from a salt and an index.
		DatumIndex(TSalt salt, TIndex index)
		{
			Value = (salt << 16) | index;
		}

		// The value of the datum index as a 32-bit integer.
		uint32_t Value;

		// Gets the datum index's salt value.
		TSalt Salt() const { return Value >> 16; }

		// Gets the datum index's index value.
		TIndex Index() const { return Value & 0xFFFF; }

		bool operator==(const DatumIndex other) const { return Value == other.Value; }
		bool operator!=(const DatumIndex other) const { return !(*this == other); }
		operator uint32_t() const { return Value; }
		operator bool() const { return *this != Null; }
	};
	static_assert(sizeof(DatumIndex) == 4, "Invalid DatumIndex size");
}