#pragma once
#if defined(OGL)


#endif
#include "ABone.h"
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
#include "ABoneInfo.h"
#include<glm/glm.hpp>
#include<assimp/Quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>
#include<glm/gtc/quaternion.hpp>
#if defined(DX11)
#include <FreeImage/Dist/x64/FreeImage.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Test.h"
#endif


class AHelper;


#define MAX_BONE_WEIGHTS 4
using namespace std;


struct ATransform {

	float scale[3];
	float rotation[3];
	float traslation[3];

};

struct AssimpData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpData> children;

};

class AModel
{
public:
	ATransform transform;
	#if defined(OGL)
	glm::mat4 model;
	
	
#endif
	bool rgb, bgr, wire, point, triangles;

#if defined(DX11)
	ID3D11ShaderResourceView* g_pTextureRV;
	ID3D11ShaderResourceView* g_NormalMap;
	ID3D11ShaderResourceView* g_Specular;
	ID3D11ShaderResourceView* g_Ao;

	GraphicsModule::CBChangesEveryFrame model;
	#endif
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);
	
public:

	vector<Texture> textures_loaded;	
	vector<Mesh>    meshes;
	string directory;
	bool gammaCorrection = true;
	
	
	AModel(string const& path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);
	}

	AModel();

	void Draw(AShader& shader,  float deltatimen);
	void Drawskelton(AShader& shader,  float deltatimen);
	void Render(AShader& shader);
	void Rendersaq(AShader& shader);
	~AModel(){
			
	};
	void loadModel(string const& path);
	
	//load skeleton
	

private:
	HWND g_hwnd;
	void processNode(aiNode* node, const aiScene* scene);
	
	
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	//load the materials 
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

public:
	auto& GetOffsetMatMap() { return m_OffsetMatMap; }
	int& GetBoneCount() { return m_BoneCount; }



private:
	std::map<string, BoneInfo> m_OffsetMatMap;
	int m_BoneCount = 0;
	Mesh skeleton;
	vector<VertexSkeleton> vertexskeleton;
	void SetVertexBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

public:
	ABone* FindBone(const std::string& name);
	inline  float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline  float GetDuration() { return m_Duration; }
	inline  const AssimpData& GetRootNode() { return m_RootNode; }

	const std::map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}
	void SetupBones(const aiAnimation* animation);
	void ReadHeirarchyData(AssimpData& dest, const aiNode* src);
	void SetupIndicesSkeleton(const AssimpData* node, const AssimpData* parent);
private:
	//animation
	aiMatrix4x4 globalTransformation;
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<ABone> m_Bones;
	AssimpData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;


public:
//sleton 
	bool Tpose=false;
	vector<unsigned int> indicesSkeleton;

	void UpdateAnimation(float dt);
	void PlayAnimation();
	void UpdateAnimationSkeleton(float dt);
	void CalculateBoneTransform(const AssimpData* node, glm::mat4 parentTransform);
	void CalculateBoneTransformSkeleton(const AssimpData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetPoseTransforms()
	{
		return m_Transforms;
	}
	std::vector<glm::mat4> GetPoseTransformsSkeleton()
	{
		return m_TransformsSkeleton;
	}

private:
	std::vector<glm::mat4> m_Transforms;
	std::vector<glm::mat4> m_TransformsSkeleton;
	

	float m_CurrentTime;
	float m_DeltaTime;

};

