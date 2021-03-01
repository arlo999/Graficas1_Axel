#pragma once
#include"AVector.h"

class AMatriz4
{
public:
	AMatriz4();
	AMatriz4(AVector X, AVector Y, AVector Z, AVector M) :x(X), y(Y), z(Z), m(M) {};
	AVector x, y, z, m;
private:
};

