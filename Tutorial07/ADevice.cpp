#include "ADevice.h"

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT ADevice::A_CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
{
    return  m_device->CreateRenderTargetView(pResource, pDesc, ppRTView);;
}


/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT ADevice::A_CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, __in_xcount_opt(pDesc -> MipLevels* pDesc -> ArraySize) const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
{
    return m_device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);;
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT ADevice::A_CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
{
   return  m_device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */

HRESULT ADevice::A_CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
    return m_device->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT ADevice::A_CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
    __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
	return  m_device->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature,
        BytecodeLength, ppInputLayout);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */

HRESULT ADevice::A_CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
    return m_device->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */
HRESULT ADevice::A_CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer)
{
    return  m_device->CreateBuffer(pDesc, pInitialData, ppBuffer);
}

/**
     * @brief   Esta funcion crea el targetView
     * @param   #??.
     * @param   #??.
     * @param   #??.
     * @return  #int : si se regresa 1 es correcto  y 0 incorrecto
     */

HRESULT ADevice::A_CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
    return m_device->CreateSamplerState(pSamplerDesc, ppSamplerState);;
}
