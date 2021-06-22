#pragma once
#include "AVector.h"
#if defined(OGL)
#include <glad/glad.h>
#endif
#include "AShader.h"
#include "ABuffer.h"
#include <string>
#include <vector>
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#endif
#include "Test.h"

using namespace std;
struct SimpleVertex
{
#if defined(DX11)
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Normal;
#endif
};

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
	float Binormal[3];
	float Tangente[3];
	float TexCoords[2];
	
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
	ABuffer  m_pVertexBuffer;
	ABuffer m_pIndexBuffer;
	
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture>      textures;

	vector<ID3D11ShaderResourceView* >textures_vec;
	unsigned int VAO;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<ID3D11ShaderResourceView* >textures_vec);
	// render the mesh
	void Draw(AShader& shader, bool points);
	void Render(AShader& shader);
//opengl
private:
	// render data 
	unsigned int VBO, EBO;
	void setupMesh();
	HWND g_hwnd;
private:
	AsimpleVertex* buffer;
	AsimpleVertexV2* m_buffer;
	AVector m_position;
	unsigned short* m_Indice;


};

