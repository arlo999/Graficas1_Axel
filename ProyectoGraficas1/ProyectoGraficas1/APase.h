#pragma once

#include "AModel.h"
#include <iostream>
#include <vector>
#include "GraphicsModule.h"
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
	~APase();

	HRESULT Init();
	void Render(std::vector<AModel*>& _ModelList);
	void ApplyGbuffer();
	void ApplySimpleLight();

	HRESULT CompileShaderFromFile(const char* szFileName, const D3D10_SHADER_MACRO* _Macros, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
private:
	HWND g_hwnd;
	ID3D11VertexShader* g_pVertexShader = NULL;
	ID3D11PixelShader* g_pPixelShader = NULL;
	ID3D11InputLayout* g_pVertexLayout = NULL;
	std::vector<AModel*>m_ModelList;

};



