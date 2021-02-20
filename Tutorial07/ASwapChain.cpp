#include "ASwapChain.h"

HRESULT ASwapChain::A_GetBuffer(UINT Buffer, REFIID riid, void** ppSurface)
{
    return m_swapchain->GetBuffer(Buffer,riid,ppSurface);
}

HRESULT ASwapChain::A_Present(UINT SyncInterval, UINT Flags)
{
    return m_swapchain->Present(SyncInterval,Flags);
}
