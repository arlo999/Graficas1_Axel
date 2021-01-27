#include "Mesh.h"
#include <iostream>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::setMesh( AsimpleVertex* vertex, int numVertex)
{
    
    buffer = new AsimpleVertex[numVertex];
   
    memcpy(buffer, vertex, numVertex*sizeof(AsimpleVertex));
    
}

AsimpleVertex* Mesh::getMesh()
{

    return buffer ;
}

float* Mesh::getMatrixTransFormacion()
{
    return new float [16] {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

}

void Mesh::setIndice(unsigned short* indice, int numIndices)
{

	m_Indice = new unsigned short[numIndices];

	memcpy(m_Indice, indice, numIndices * sizeof(unsigned short));

}
