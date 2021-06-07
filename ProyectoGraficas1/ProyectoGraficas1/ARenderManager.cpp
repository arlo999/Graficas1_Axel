#include "ARenderManager.h"
namespace RManager {
ARenderManager::ARenderManager()
{
	
}

ARenderManager::~ARenderManager()
{
}

bool ARenderManager::Init()
{
	std::cout<<"Init render manager"<<std::endl;
	
	ForwardInit();

	return true;
	
}

void ARenderManager::ApplyEfects()
{
	
}



void ARenderManager::Render(std::vector<AModel*>& _ModelList)
{
//usar modelList in singleton	
	
	Forward(_ModelList);


}

void ARenderManager::Forward(std::vector<AModel*>& _ModelList)
{
	
	

	for (int i = 0; i < m_EfectoList.size(); i++)
	{
		m_EfectoList[i]->Render(_ModelList);
	}

}

void ARenderManager::Deferred()
{
}

bool ARenderManager::ForwardInit()
{
	AEfecto* forward= new AEfecto;

	m_EfectoList.push_back(forward);

	return true;
}

extern ARenderManager& SingletonRM()
{
	static ARenderManager* m_SingletonRM;
	if (m_SingletonRM == nullptr)
	{
		m_SingletonRM = new ARenderManager();
		m_SingletonRM->Init();
	}
	return *m_SingletonRM;
}

}