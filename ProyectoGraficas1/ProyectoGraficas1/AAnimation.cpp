#include "AAnimation.h"
#include "AModel.h"
AAnimation::AAnimation()
{

}

AAnimation::~AAnimation()
{

}

bool AAnimation::Init(const std::string& animationPath, AModel* model)
{
	Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate| aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	 scene = importer.ReadFile(animationPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return false;
	}
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	SetupBones(animation, *model);
	ReadHeirarchyData(m_RootNode, scene->mRootNode);
	//ReadMissingBones(animation, *model);
	 
}

ABone* AAnimation::FindBone(const std::string& name)
{

	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
		[&](const ABone& Bone)
		{
			return Bone.GetBoneName() == name;
		}
	);
	if (iter == m_Bones.end()) return nullptr;
	else return &(*iter);
}



void AAnimation::SetupBones(const aiAnimation* animation, AModel& model)
{
	int size = animation->mNumChannels;

	auto& boneInfoMap = model.GetOffsetMatMap();
	int& boneCount = model.GetBoneCount();

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_Bones.push_back(ABone(channel->mNodeName.data,
			boneInfoMap[channel->mNodeName.data].id, channel));
	}

	m_BoneInfoMap = boneInfoMap;
}


void AAnimation::ReadHeirarchyData(AssimpData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = AHelper::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}


