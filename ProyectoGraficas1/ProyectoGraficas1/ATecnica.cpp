#include "ATecnica.h"
#include "ARenderManager.h"
ATecnica::ATecnica()
{
	
}

ATecnica::~ATecnica()
{
	
}

void ATecnica::Init()
{
	


	auto& RM = RManager::SingletonRM();
	D3D10_SHADER_MACRO  vertex_Light;
	vertex_Light.Name= "VERTEX_LIGHT";
	vertex_Light.Definition="TRUE" ;
	RM.m_macros.push_back(vertex_Light);

	


	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition=NULL;
	Terminate.Name=NULL;

	RM.m_macros.push_back(Terminate);


	
	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::Render()
{
	for (int i = 0; i < m_PaseList.size(); i++)
	{
		m_PaseList[i]->Render();
	}
	

}

void ATecnica::InitVertex()
{

	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "VERTEX_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);


	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);
	
	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
	
}

void ATecnica::InitVertex_Phong()
{


	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "VERTEX_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);

	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "PHONG";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitVertex_BlinnPhong()
{

	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "VERTEX_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);

	

	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "BLINN_PHONG";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixel()
{

	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);


	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixel_Phong()
{
	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);



	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "PHONG";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixel_BlinnPhong()
{


	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);



	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "BLINN_PHONG";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixelMap()
{
	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);


	D3D10_SHADER_MACRO  pixelmap_Light;
	pixelmap_Light.Name = "NORMAL_MAP_PIXEL_LIGHT";
	pixelmap_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelmap_Light);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixelMap_Phong()
{

	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);


	D3D10_SHADER_MACRO  pixelmap_Light;
	pixelmap_Light.Name = "NORMAL_MAP_PIXEL_LIGHT";
	pixelmap_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelmap_Light);

	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "PHONG";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixelMap_BlinnPhong()
{

	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);


	D3D10_SHADER_MACRO  pixelmap_Light;
	pixelmap_Light.Name = "NORMAL_MAP_PIXEL_LIGHT";
	pixelmap_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelmap_Light);

	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "BLINN_PHONG";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixelMapSpec()
{
	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);


	D3D10_SHADER_MACRO  pixelmap_Light;
	pixelmap_Light.Name = "NORMAL_MAP_PIXEL_LIGHT";
	pixelmap_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelmap_Light);

	D3D10_SHADER_MACRO  pixelspec_Light;
	pixelspec_Light.Name = "SPECULAR_MAP_PIXEL_LIGHT";
	pixelspec_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelspec_Light);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixelMapSpec_Phong()
{

	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);


	D3D10_SHADER_MACRO  pixelmap_Light;
	pixelmap_Light.Name = "NORMAL_MAP_PIXEL_LIGHT";
	pixelmap_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelmap_Light);

	D3D10_SHADER_MACRO  pixelspec_Light;
	pixelspec_Light.Name = "SPECULAR_MAP_PIXEL_LIGHT";
	pixelspec_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelspec_Light);


	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "PHONG";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);


	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitPixelMapSpec_BlinnPhong()
{

	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  pixel_Light;
	pixel_Light.Name = "PIXEL_LIGHT";
	pixel_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixel_Light);


	D3D10_SHADER_MACRO  pixelmap_Light;
	pixelmap_Light.Name = "NORMAL_MAP_PIXEL_LIGHT";
	pixelmap_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelmap_Light);

	D3D10_SHADER_MACRO  pixelspec_Light;
	pixelspec_Light.Name = "SPECULAR_MAP_PIXEL_LIGHT";
	pixelspec_Light.Definition = "TRUE";
	RM.m_macros.push_back(pixelspec_Light);


	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "BLINN_PHONG";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);


	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);

	APase* pase = new APase;
	pase->Init();
	m_PaseList.push_back(pase);
}

void ATecnica::InitGbuffer_NormSpec()
{

	auto& RM = RManager::SingletonRM();
	RM.m_macros.clear();
	D3D10_SHADER_MACRO  normalMap;
	normalMap.Name = "NORMAL_MAP";
	normalMap.Definition = "TRUE";
	RM.m_macros.push_back(normalMap);


	D3D10_SHADER_MACRO  specMap;
	specMap.Name = "SPECULAR_MAP";
	specMap.Definition = "TRUE";
	RM.m_macros.push_back(specMap);

	D3D10_SHADER_MACRO Terminate;
	Terminate.Definition = NULL;
	Terminate.Name = NULL;

	RM.m_macros.push_back(Terminate);


	APase* pase = new APase;
	pase->InitGBuffer();
	m_PaseList.push_back(pase);



	//post procesos
	
	RM.m_macros.clear();
	

	D3D10_SHADER_MACRO  Phong;
	Phong.Name = "BLINN";
	Phong.Definition = "TRUE";
	RM.m_macros.push_back(Phong);
	
	Terminate.Definition = NULL;
	Terminate.Name = NULL;
	RM.m_macros.push_back(Terminate);
	
	
	/*
	APase* paseSkyBox = new APase(true);
	paseSkyBox->InitSkybox();
	m_PaseList.push_back(paseSkyBox);
	*/
	


/*
APase* paseLSao = new APase(true);
paseLSao->InitSao();
m_PaseList.push_back(paseLSao);
*/

	
	
	APase* paseLight = new APase(true);
	paseLight->InitLight();
	m_PaseList.push_back(paseLight);
	



	
	
	
	APase* paseToonMap = new APase(true);
	paseToonMap->InitTooneMap();
	m_PaseList.push_back(paseToonMap);
	
	
	
	

	APase* paseCopy = new APase(true);
	paseCopy->InitCopy();
	m_PaseList.push_back(paseCopy);
	
	

}

