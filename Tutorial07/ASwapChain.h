#pragma once
class ASwapChain
{
public:
	ASwapChain() = default;
	~ASwapChain() = default;

	int A_GetBuffer(  
		/* [in] */ UINT Buffer,
		/* [annotation][in] */
		__in  REFIID riid,
		/* [annotation][out][in] */
		__out  void** ppSurface);

	int A_Release();

	int A_Present(  
		/* [in] */ UINT SyncInterval,
		/* [in] */ UINT Flags);



};

