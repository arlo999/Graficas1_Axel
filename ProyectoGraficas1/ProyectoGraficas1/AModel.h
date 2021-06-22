#pragma once
#if defined(OGL)
#include <glad/glad.h> 
#endif

#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AVector.h"
#include "AShader.h"
#include "Mesh.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "AMaterial.h"
#if defined(DX11)
#include <FreeImage/Dist/x64/FreeImage.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Test.h"
#endif

using namespace std;
struct ATransform {

	float scale[3];
	float rotation[3];
	float traslation[3];

};
class AModel
{
public:
	ATransform transform;
	#if defined(OGL)
	glm::mat4 model;
	#endif
	bool rgb, bgr, wire, point, triangles;

	ID3D11ShaderResourceView* g_pTextureRV;
	ID3D11ShaderResourceView* g_NormalMap;
	ID3D11ShaderResourceView* g_Specular;
	ID3D11ShaderResourceView* g_Ao;

	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);
	
	

	vector<Texture> textures_loaded;	
	vector<AMaterial> material; 
	vector<Mesh>    meshes;
	string directory;
	bool gammaCorrection = true;

	
	AModel(string const& path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);
	}
	AModel(){

	}


	void Draw(AShader& shader);
	void Render(AShader& shader);
	void Rendersaq(AShader& shader);
	~AModel(){
			
	};
	void loadModel(string const& path);

private:
	HWND g_hwnd;
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);


	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

