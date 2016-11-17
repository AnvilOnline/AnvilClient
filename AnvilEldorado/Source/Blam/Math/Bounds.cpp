#include "Bounds.hpp"

namespace Blam::Math
{
	template <typename T>
	Bounds<T>::Bounds()
		: Bounds(T(), T())
	{
	}

	template <typename T>
	Bounds<T>::Bounds(const T &lower, const T &upper)
		: Lower(lower), Upper(upper)
	{
	}

	template <typename T>
	bool Bounds<T>::operator==(const Bounds<T> &other) const
	{
		return (Lower == other.Lower)
			&& (Upper == other.Upper);
	}

	template <typename T>
	bool Bounds<T>::operator!=(const Bounds<T> &other) const
	{
		return !(*this == other);
	}
}
