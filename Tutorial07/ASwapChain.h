#pragma once
#include <d3d11.h>
class ASwapChain
{
public:
	ASwapChain() = default;
	~ASwapChain() = default;


	HRESULT A_GetBuffer(
		/* [in] */ UINT Buffer,
		/* [annotation][in] */
		__in  REFIID riid,
		/* [annotation][out][in] */
		__out  void** ppSurface);

	HRESULT A_Release();

	HRESULT A_Present(
		/* [in] */ UINT SyncInterval,
		/* [in] */ UINT Flags);

public:
	IDXGISwapChain* m_swapchain=NULL;

};

