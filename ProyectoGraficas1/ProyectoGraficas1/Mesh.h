#pragma once
#include "AVector.h"
#if defined(OGL)
#include <glad/glad.h>
#endif
#include "AShader.h"
#include "ABuffer.h"
#include <string>
#include <vector>
#include <map>

#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#endif
#include "Test.h"
#define MAX_BONE_INFLUENCE 4
using namespace std;
struct SimpleVertex
{
#if defined(DX11)
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Normal;
#endif
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
	unsigned int m_BoneIDs[4];
	float m_Weights[4];
	
};
struct VertexSkeleton {
	float Position[4];
	unsigned int m_BoneIDs[4];
	float m_Weights[4];
};





class Mesh
{
public:
	Mesh();
	~Mesh();

	float* getMatrixTransFormacion();
	

	
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
	vector<VertexSkeleton>       verticesSkeleton;
	vector<unsigned int> indices;
	vector<Texture>      textures;

	vector<ID3D11ShaderResourceView* >textures_vec;
	unsigned int VAO;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Init(vector<VertexSkeleton> vertices, vector<unsigned int> indices);

	Mesh(vector<VertexSkeleton> vertices, vector<unsigned int> indices, vector<Texture> textures);
	// render the mesh
	void Draw(AShader& shader, bool points);
	void Render(AShader& shader);
	void RenderSkeleton();
//opengl
private:
	// render data 
	unsigned int VBO, EBO;
	void setupMesh();
	void setupMeshSkeleton();
	HWND g_hwnd;
private:

	AVector m_position;
	unsigned short* m_Indice;


};

