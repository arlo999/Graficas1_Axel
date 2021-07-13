#pragma once

#include "AModel.h"
#include <iostream>
#include <vector>
#include "GraphicsModule.h"
#include "AShader.h"
#if defined(DX11)

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#endif



class APase
{
public:
	APase();
	APase(bool _PostProceso);
	APase(int _typePase);
	~APase();

	HRESULT Init();
	void Render();
	HRESULT InitGBuffer();
	HRESULT InitCopy();
	HRESULT InitLight();
	HRESULT InitSao();
	HRESULT InitTooneMap();
	HRESULT InitSkybox();
	HRESULT InitSkeleto();


	bool forward = false;

	HRESULT CompileShaderFromFile(const char* szFileName, const D3D10_SHADER_MACRO* _Macros, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
private:
	HWND g_hwnd;
	ID3D11VertexShader* g_pVertexShader = NULL;
	ID3D11PixelShader* g_pPixelShader = NULL;
	ID3D11InputLayout* g_pVertexLayout = NULL;
	ID3D11RasterizerState* m_Rasterizador = NULL;
	std::vector<ID3D11RenderTargetView*>m_ListRenderTV;

	std::vector<AModel*>m_ModelList;
	int m_TypePase;
	
};



