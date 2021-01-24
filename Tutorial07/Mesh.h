#pragma once

struct simpleVertex {

	float Vertex[3];
	float TexVex[2];

};
class Mesh
{
	Mesh();
	~Mesh();
	void setMesh(const simpleVertex&, int);
	simpleVertex* getMesh();
	void MatrixTransFormacion();
};

