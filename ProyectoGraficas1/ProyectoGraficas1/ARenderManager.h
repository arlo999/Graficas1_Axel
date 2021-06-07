#pragma once
#include <iostream>
#include <vector>
#include "AModel.h"
#include "AEfecto.h"

namespace RManager {
	
	enum TypeTecnicas
	{
		VERTEX = 0,
		VERTEX_AND_BLINN,
		VERTEX_AND_PHONG,
		PIXEL,
		PIXEL_AND_BLINN,
		PIXEL_AND_PHONG,
		NORMALMAP,
		NORMALMAP_AND_BLINN,
		NORMALMAP_AND_PHONG,
		NORMAL_SPECULAR,
		NORMAL_SPECULAR_AND_BLINN,
		NORMAL_SPECULAR_AND_PHONG,

	};


class ARenderManager
{
public:
	//-------------------------Constructor ----------------------//	
	ARenderManager();

	//----------------------Destructor---------------------//
	~ARenderManager();


	/*------------------------------------------------------------/
	* variables
	---------------------------------------------------------------*/


	//PUT ALL MACROS 

	std::vector<D3D10_SHADER_MACRO> m_macros;

	
	std::vector<AEfecto*>m_EfectoList;
	int m_typeTecnicaRender=0;
private:
	
	AModel m_ScreenAlignedQuad;
	


	/*------------------------------------------------------------/
	* functions
	---------------------------------------------------------------*/
public:
	bool Init();
	void ApplyEfects();
	void Render(std::vector<AModel*>& _ModelList);

	void Forward(std::vector<AModel*>& _ModelList);
	void Deferred();
	//inits efects
	bool ForwardInit();

};

extern ARenderManager&  SingletonRM();

}