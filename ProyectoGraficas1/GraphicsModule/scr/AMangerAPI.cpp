#include "AMangerAPI.h"

/////------------------DEVICE-------------------------/////
HRESULT AMangerAPI::A_CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
{
    return  m_device.A_CreateRenderTargetView(pResource, pDesc, ppRTView);;
}
/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT AMangerAPI::A_CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, __in_xcount_opt(pDesc -> MipLevels* pDesc -> ArraySize) const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
{
    return m_device.A_CreateTexture2D(pDesc, pInitialData, ppTexture2D);;
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT AMangerAPI::A_CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
{
    return  m_device.A_CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */

HRESULT AMangerAPI::A_CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
    return m_device.A_CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT AMangerAPI::A_CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
    __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
    return  m_device.A_CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature,
        BytecodeLength, ppInputLayout);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */

HRESULT AMangerAPI::A_CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
    return m_device.A_CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT AMangerAPI::A_CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer)
{
    return  m_device.A_CreateBuffer(pDesc, pInitialData, ppBuffer);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */

HRESULT AMangerAPI::A_CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
    return m_device.A_CreateSamplerState(pSamplerDesc, ppSamplerState);;
}

HRESULT AMangerAPI::A_ReleaseDevice()
{
    return m_device.A_Release();
}

//----------------------DEVICE---------------------///




//----------------------DEVICECONTEXT-------------/////



void AMangerAPI::A_ClearState()
{
    m_devicecontext.A_ClearState();
}

HRESULT AMangerAPI::A_ReleaseDeviceContext()
{
    return m_devicecontext.A_Release();
}

void AMangerAPI::A_OMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)UINT NumViews, ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView)
{
	m_devicecontext.A_OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

void AMangerAPI::A_RSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)UINT NumViewports, const D3D11_VIEWPORT* pViewports)
{
	m_devicecontext.A_RSSetViewports(NumViewports, pViewports);
}

void AMangerAPI::A_IASetInputLayout(ID3D11InputLayout* pInputLayout)
{
	m_devicecontext.A_IASetInputLayout(pInputLayout);
}

void AMangerAPI::A_IASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)UINT StartSlot, __in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)UINT NumBuffers,
	ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
{
	m_devicecontext.A_IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void AMangerAPI::A_IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
	m_devicecontext.A_IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

void AMangerAPI::A_IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	m_devicecontext.A_IASetPrimitiveTopology(Topology);
}

void AMangerAPI::A_UpdateSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox, const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
	m_devicecontext.A_UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

void AMangerAPI::A_ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4])
{
	m_devicecontext.A_ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

void AMangerAPI::A_ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
{
	m_devicecontext.A_ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
}

void AMangerAPI::A_DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	m_devicecontext.A_DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void AMangerAPI::A_VSSetShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
{
	m_devicecontext.A_VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

void AMangerAPI::A_VSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)UINT StartSlot,
	__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers)
{
	m_devicecontext.A_VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void AMangerAPI::A_PSSetShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
{
	m_devicecontext.A_PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

void AMangerAPI::A_PSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)UINT StartSlot,
	__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
	m_devicecontext.A_PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

void AMangerAPI::A_PSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)UINT NumSamplers,
	ID3D11SamplerState* const* ppSamplers)
{
	m_devicecontext.A_PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}


//----------------------DEVICECONTEXT-------------/////


///-----------------SWAP CHAIM------------------///


HRESULT AMangerAPI::A_GetBuffer(UINT Buffer, REFIID riid, void** ppSurface)
{
    return m_swapchain.A_GetBuffer(Buffer, riid, ppSurface);
}
HRESULT AMangerAPI::A_ReleaseSwapChain()
{
    return m_swapchain.A_Release();
}
HRESULT AMangerAPI::A_Present(UINT SyncInterval, UINT Flags)
{
    return m_swapchain.A_Present(SyncInterval, Flags);
}

///-----------------SWAP CHAIM------------------///
