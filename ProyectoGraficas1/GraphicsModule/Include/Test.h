#pragma once

#include <windows.h>
#include "ABuffer.h"
#include "Mesh.h"
#include "ADevice.h"
#include "ADeviceContext.h"
#include "ASwapChain.h"
#include "ATexture2D.h"
#include "ATextura.h"
#if defined(OGL)

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif
#if defined(DX11)
#include "ACamera.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif


#include <vector>
#include <iostream>

namespace GraphicsModule
{
#if defined(DX11)
	struct DirLigth {
	
	XMFLOAT4 dir;
	};
#endif
	struct SimpleVertex
	{
#if defined(DX11)
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Normal;
	#endif
	};

	struct CBNeverChanges
	{
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
		

		ID3D11DepthStencilView* g_pDepthStencilView = NULL;
		ID3D11ShaderResourceView* g_pDepthStencilSRV = NULL;
		ID3D11VertexShader* g_pVertexShader = NULL;
		ID3D11PixelShader* g_pPixelShader = NULL;
		ID3D11InputLayout* g_pVertexLayout = NULL;
		
		
		ABuffer  m_pVertexBuffer;
		ABuffer m_pIndexBuffer;
		ABuffer m_pCBNeverChanges;
		ABuffer m_pCBChangeOnResize ;
		ABuffer m_pCBChangesEveryFrame;
		//DirLight
		ABuffer  m_LigthBuffer;
		DirLigth m_LigthBufferStruct;
		//shaders ResourceView

		ID3D11ShaderResourceView* g_pTextureRV=NULL;
		ID3D11ShaderResourceView* m_Shader2 = NULL;
		ID3D11ShaderResourceView* m_Shader3 = NULL;
		ID3D11ShaderResourceView* m_Shader4 = NULL;
		
		//render targets

		ID3D11RenderTargetView* g_pRenderTargetView = NULL;
		ID3D11RenderTargetView* m_Target2 = NULL;
		ID3D11RenderTargetView* m_Target3 = NULL;
		ID3D11RenderTargetView* m_Target4 = NULL;



		ID3D11SamplerState* g_pSamplerLinear = NULL;
		
		XMMATRIX                            g_World;
		XMMATRIX                            g_View;
		XMMATRIX                            g_Projection;
		XMFLOAT4                            g_vMeshColor;
		//camara axel
		ACamera				*camera;
		CBNeverChanges cbNeverChanges;
	
		
		
		//importans variables
		D3D11_VIEWPORT vp;
		HWND m_hwnd;
		
#endif
	public:
		//variables for load model
		Afloat3 m_pos;
		Afloat3 m_normal;
		Afloat2 m_vertex;
		unsigned int numVertex;
		std::vector <AsimpleVertexV2> arrSimpleVertex;
		std::vector<unsigned int> m_indices;
		
#if defined(DX11)
		HRESULT ReloadBuffer(unsigned int width, unsigned int height);
		
		HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
#endif

		HRESULT InitDevice(HWND _hwnd);
		HRESULT InitDeviceOGL(HWND _hwnd);
public:
		void Render();
		void Update();
		void CleanupDevice();
	

	};

	extern  Test& GetTestObj(HWND _hwnd);
}