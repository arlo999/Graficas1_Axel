#pragma once
#include <map>
#include <vector>
#include <string.h>
#include <iostream>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>




class AAnimator
{
public:

	
	AAnimator() {

	};
	AAnimator(AModel* current);
	

	~AAnimator();


	void Init(AModel* current);
	void UpdateAnimation(float dt);
	void PlayAnimation(AModel* pAnimation);
	
	void CalculateBoneTransform(const AssimpData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetPoseTransforms()
	{
		return m_Transforms;
	}
	//std::vector<float[16]>m_Transformsf; 
private:
	std::vector<glm::mat4> m_Transforms;
	AModel* m_CurrentAnimation;

	float m_CurrentTime;
	float m_DeltaTime;

};

