#include "ADeviceContext.h"

void ADeviceContext::A_ClearState()
{
	 m_devicecontext->ClearState();
}

HRESULT ADeviceContext::A_Release()
{
	return m_devicecontext->Release();
}

void ADeviceContext::A_OMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)UINT NumViews, ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView)
{
	 m_devicecontext->OMSetRenderTargets(NumViews,ppRenderTargetViews,pDepthStencilView);
}

void __stdcall ADeviceContext::PSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers)
{
	m_devicecontext->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void ADeviceContext::A_RSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)UINT NumViewports, const D3D11_VIEWPORT* pViewports)
{
	 m_devicecontext->RSSetViewports(NumViewports,pViewports);
}

void ADeviceContext::A_IASetInputLayout(ID3D11InputLayout* pInputLayout)
{
	 m_devicecontext->IASetInputLayout(pInputLayout);
}

void ADeviceContext::A_IASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)UINT StartSlot, __in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)UINT NumBuffers,
ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
{
	 m_devicecontext->IASetVertexBuffers(StartSlot,NumBuffers,ppVertexBuffers,pStrides,pOffsets);
}

void ADeviceContext::A_IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
	 m_devicecontext->IASetIndexBuffer(pIndexBuffer,Format,Offset);
}

void ADeviceContext::A_IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	 m_devicecontext->IASetPrimitiveTopology(Topology);
}

void ADeviceContext::A_UpdateSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox, const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
	 m_devicecontext->UpdateSubresource(pDstResource,DstSubresource,pDstBox,pSrcData,SrcRowPitch,SrcDepthPitch);
}

void ADeviceContext::A_ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4])
{
	 m_devicecontext->ClearRenderTargetView(pRenderTargetView,ColorRGBA);
}

void ADeviceContext::A_ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
{
	 m_devicecontext->ClearDepthStencilView(pDepthStencilView,ClearFlags,Depth,Stencil);
}

void ADeviceContext::A_DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	 m_devicecontext->DrawIndexed(IndexCount,StartIndexLocation,BaseVertexLocation);
}

void ADeviceContext::A_VSSetShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
{
	 m_devicecontext->VSSetShader(pVertexShader,ppClassInstances,NumClassInstances);
}

void ADeviceContext::A_VSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)UINT StartSlot,
__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers)
{
	 m_devicecontext->VSSetConstantBuffers(StartSlot,NumBuffers,ppConstantBuffers);
}

void ADeviceContext::A_PSSetShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
{
	 m_devicecontext->PSSetShader(pPixelShader,ppClassInstances,NumClassInstances);
}

void ADeviceContext::A_PSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)UINT StartSlot,
__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
	 m_devicecontext->PSSetShaderResources(StartSlot,NumViews,ppShaderResourceViews);
}

void __stdcall ADeviceContext::A_RSSetState(ID3D11RasterizerState* pRasterizerState)
{
	m_devicecontext->RSSetState(pRasterizerState);
}

void __stdcall ADeviceContext::A_CSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers)
{
	m_devicecontext->CSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void ADeviceContext::A_PSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)UINT NumSamplers,
ID3D11SamplerState* const* ppSamplers)
{
	 m_devicecontext->PSSetSamplers(StartSlot,NumSamplers,ppSamplers);
}
