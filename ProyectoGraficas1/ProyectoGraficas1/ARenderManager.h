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

	enum TypePase {

		GBUFFER=0,
		SKYBOX,
		LIGHT,
		AO,
		TONEMAP,
		COPY
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


	vector<std::string> faces
	{
		"SkyboxAsset/right.jpg",
			"SkyboxAsset/left.jpg",
			"SkyboxAsset/top.jpg",
			"SkyboxAsset/bottom.jpg",
			"SkyboxAsset/front.jpg",
			"SkyboxAsset/back.jpg"
	};
	//PUT ALL MACROS 

#if defined(DX11) 
	std::vector<D3D10_SHADER_MACRO> m_macros;
	std::vector<ID3D11RenderTargetView*>m_RTVList;
#endif	
	std::vector<AModel*>m_ModelList;
	std::vector<AEfecto*>m_EfectoList;
	int m_typeTecnicaRender=0;
	AModel m_ScreenAlignedQuad;
	AModel m_Skybox;

	//todo usar abstraccion
	//SHADER RESOURCE


#if defined(DX11) 
	ID3D11ShaderResourceView* m_PositionSRV = NULL;
	ID3D11ShaderResourceView* m_NormalSRV = NULL;
	ID3D11ShaderResourceView* m_SpecularSRV = NULL;
	ID3D11ShaderResourceView* m_AlbedoSRV = NULL;
	ID3D11ShaderResourceView* m_LightSRV = NULL;
	ID3D11ShaderResourceView* m_SSaoSRV = NULL;
	ID3D11ShaderResourceView* m_ToonMapSRV = NULL;
	ID3D11ShaderResourceView* m_SkyboxMapSRV=NULL;
	ID3D11ShaderResourceView* m_SkyboxNormalMapSRV=NULL;
	//RENDER TARGETS
	ID3D11RenderTargetView* m_PositionRT = NULL;
	ID3D11RenderTargetView* m_NormalRT = NULL;
	ID3D11RenderTargetView* m_SpecularRT = NULL;
	ID3D11RenderTargetView* m_AlbedoRT = NULL;
	ID3D11RenderTargetView* m_LightRT = NULL;
	ID3D11RenderTargetView* m_SsaoRT = NULL;
	ID3D11RenderTargetView* m_ToonRT = NULL;
	ID3D11RenderTargetView* m_SkyboxRT = NULL;
	//SAMPLERS
	ID3D11SamplerState* m_generico = NULL;
#endif
	//


	//pases deferred

	AShader m_shaderLight;
	AShader m_shaderGBuffer;
	AShader m_shaderCopy;
	AShader m_shaderToneMap;
	AShader m_shaderSAO;
	AShader m_shaderSkybox;
	//pases forward

	AShader m_shaderlightF;
	


	bool m_Forward=false;

	//ogl
	//gbuffer
	unsigned int gBuffer;
	unsigned int gAlbedo, gPosition, gNormal, gColorSpec, gAlbedoSpec;
	unsigned int rboDepth;
	//copy
	unsigned int CopyRT;
	//light
	unsigned int LightRT;
	unsigned int Lightsrv;
	//toneMap
	unsigned int TonemapRT;
	unsigned int toneMapsrv;
	//SSAO
	unsigned int SAORT;
	unsigned int SAOsrv;
	//skybox

	unsigned int Skyboxsrv;
	//forward 
	
	unsigned int LightForwardRT;
	unsigned int LightForwardsrv;
	unsigned int ToneMapForwardRT;
	unsigned int ToneMapForwardsrv;
	unsigned int CopyForwardRT;
	

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

	unsigned int loadCubemap(vector<std::string> faces);

};

extern ARenderManager&  SingletonRM();

}