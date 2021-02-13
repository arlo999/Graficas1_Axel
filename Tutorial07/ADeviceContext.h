#pragma once
class ADeviceContext
{
	ADeviceContext() = default;
	~ADeviceContext() = default;
	
	int A_ClearState();

	int A_Release();

	int A_OMSetRenderTargets(
		/* [annotation] */
		__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
		/* [annotation] */
		__in_ecount_opt(NumViews)  ID3D11RenderTargetView* const* ppRenderTargetViews,
		/* [annotation] */
		__in_opt  ID3D11DepthStencilView* pDepthStencilView);

	int A_RSSetViewports(
		/* [annotation] */
		__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
		/* [annotation] */
		__in_ecount_opt(NumViewports)  const D3D11_VIEWPORT* pViewports);

	int A_IASetInputLayout(
	
		/* [annotation] */
		__in_opt  ID3D11InputLayout* pInputLayout);

	int A_IASetVertexBuffers(
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

	int A_IASetIndexBuffer(
		/* [annotation] */
		__in_opt  ID3D11Buffer* pIndexBuffer,
		/* [annotation] */
		__in  DXGI_FORMAT Format,
		/* [annotation] */
		__in  UINT Offset);

	int A_IASetPrimitiveTopology(  
		/* [annotation] */
		__in  D3D11_PRIMITIVE_TOPOLOGY Topology);

	int A_UpdateSubresource(
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

	int A_ClearRenderTargetView(
		/* [annotation] */
		__in  ID3D11RenderTargetView* pRenderTargetView,
		/* [annotation] */
		__in  const FLOAT ColorRGBA[4]);

	int A_ClearDepthStencilView(
		ID3D11DeviceContext* This,
		/* [annotation] */
		__in  ID3D11DepthStencilView* pDepthStencilView,
		/* [annotation] */
		__in  UINT ClearFlags,
		/* [annotation] */
		__in  FLOAT Depth,
		/* [annotation] */
		__in  UINT8 Stencil);

	int A_DrawIndexed(
		/* [annotation] */
		__in  UINT IndexCount,
		/* [annotation] */
		__in  UINT StartIndexLocation,
		/* [annotation] */
		__in  INT BaseVertexLocation);

	int A_VSSetShader(
		/* [annotation] */
		__in_opt  ID3D11VertexShader* pVertexShader,
		/* [annotation] */
		__in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
		UINT NumClassInstances);

	int A_VSSetConstantBuffers(
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		__in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);

	int A_PSSetShader(  
		/* [annotation] */
		__in_opt  ID3D11PixelShader* pPixelShader,
		/* [annotation] */
		__in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
		UINT NumClassInstances);

	int A_PSSetShaderResources(   
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		__in_ecount(NumViews)  ID3D11ShaderResourceView* const* ppShaderResourceViews);
	
	int  A_PSSetSamplers(  
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		__in_ecount(NumSamplers)  ID3D11SamplerState* const* ppSamplers);
};

