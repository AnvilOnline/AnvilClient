#pragma once
#include <cstdint>

namespace Blam::Math
{
	template <typename T>
	struct BasePoint2D
	{
		T X;
		T Y;

		BasePoint2D()
			: BasePoint2D(T(), T())
		{
		}

		BasePoint2D(const T p_X, const T p_Y)
			: X(p_X), Y(p_Y)
		{
		}

		bool operator==(const BasePoint2D<T> &p_Other) const
		{
			return (X == p_Other.X)
				&& (Y == p_Other.Y);
		}

		bool operator!=(const BasePoint2D<T> &p_Other) const
		{
			return !(*this == p_Other);
		}

		explicit operator const T *() const
		{
			return &X;
		}

		BasePoint2D<T> &operator+=(const BasePoint2D<T> &p_Other)
		{
			X += p_Other.X;
			Y += p_Other.Y;

			return *this;
		}

		BasePoint2D<T> &operator+=(const T p_Other)
		{
			X += p_Other;
			Y += p_Other;

			return *this;
		}

		BasePoint2D<T> operator+(const BasePoint2D<T> &p_Other) const
		{
			return BasePoint2D<T>(X + p_Other.X, Y + p_Other.Y);
		}

		BasePoint2D<T> operator+(const T p_Other) const
		{
			return BasePoint2D<T>(X + p_Other, Y + p_Other);
		}

		friend BasePoint2D<T> operator+(const T a, const BasePoint2D<T> &b)
		{
			return BasePoint2D<T>(a + b.X, a + b.Y);
		}

		BasePoint2D<T> &operator-=(const BasePoint2D<T> &p_Other)
		{
			X -= p_Other.X;
			Y -= p_Other.Y;

			return *this;
		}

		BasePoint2D<T> &operator-=(const T p_Other)
		{
			X -= p_Other;
			Y -= p_Other;

			return *this;
		}

		BasePoint2D<T> operator-(const BasePoint2D<T> &p_Other) const
		{
			return BasePoint2D<T>(X - p_Other.X, Y - p_Other.Y);
		}

		BasePoint2D<T> operator-(const T p_Other) const
		{
			return BasePoint2D<T>(X - p_Other, Y - p_Other);
		}

		friend BasePoint2D<T> operator-(const T a, const BasePoint2D<T> &b)
		{
			return BasePoint2D<T>(a - b.X, a - b.Y);
		}

		BasePoint2D<T> &operator*=(const BasePoint2D<T> &p_Other)
		{
			X *= p_Other.X;
			Y *= p_Other.Y;

			return *this;
		}

		BasePoint2D<T> &operator*=(const T p_Other)
		{
			X *= p_Other;
			Y *= p_Other;

			return *this;
		}

		BasePoint2D<T> operator*(const BasePoint2D<T> &p_Other) const
		{
			return BasePoint2D<T>(X * p_Other.X, Y * p_Other.Y);
		}

		BasePoint2D<T> operator*(const T p_Other) const
		{
			return BasePoint2D<T>(X * p_Other, Y * p_Other);
		}

		friend BasePoint2D<T> operator*(const T a, const BasePoint2D<T> &b)
		{
			return BasePoint2D<T>(a * b.X, a * b.Y);
		}

		BasePoint2D<T> &operator/=(const BasePoint2D<T> &p_Other)
		{
			X /= p_Other.X;
			Y /= p_Other.Y;

			return *this;
		}

		BasePoint2D<T> &operator/=(const T p_Other)
		{
			X /= p_Other;
			Y /= p_Other;

			return *this;
		}

		BasePoint2D<T> operator/(const BasePoint2D<T> &p_Other) const
		{
			return BasePoint2D<T>(X / p_Other.X, Y / p_Other.Y);
		}

		BasePoint2D<T> operator/(const T p_Other) const
		{
			return BasePoint2D<T>(X / p_Other, Y / p_Other);
		}

		friend BasePoint2D<T> operator/(const T a, const BasePoint2D<T> &b)
		{
			return BasePoint2D<T>(a / b.X, a / b.Y);
		}
	};
}
