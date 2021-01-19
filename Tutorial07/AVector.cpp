#include "AVector.h"

AVector::AVector()
{
	ZeroVector();
}

AVector::AVector(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;

}

AVector& AVector::operator=(const AVector& other)
{
	// TODO: Insertar una instrucción "return" aquí
	X = other.X;
	Y = other.Y;
	Z = other.Z;

	return *this;
}

AVector AVector::operator+(const AVector& other)
{
	AVector res;
	res.setX(this->X + other.X);
	res.setY(this->Y + other.Y);
	res.setZ(this->Z + other.Z);
	
	return res;
}

AVector& AVector::operator+=(const AVector& other)
{
	// TODO: Insertar una instrucción "return" aquí
	X += other.X;
	Y += other.Y;
	Z += other.Z;

	return *this;
}

AVector AVector::operator-(const AVector& other)
{
	AVector res;
	res.setX(this->X - other.X);
	res.setY(this->Y - other.Y);
	res.setZ(this->Z - other.Z);
	return res;
}

AVector& AVector::operator-=(const AVector& other)
{
	// TODO: Insertar una instrucción "return" aquí
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;

	return *this;
}

AVector AVector::operator*(float delta)
{
	AVector res;
	res.setX(this->X * delta);
	res.setY(this->Y* delta);
	res.setZ(this->Z * delta);
	return res;
}

AVector& AVector::operator*=(float delta)
{
	// TODO: Insertar una instrucción "return" aquí
	X *= delta;
	Y *= delta;
	Z *= delta;

	return *this;
}

AVector AVector::ProductoCruz(const AVector& other)
{
	AVector res;
	res.setX(this->Y * other.Z - this->Z * other.Y);
	res.setY(this->Z * other.X - this->X * other.Z);
	res.setZ(this->X * other.Y - this->Y * other.X);
	return res;
}

float AVector::ProductoPunto(const AVector& other)
{
	return ((this->X * other.X) + (this->Y * other.Y) + (this->Z * other.Z));
}

void AVector::normalize()
{
	float mag = magnitude();
	X /= mag;
	Y /= mag;
	Z /= mag;
}

float AVector::magnitude()
{
	float len = (X * X) + (Y * Y) + (Z * Z);
	float invSqRoot = quickReverseSqrt2(len);
	float sqRoot = 1 / invSqRoot;

	return sqRoot;
	//return sqrt(len);

}

float AVector::quickReverseSqrt2(float number)
{
	union {
		float f;
		long i;
	} conv;

	float x2;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	conv.f = number;
	conv.i = 0x5f3759df - (conv.i >> 1);	// what the fuck? 
	conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
	return conv.f;
}
