#pragma once
#include <vector>
#include "AModel.h"
#include "APase.h"

class ATecnica
{
public:
	ATecnica();
	~ATecnica();

	void Init();
	void Render();
	//vertex
	void InitVertex();
	void InitVertex_Phong();
	void InitVertex_BlinnPhong();
	//pixel
	void InitPixel();
	void InitPixel_Phong();
	void InitPixel_BlinnPhong();
	//pixelmap
	void InitPixelMap();
	void InitPixelMap_Phong();
	void InitPixelMap_BlinnPhong();
//specular
	void InitPixelMapSpec();
	void InitPixelMapSpec_Phong();
	void InitPixelMapSpec_BlinnPhong();

	//Gbuffer Tecnicas
	void InitGbuffer_NormSpec();

private:
	
	std::vector<APase*> m_PaseList;
	std::vector<APase*> m_PaseListDefferd;
	
};

