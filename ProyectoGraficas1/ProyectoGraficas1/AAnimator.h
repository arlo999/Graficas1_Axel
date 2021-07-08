#pragma once
#include <map>
#include <vector>
#include <string.h>
#include <iostream>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "AAnimation.h"



class AAnimator
{
public:

	
	AAnimator() {

	};
	AAnimator(AAnimation* current);
	

	~AAnimator();


	void Init(AAnimation* current);
	void UpdateAnimation(float dt);
	void PlayAnimation(AAnimation* pAnimation);
	
	void CalculateBoneTransform(const AssimpData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetPoseTransforms()
	{
		return m_Transforms;
	}
	//std::vector<float[16]>m_Transformsf; 
private:
	std::vector<glm::mat4> m_Transforms;
	AAnimation* m_CurrentAnimation;
	AAnimation prueba;
	float m_CurrentTime;
	float m_DeltaTime;

};

