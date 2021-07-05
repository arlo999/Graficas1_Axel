#pragma once
#if defined(OGL)
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	glm::mat4 m_globalInversTransform;
	int boneMatricesLocation=0;
	unsigned int bonecount=0;

	glm::mat4 identidad=glm::mat4(1.0);

	//currentPose is held in this vector and uploaded to gpu as a matrix array uniform
	std::vector<glm::mat4> currentPose = {};
	
	
	inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 from)
	{
		glm::mat4 to;


		to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
		to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
		to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
		to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

		return to;
	}
	//helper tiene que ir en una clase de utilidades
	inline AVector assimpToAvector(aiVector3D vec) {
		return AVector(vec.x, vec.y, vec.z);
	}
	inline glm::vec3 assimpToGlmVec3(aiVector3D vec) {
		return glm::vec3(vec.x, vec.y, vec.z);
	}
	//helper tiene que ir en una clase de utilidades
	inline glm::quat assimpToGlmQuat(aiQuaternion quat) {
		glm::quat q;
		q.x = quat.x;
		q.y = quat.y;
		q.z = quat.z;
		q.w = quat.w;

		return q;
	}

	//tiene que ir en utilidades
	std::pair<unsigned int, float> getTimeFraction(std::vector<float>& times, float& dt) {
		int segment = 0;
		while (dt > times[segment]) {
		
			segment++;
		}
		float start = times[segment - 1];
		float end = times[segment];
		float frac = (dt - start) / (end - start);
		return { segment, frac };
	}
#endif
	bool rgb, bgr, wire, point, triangles;

#if defined(DX11)
	ID3D11ShaderResourceView* g_pTextureRV;
	ID3D11ShaderResourceView* g_NormalMap;
	ID3D11ShaderResourceView* g_Specular;
	ID3D11ShaderResourceView* g_Ao;
	#endif
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);
	
public:
	Animation animation;
	vector<Texture> textures_loaded;	
	vector<AMaterial> material; 
	vector<Mesh>    meshes;
	string directory;
	bool gammaCorrection = true;
	Bone skeleton;
	float deltatime=2;
	
	AModel(string const& path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);
	}

	AModel(){

	}

	void getPose(Animation& animation, Bone& skeletion, float dt, std::vector<glm::mat4>& output, glm::mat4& parentTransform, glm::mat4& globalInverseTransform);
	void Draw(AShader& shader);
	void Render(AShader& shader);
	void Rendersaq(AShader& shader);
	~AModel(){
			
	};
	void loadModel(string const& path);
	//load skeleton
	void loadModelSkeleton(string const& path);

private:
	HWND g_hwnd;
	void processNode(aiNode* node, const aiScene* scene);
	void processNodeSkeleton(aiNode* node, const aiScene* scene);

	//process skeleton mesh only
	Mesh processMeshSkeleton(aiMesh* mesh,const aiScene* scene);
	//process mesh unskeleton
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	//readskeleton for hirechy
	bool  readSkeleton(Bone& boneOutput, aiNode* node, std::map<std::string, std::pair<int, glm::mat4>>& boneInfoTable);
	void loadAnimation(const aiScene* scene,Animation& animation);
	//load the materials 
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

