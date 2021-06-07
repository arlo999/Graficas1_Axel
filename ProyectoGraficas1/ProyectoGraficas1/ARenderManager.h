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
	std::vector<AModel*>m_ModelList;
	std::vector<AEfecto*>m_EfectoList;
	std::vector<ID3D11RenderTargetView*>m_RTVList;
	int m_typeTecnicaRender=0;
	AModel m_ScreenAlignedQuad;
	AModel m_Skybox;


	//SHADER RESOURCE
	ID3D11ShaderResourceView* m_PositionSRV = NULL;
	ID3D11ShaderResourceView* m_NormalSRV = NULL;
	ID3D11ShaderResourceView* m_SpecularSRV = NULL;
	ID3D11ShaderResourceView* m_AlbedoSRV = NULL;
	ID3D11ShaderResourceView* m_SSaoSRV = NULL;
	ID3D11ShaderResourceView* m_FinalSRV = NULL;
	ID3D11ShaderResourceView* m_SkyboxMapSRV=NULL;
	//RENDER TARGETS
	ID3D11RenderTargetView* m_PositionRT = NULL;
	ID3D11RenderTargetView* m_NormalRT = NULL;
	ID3D11RenderTargetView* m_SpecularRT = NULL;
	ID3D11RenderTargetView* m_AlbedoRT = NULL;
	ID3D11RenderTargetView* m_SsaoRT = NULL;
	ID3D11RenderTargetView* m_FinalRT = NULL;
	ID3D11RenderTargetView* m_SkyboxRT = NULL;
	//SAMPLERS
	ID3D11SamplerState* m_generico = NULL;


private:
	

	HWND g_hwnd;
	/*------------------------------------------------------------/
	* functions
	---------------------------------------------------------------*/
public:
	bool Init();
	void LoadDependences();
	HRESULT ApplyEfects();
	void Render(std::vector<AModel*>& _ModelList);
	
	//inits efects

	bool ForwardInit();
	bool DefferedInit();


	/*------------------------------------------------------------/
	* EFECTOS
	---------------------------------------------------------------*/
	void Forward();
	void Deferred();



};

extern ARenderManager&  SingletonRM();

}