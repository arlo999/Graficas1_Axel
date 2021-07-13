#include "AAnimator.h"


AAnimator::~AAnimator()
{
}

AAnimator::AAnimator(AModel* current)
{

	m_CurrentAnimation = current;
	m_CurrentTime = 0.0f;
	m_DeltaTime = 0.0f;
	m_Transforms.reserve(100);
	for (int i = 0; i < 100; i++)
		m_Transforms.push_back(glm::mat4(0.0f));
}

void AAnimator::Init(AModel* current)
{
	m_CurrentAnimation = current;
	m_CurrentTime = 0.0f;
	m_DeltaTime=0.0f;
	m_Transforms.reserve(100);
	for (int i = 0; i < 100; i++)
		m_Transforms.push_back(glm::mat4(0.0f));
}

void AAnimator::UpdateAnimation(float dt)
{
	m_DeltaTime = dt;
	if (m_CurrentAnimation)
	{
		float TicksPerSecond = m_CurrentAnimation->GetTicksPerSecond() != 0 ?
			m_CurrentAnimation->GetTicksPerSecond() : 25.0f;
		float TimeInTicks = dt * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, m_CurrentAnimation->GetDuration());
		//m_CurrentTime=AnimationTime;
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());

		CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		
	}
}

void AAnimator::PlayAnimation(AModel* pAnimation)
{
	m_CurrentAnimation = pAnimation;
	m_CurrentTime = 0.0f;
}


void AAnimator::CalculateBoneTransform(const AssimpData* node, glm::mat4 parentTransform)
{

	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	ABone* Bone = m_CurrentAnimation->FindBone(nodeName);
	if (Bone)
	{
	
	
		Bone->Update(m_CurrentTime);
		nodeTransform = Bone->GetLocalTransform();
	
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_Transforms[index] = globalTransformation * offset ;
	
		
	}

	for (int i = 0; i < node->childrenCount; i++){
		CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}






