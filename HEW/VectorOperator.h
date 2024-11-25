#pragma once

template<class T>
class CVector2
{
public:
	CVector2()
		:X(0.0f), Y(0.0f)
	{

	}


	CVector2 operator+(CVector2<T> inVector)
	{
		CVector2 ret;
		ret.X = X + inVector.X;
		ret.Y = Y + inVector.Y;

		return ret;
	}
	void operator+=(CVector2<T> inVector)
	{
		inVector.X += inVector.X;
		inVector.Y += inVector.Y;
	}
	CVector2 operator-(CVector2<T> inVector)
	{
		CVector2 ret;
		ret.X = X - inVector.X;
		ret.Y = Y - inVector.Y;

		return ret;
	}
	void operator-=(CVector2<T> inVector)
	{
		inVector.X -= inVector.X;
		inVector.Y -= inVector.Y;
	}
	CVector2 operator*(CVector2<T> inVector)
	{
		CVector2 ret;
		ret.X = X * inVector.X;
		ret.Y = Y * inVector.Y;

		return ret;
	}
	void operator*=(CVector2<T> inVector)
	{
		inVector.X *= inVector.X;
		inVector.Y *= inVector.Y;
	}
	CVector2 operator/(CVector2<T> inVector)
	{
		CVector2 ret;
		ret.X = X / inVector.X;
		ret.Y = Y / inVector.Y;

		return ret;
	}
	void operator/=(CVector2<T> inVector)
	{
		inVector.X /= inVector.X;
		inVector.Y /= inVector.Y;
	}
public:
	T X, Y;
};

template<class T>
class CVector3
{
public:
	CVector3()
	:X(0.0f), Y(0.0f), Z(0.0f)
	{

	}

	CVector3 operator+(CVector3 inVector)
	{
		CVector3 ret;
		ret.X = X + inVector.X;
		ret.Y = Y + inVector.Y;
		ret.Z = Z + inVector.Z;

		return ret;
	}
	void operator+=(CVector3<T> inVector)
	{
		inVector.X += inVector.X;
		inVector.Y += inVector.Y;
		inVector.Z += inVector.Z;
	}
	CVector3 operator-(CVector3<T> inVector)
	{
		inVector.X -= inVector.X;
		inVector.Y -= inVector.Y;
		inVector.Z -= inVector.Z;
	}
	void operator-=(CVector3<T> inVector)
	{
		CVector3 ret;
		ret.X = X - inVector.X;
		ret.Y = Y - inVector.Y;
		ret.Z = Z - inVector.Z;

		return ret;
	}
	CVector3 operator*(CVector3<T> inVector)
	{
		CVector3 ret;
		ret.X = X * inVector.X;
		ret.Y = Y * inVector.Y;
		ret.Z = Z * inVector.Z;

		return ret;
	}
	void operator*=(CVector3<T> inVector)
	{
		inVector.X *= inVector.X;
		inVector.Y *= inVector.Y;
		inVector.Z *= inVector.Z;
	}
	CVector3 operator/(CVector3<T> inVector)
	{
		CVector3 ret;
		ret.X = X / inVector.X;
		ret.Y = Y / inVector.Y;
		ret.Z = Z / inVector.Z;

		return ret;
	}
	void operator/=(CVector3<T> inVector)
	{
		inVector.X /= inVector.X;
		inVector.Y /= inVector.Y;
		inVector.Z /= inVector.Z;
	}

public:
	T X, Y, Z;
};

template<class T>
class CVector4
{
public:
	CVector4()
		:X(0.0f), Y(0.0f), Z(0.0f), W(0.0f)
	{
	}

	CVector4 operator+(CVector4<T> inVector)
	{
		CVector4 ret;
		ret.X = X + inVector.X;
		ret.Y = Y + inVector.Y;
		ret.Z = Z + inVector.Z;
		ret.W = W + inVector.W;

		return ret;
	}
	void operator+=(CVector4<T> inVector)
	{
		inVector.X += inVector.X;
		inVector.Y += inVector.Y;
		inVector.Z += inVector.Z;
		inVector.W += inVector.W;
	}
	CVector4 operator-(CVector4<T> inVector)
	{
		CVector4 ret;
		ret.X = X - inVector.X;
		ret.Y = Y - inVector.Y;
		ret.Z = Z - inVector.Z;
		ret.W = W - inVector.W;

		return ret;
	}
	void operator-=(CVector4<T> inVector)
	{
		inVector.X -= inVector.X;
		inVector.Y -= inVector.Y;
		inVector.Z -= inVector.Z;
		inVector.W -= inVector.W;
	}
	CVector4 operator*(CVector4<T> inVector)
	{
		CVector4 ret;
		ret.X = X * inVector.X;
		ret.Y = Y * inVector.Y;
		ret.Z = Z * inVector.Z;
		ret.W = W * inVector.W;

		return ret;
	}
	void operator*=(CVector4<T> inVector)
	{
		inVector.X *= inVector.X;
		inVector.Y *= inVector.Y;
		inVector.Z *= inVector.Z;
		inVector.W *= inVector.W;
	}
	CVector4 operator/(CVector4<T> inVector)
	{
		CVector4 ret;
		ret.X = X / inVector.X;
		ret.Y = Y / inVector.Y;
		ret.Z = Z / inVector.Z;
		ret.W = W / inVector.W;

		return ret;
	}
	void operator/=(CVector4<T> inVector)
	{
		inVector.X /= inVector.X;
		inVector.Y /= inVector.Y;
		inVector.Z /= inVector.Z;
		inVector.W /= inVector.W;
	}

public:
	T X, Y, Z, W;
};