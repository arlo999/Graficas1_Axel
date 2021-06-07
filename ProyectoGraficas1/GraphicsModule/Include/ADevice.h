#pragma once
#include <d3d11.h>

class ADevice
{
public:

	ADevice()=default;
	~ADevice() = default;



	HRESULT A_CreateRenderTargetView(  
		/* [annotation] */
		__in  ID3D11Resource* pResource,
		/* [annotation] */
		__in_opt  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
		/* [annotation] */
		__out_opt  ID3D11RenderTargetView** ppRTView );

	HRESULT A_CreateTexture2D(
		/* [annotation] */
		__in  const D3D11_TEXTURE2D_DESC* pDesc,
		/* [annotation] */
		__in_xcount_opt(pDesc->MipLevels* pDesc->ArraySize)  const D3D11_SUBRESOURCE_DATA* pInitialData,
		/* [annotation] */
		__out_opt  ID3D11Texture2D** ppTexture2D);

	HRESULT A_CreateDepthStencilView(
		/* [annotation] */
		__in  ID3D11Resource* pResource,
		/* [annotation] */
		__in_opt  const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
		/* [annotation] */
		__out_opt  ID3D11DepthStencilView** ppDepthStencilView);

	HRESULT A_CreateVertexShader(
		/* [annotation] */
		__in  const void* pShaderBytecode,
		/* [annotation] */
		__in  SIZE_T BytecodeLength,
		/* [annotation] */
		__in_opt  ID3D11ClassLinkage* pClassLinkage,
		/* [annotation] */
		__out_opt  ID3D11VertexShader** ppVertexShader);
	
	HRESULT A_CreateInputLayout(
		/* [annotation] */
		__in_ecount(NumElements)  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
		/* [annotation] */
		__in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements,
		/* [annotation] */
		__in  const void* pShaderBytecodeWithInputSignature,
		/* [annotation] */
		__in  SIZE_T BytecodeLength,
		/* [annotation] */
		__out_opt  ID3D11InputLayout** ppInputLayout);

	HRESULT A_CreatePixelShader(
		__in  const void* pShaderBytecode,
		/* [annotation] */
		__in  SIZE_T BytecodeLength,
		/* [annotation] */
		__in_opt  ID3D11ClassLinkage* pClassLinkage,
		/* [annotation] */
		__out_opt  ID3D11PixelShader** ppPixelShader);
	
	HRESULT A_CreateBuffer(
		/* [annotation] */
		__in  const D3D11_BUFFER_DESC* pDesc,
		/* [annotation] */
		__in_opt  const D3D11_SUBRESOURCE_DATA* pInitialData,
		/* [annotation] */
		__out_opt  ID3D11Buffer** ppBuffer);
	
	HRESULT  A_CreateRasterizerState(
		/* [annotation] */
		_In_  const D3D11_RASTERIZER_DESC* pRasterizerDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11RasterizerState** ppRasterizerState);

	HRESULT A_CreateSamplerState(
		/* [annotation] */
		__in  const D3D11_SAMPLER_DESC* pSamplerDesc,
		/* [annotation] */
		__out_opt  ID3D11SamplerState** ppSamplerState);
	 HRESULT  A_CreateShaderResourceView(
		 /* [annotation] */
		 __in  ID3D11Resource* pResource,
	/* [annotation] */
		__in_opt  const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc,
		/* [annotation] */
		__out_opt  ID3D11ShaderResourceView** ppSRView);

	HRESULT A_Release();
	ID3D11Device* m_device=NULL;
private:
};

