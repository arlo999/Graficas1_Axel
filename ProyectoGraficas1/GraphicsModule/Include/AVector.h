#pragma once
class AVector
{
public:

	AVector();
	AVector(float,  float, float);
	AVector(float x, float y, float z, float m) :X(x), Y(y),Z(z),M(m) {};
	/// getters
	float getX() { return X; }
	float getY() { return Y; }
	float getZ() { return Z; }
	float getM() { return M; }
	/// setter
	void setX(float x) { X = x; }
	void setY(float y) { Y = y; }
	void setZ(float z) { Z = z; }
	void setM(float m) { M = m; }
	//set all zero vector
	void ZeroVector() {
		X = 0;
		Y = 0;
		Z = 0;
	}

	AVector& operator=(const AVector& other);

	AVector operator+(const AVector& other);
	AVector& operator+=(const AVector& other);

	AVector operator-(const AVector& other);
	AVector& operator-=(const AVector& other);

	AVector operator*(float delta);
	AVector& operator*=(float delta);

	AVector ProductoCruz(const AVector& other);
	float ProductoPunto(const AVector& other);
	void normalize();
	float magnitude();
	float quickReverseSqrt2(float number);

private:
	float X;
	float Y;
	float Z;
	float M;

};

