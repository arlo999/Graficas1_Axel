#pragma once
#include "AVector.h"
#include <glad/glad.h>
#include "AShader.h"
#include <string>
#include <vector>

using namespace std;

struct AsimpleVertex {

	float Vertex[3];
	float TexVex[2];
	float Normal[3];

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
	Afloat3 Pos;
	Afloat2 TextVex;
	Afloat3 Vertex;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};
struct Vertex {
	float Position[3];
	
	float Normal[3];
	
	float TexCoords[2];

	float Tanget[3];

	float Bitanget[3];
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


	//opengl
public:
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture>      textures;
	unsigned int VAO;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	// render the mesh
	void Draw(AShader& shader);

//opengl
private:
	// render data 
	unsigned int VBO, EBO;
	void setupMesh();

private:
	AsimpleVertex* buffer;
	AsimpleVertexV2* m_buffer;
	AVector m_position;
	unsigned short* m_Indice;
};

