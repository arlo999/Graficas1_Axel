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
struct AsimpleVertexV2 {
	Afloat3 Vertex;
	Afloat2 TextVex;
};


class Mesh
{
public:
	Mesh();
	~Mesh();
	//version con el struc con float
	void setMesh( AsimpleVertex*, int);
	AsimpleVertex* getMesh();
	float* getMatrixTransFormacion();
	
	//esta es la version 2
	//esta version utiliza un struc que llama a otro struc que contiene 3 floats
	void setMesh2(AsimpleVertexV2*, int);
	AsimpleVertexV2* getMesh2();
	
	
	//get set transformacion
	void setPosition(AVector Position);
	AVector getPosition() { return m_position; }

	void setIndice(unsigned short* indice, int);
	unsigned short* getIndice() { return m_Indice; }

private:
	AsimpleVertex* buffer;
	AsimpleVertexV2* m_buffer;
	AVector m_position;
	unsigned short* m_Indice;
};

