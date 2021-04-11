#pragma once

#include "ADeviceContext.h"
#include "ASwapChain.h"
#include "ADevice.h"


class AMangerAPI
{
public:
//------------------------------- Device ------------------------------------//
	HRESULT A_CreateRenderTargetView(
		/* [annotation] */
		__in  ID3D11Resource* pResource,
		/* [annotation] */
		__in_opt  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
		/* [annotation] */
		__out_opt  ID3D11RenderTargetView** ppRTView);

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

	HRESULT A_CreateSamplerState(
		/* [annotation] */
		__in  const D3D11_SAMPLER_DESC* pSamplerDesc,
		/* [annotation] */
		__out_opt  ID3D11SamplerState** ppSamplerState);

	HRESULT A_ReleaseDevice();
	//------------------------------- Device ------------------------------------//



	//------------------------------- DeviceContext ------------------------------------//
	
	void A_ClearState();

	HRESULT A_ReleaseDeviceContext();

	void A_OMSetRenderTargets(
		/* [annotation] */
		__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
		/* [annotation] */
		__in_ecount_opt(NumViews)  ID3D11RenderTargetView* const* ppRenderTargetViews,
		/* [annotation] */
		__in_opt  ID3D11DepthStencilView* pDepthStencilView);

	void A_RSSetViewports(
		/* [annotation] */
		__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
		/* [annotation] */
		__in_ecount_opt(NumViewports)  const D3D11_VIEWPORT* pViewports);

	void A_IASetInputLayout(

		/* [annotation] */
		__in_opt  ID3D11InputLayout* pInputLayout);

	void A_IASetVertexBuffers(
		/* [annotation] */
		__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		__in_ecount(NumBuffers)  ID3D11Buffer* const* ppVertexBuffers,
		/* [annotation] */
		__in_ecount(NumBuffers)  const UINT* pStrides,
		/* [annotation] */
		__in_ecount(NumBuffers)  const UINT* pOffsets);

	void A_IASetIndexBuffer(
		/* [annotation] */
		__in_opt  ID3D11Buffer* pIndexBuffer,
		/* [annotation] */
		__in  DXGI_FORMAT Format,
		/* [annotation] */
		__in  UINT Offset);

	void A_IASetPrimitiveTopology(
		/* [annotation] */
		__in  D3D11_PRIMITIVE_TOPOLOGY Topology);

	void A_UpdateSubresource(
		/* [annotation] */
		__in  ID3D11Resource* pDstResource,
		/* [annotation] */
		__in  UINT DstSubresource,
		/* [annotation] */
		__in_opt  const D3D11_BOX* pDstBox,
		/* [annotation] */
		__in  const void* pSrcData,
		/* [annotation] */
		__in  UINT SrcRowPitch,
		/* [annotation] */
		__in  UINT SrcDepthPitch);

	void A_ClearRenderTargetView(
		/* [annotation] */
		__in  ID3D11RenderTargetView* pRenderTargetView,
		/* [annotation] */
		__in  const FLOAT ColorRGBA[4]);

	void A_ClearDepthStencilView(

		__in  ID3D11DepthStencilView* pDepthStencilView,
		/* [annotation] */
		__in  UINT ClearFlags,
		/* [annotation] */
		__in  FLOAT Depth,
		/* [annotation] */
		__in  UINT8 Stencil);

	void A_DrawIndexed(
		/* [annotation] */
		__in  UINT IndexCount,
		/* [annotation] */
		__in  UINT StartIndexLocation,
		/* [annotation] */
		__in  INT BaseVertexLocation);

	void A_VSSetShader(
		/* [annotation] */
		__in_opt  ID3D11VertexShader* pVertexShader,
		/* [annotation] */
		__in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
		UINT NumClassInstances);

	void A_VSSetConstantBuffers(
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		__in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);

	void A_PSSetShader(
		/* [annotation] */
		__in_opt  ID3D11PixelShader* pPixelShader,
		/* [annotation] */
		__in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
		UINT NumClassInstances);

	void A_PSSetShaderResources(
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		__in_ecount(NumViews)  ID3D11ShaderResourceView* const* ppShaderResourceViews);

	void  A_PSSetSamplers(
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		__in_ecount(NumSamplers)  ID3D11SamplerState* const* ppSamplers);
//---------------------DEVICECONTEXT End-------------------///

	HRESULT A_GetBuffer(
		/* [in] */ UINT Buffer,
		/* [annotation][in] */
		__in  REFIID riid,
		/* [annotation][out][in] */
		__out  void** ppSurface);

	HRESULT A_ReleaseSwapChain();

	HRESULT A_Present(
		/* [in] */ UINT SyncInterval,
		/* [in] */ UINT Flags);


		
private:
	
	
	ADevice m_device;
	ADeviceContext m_devicecontext;
	ASwapChain m_swapchain;
};

