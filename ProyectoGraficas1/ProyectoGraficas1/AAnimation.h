#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#if defined(OGL)  
#include <glm/glm.hpp>
#endif
#include "assimp/scene.h"
#include <functional>
#include "ABone.h"
#include "ABoneInfo.h"
class AModel;

struct AssimpData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpData> children;

};

class AAnimation
{
public:

	AAnimation();
	//destructor
	~AAnimation();

	void Init(const std::string& animationPath, AModel* model);
	ABone* FindBone(const std::string& name);
	inline  float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline  float GetDuration() { return m_Duration; }
	inline  const AssimpData& GetRootNode() { return m_RootNode; }
	
	const std::map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}
	void SetupBones(const aiAnimation* animation, AModel& model);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
	{
		for (int i = 0; i < pAnimation->mNumChannels; i++) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
				return pNodeAnim;
			}
		}

		return NULL;
	}
	
	void ReadHeirarchyData(AssimpData& dest, const aiNode* src);
	aiMatrix4x4 globalTransformation;
	glm::mat4 nodeTransform;
private:
	//const	aiAnimation* animation;
	const aiScene* scene;
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<ABone> m_Bones;
	AssimpData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
};

