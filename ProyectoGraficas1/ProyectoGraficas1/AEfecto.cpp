#include "AEfecto.h"
#include "ARenderManager.h"
AEfecto::AEfecto()
{
	
}

AEfecto::~AEfecto()
{
}

void AEfecto::Init()
{

	InitDefered();

}

void AEfecto::Render()
{
	auto& RM = RManager::SingletonRM();
	if (RM.m_Forward) {

		m_TecnicaList[RM.m_typeTecnicaRender]->Render();
	}
	else {


		for (int i = 0; i < m_TecnicaListDefferd.size(); i++)
		{
			m_TecnicaListDefferd[i]->Render();
		}
	}
	
}

void AEfecto::InitForward()
{

	//vertex
	ATecnica* m_vertex = new ATecnica;
	m_vertex->Init();
	m_TecnicaList.push_back(m_vertex);

	ATecnica* m_vertexPHONG = new ATecnica;
	m_vertexPHONG->InitVertex_Phong();
	m_TecnicaList.push_back(m_vertexPHONG);

	ATecnica* m_vertexBlinn = new ATecnica;
	m_vertexBlinn->InitVertex_BlinnPhong();
	m_TecnicaList.push_back(m_vertexBlinn);
	//pixel
	ATecnica* m_pixel = new ATecnica;
	m_pixel->InitPixel();
	m_TecnicaList.push_back(m_pixel);

	ATecnica* m_pixelPhong = new ATecnica;
	m_pixelPhong->InitPixel_Phong();
	m_TecnicaList.push_back(m_pixelPhong);

	ATecnica* m_pixelBlinnPhong = new ATecnica;
	m_pixelBlinnPhong->InitPixel_BlinnPhong();
	m_TecnicaList.push_back(m_pixelBlinnPhong);

	//pixel map
	ATecnica* m_pixelmap = new ATecnica;
	m_pixelmap->InitPixelMap();
	m_TecnicaList.push_back(m_pixelmap);

	ATecnica* m_pixelmapPhong = new ATecnica;
	m_pixelmapPhong->InitPixelMap_Phong();
	m_TecnicaList.push_back(m_pixelmapPhong);

	ATecnica* m_pixelmapBlinnPhong = new ATecnica;
	m_pixelmapBlinnPhong->InitPixelMap_BlinnPhong();
	m_TecnicaList.push_back(m_pixelmapBlinnPhong);

	//specular


	ATecnica* m_pixelmapSpecular = new ATecnica;
	m_pixelmapSpecular->InitPixelMap();
	m_TecnicaList.push_back(m_pixelmapSpecular);

	ATecnica* m_pixelmapSpecularPhong = new ATecnica;
	m_pixelmapSpecularPhong->InitPixelMap_Phong();
	m_TecnicaList.push_back(m_pixelmapSpecularPhong);

	ATecnica* m_pixelmapSpecularBlinnPhong = new ATecnica;
	m_pixelmapSpecularBlinnPhong->InitPixelMap_BlinnPhong();
	m_TecnicaList.push_back(m_pixelmapSpecularBlinnPhong);
}

void AEfecto::InitDefered()
{
	
	ATecnica* m_Gbuffer = new ATecnica;
	m_Gbuffer->InitGbuffer_NormSpec();
	m_TecnicaListDefferd.push_back(m_Gbuffer);






}
