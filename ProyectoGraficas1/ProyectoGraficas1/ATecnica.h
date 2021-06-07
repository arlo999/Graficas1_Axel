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
	void Render(std::vector<AModel*>& _ModelList);
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
private:
	
	std::vector<APase*> m_PaseList;
	
};

