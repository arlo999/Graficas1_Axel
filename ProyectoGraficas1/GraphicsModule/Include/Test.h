#pragma once
#include <windows.h>
#include "ABuffer.h"
#include "ADevice.h"
#include "ADeviceContext.h"
#include "ASwapChain.h"
#include "ATexture2D.h"
#include "ACamera.h"

#if defined(OGL)

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif
#if defined(DX11)

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif



#include <vector>
#include <iostream>

namespace GraphicsModule
{
	
	struct TooneMap
	{
#if defined(DX11)
		XMFLOAT3 n;
		FLOAT exposur;
	#endif



	};
	struct Sao
	{
#if defined(DX11)
		FLOAT  sampleRadius;
		FLOAT  scale;
		FLOAT  bias;
		FLOAT  intensity;
		XMFLOAT4 sampleIterations;
	#endif	
	};

	struct DirLigth {

#if defined(DX11)
		XMFLOAT4 dir;
		XMFLOAT4 lightDirColor;
#endif

	};

	struct PointLight {
#if defined(DX11)

		XMFLOAT4 pointLightColor;
		XMFLOAT3 pointLightPos;
		FLOAT  pointLightAtt;
#endif
	};
	struct spotLight {
#if defined(DX11)

		XMFLOAT4 spotLightColor;
		XMFLOAT4 spotLightPos;
		XMFLOAT4 spotLightDir;
		FLOAT  SpotlightAtt;
		FLOAT  spotLightInner;
		FLOAT  spotLightOutner;
		FLOAT  n = 0;
#endif
	};

	struct Shinies
	{
#if defined(DX11)
		XMFLOAT3 n;
		FLOAT shininess;
#endif
	};

	struct Diffuse
	{
#if defined(DX11)
		XMFLOAT3 n;
		FLOAT kDiffuse;
#endif
	};

	struct Specular
	{
#if defined(DX11)
		XMFLOAT3 n;
		FLOAT kSpecular;
#endif
	};

	struct Ambient {
#if defined(DX11)
		XMFLOAT4 ambientColor;
		XMFLOAT3 n;
		FLOAT kAmbient;
#endif

	};



	struct CBNeverChanges{
#if defined(DX11)
		XMMATRIX mView;
#endif
	};

	struct CBChangeOnResize
	{
#if defined(DX11)
		XMMATRIX mProjection;
#endif
	};

	struct CBChangesEveryFrame
	{
#if defined(DX11)
		XMMATRIX mWorld;
		XMFLOAT4 vMeshColor;
#endif
	};

	class Test
	{
	public:

#if defined(DX11)
		D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
		
		ADevice			g_pd3dDevice;
		ADeviceContext	g_pImmediateContext;
		ASwapChain		g_pSwapChain;
		ATexture2D		g_pDepthStencil;
		
/*
todo esto necesita abstra
*/
		ID3D11DepthStencilView* g_pDepthStencilView = NULL;
		ID3D11ShaderResourceView* g_pDepthStencilSRV = NULL;
		UINT width;
		UINT height;
	
		///////////////////////////////////////////
	//----------------------constanst buffer------------------//
		ABuffer m_pCBNeverChanges;
		ABuffer m_pCBChangeOnResize ;
		ABuffer m_pCBChangesEveryFrame;
		CBNeverChanges cbNeverChanges;//--------------------view matrix
		CBChangesEveryFrame cb;//--------------------------world matrix

		//DirLight
		ABuffer  m_LigthBuffer;
		DirLigth m_LigthBufferStruct;
		//point Light
		ABuffer m_PointLightBuffer;
		PointLight m_PointLightBufferStruct;
		//SpotLight
		ABuffer m_SpotLightBuffer;
		spotLight m_SpotLightBufferStruct;
		//Ambient 
		ABuffer m_AmbientBuffer;
		Ambient m_AmbientBufferStruct;
		//Specular
		ABuffer m_SpecularBuffer;
		Specular m_SpecularBufferStruct;
		//Shinies
		ABuffer m_ShiniesBuffer;
		Shinies m_ShiniesBufferStruct;
		//Diffuse
		ABuffer m_DiffuseBuffer;
		Diffuse m_DiffuseBufferStruct;
		//SAO
		ABuffer m_SaoBuffer;
		Sao m_SaoBufferStruct;
	//toone map
		ABuffer m_TooneMaBuffer;
		TooneMap m_TooneMaBufferStruct;
		//----------------------------------render targets----------------------//
		ID3D11RenderTargetView* g_pRenderTargetView = NULL;
	
	//-----------------------samplers-------------------------------------//

		ID3D11SamplerState* g_normalMapSampler = NULL;
		ID3D11SamplerState* g_SpecularSampler = NULL;
		ID3D11SamplerState* g_DiffuseSampler = NULL;

		//-------------------------------------------- cambiar a mis variables ----------///
		XMMATRIX                            g_World;
		XMMATRIX                            g_View;
		XMMATRIX                            g_Projection;
		XMFLOAT4                            g_vMeshColor;
		//-----------------camarera-----------------------//
		
		//-------------------------------------- viewport ---------------------//
		D3D11_VIEWPORT vp;
		//-------------------------------el gran general----------------------------//
		HWND m_hwnd;
		
#endif
	public:
	//camera
		ACamera				*camera;
		
		
		
#if defined(DX11)

		HRESULT ReloadBuffer(unsigned int width, unsigned int height);
		
	//	HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
#endif
		HRESULT InitDevice(HWND _hwnd);
public:
		void Render();
		void Update();
		void CleanupDevice();
	

	};

	extern  Test& GetTestObj(HWND _hwnd);
}