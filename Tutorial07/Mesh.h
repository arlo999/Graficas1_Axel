#pragma once
#include "AVector.h"
struct AsimpleVertex {

	float Vertex[4];
	float TexVex[3];

};
struct Afloat3 {

	float x;
	float y;
	float z;
};
struct Afloat2 {

	float x;
	float y;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void setMesh( AsimpleVertex*, int);
	AsimpleVertex* getMesh();
	float* getMatrixTransFormacion();

	//get set transformacion
	void setPosition(AVector Position);
	AVector getPosition() { return m_position; }

	void setIndice(unsigned short* indice, int);
	unsigned short* getIndice() { return m_Indice; }

private:
	AsimpleVertex* buffer;
	AVector m_position;
	unsigned short* m_Indice;
};

