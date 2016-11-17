#pragma once

namespace Blam::Math
{
	template <typename T>
	struct Bounds
	{
		T Lower;
		T Upper;

		Bounds();
		Bounds(const T &lower, const T &upper);

		bool operator==(const Bounds<T> &other) const;
		bool operator!=(const Bounds<T> &other) const;
	};
}