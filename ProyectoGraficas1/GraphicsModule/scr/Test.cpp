
#include "Test.h"
#include "..\Include\Test.h"



namespace GraphicsModule
{
	


	HRESULT Test::InitDevice(HWND _hwnd)
	{
	#if defined(OGL)	
	
		 camera= new ACamera(glm::vec3(0.0f, 0.0f, 3.0f));
	
	#endif

#if defined(DX11)
		m_hwnd = _hwnd;
		camera = new ACamera();
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
	 width = rc.right - rc.left;
	height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			g_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &g_pSwapChain.m_swapchain, &g_pd3dDevice.m_device, &g_featureLevel, &g_pImmediateContext.m_devicecontext);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return hr;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = g_pSwapChain.A_GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		hr = g_pd3dDevice.A_CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = g_pd3dDevice.A_CreateTexture2D(&descDepth, NULL, &g_pDepthStencil.getAtexture2D());
		if (FAILED(hr))
			return hr;

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = g_pd3dDevice.A_CreateDepthStencilView(g_pDepthStencil.getAtexture2D(), &descDSV, &g_pDepthStencilView);
		if (FAILED(hr))
			return hr;

		// and the resource view for the shader
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1; // same as orig texture
		hr = g_pd3dDevice.A_CreateShaderResourceView(g_pDepthStencil.getAtexture2D(), &srvDesc, &g_pDepthStencilSRV);
		if (FAILED(hr))
			return hr;

		// Setup the viewport
		
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_pImmediateContext.A_RSSetViewports(1, &vp);
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		// Create the constant buffers
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CBNeverChanges);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_pCBNeverChanges.getBufferDX11());
		if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(CBChangeOnResize);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_pCBChangeOnResize.getBufferDX11());
		if (FAILED(hr))
			return hr;

		bd.ByteWidth = sizeof(CBChangesEveryFrame);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_pCBChangesEveryFrame.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(BoneTransform);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_BonetransformBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		// Create the constant buffers
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DirLigth);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		//light 
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_LigthBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(PointLight);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_PointLightBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(spotLight);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_SpotLightBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(Ambient);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_AmbientBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(Specular);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_SpecularBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(Shinies);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_ShiniesBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(Diffuse);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_DiffuseBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(Sao);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_SaoBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		bd.ByteWidth = sizeof(TooneMap);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_TooneMaBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;
		// Create the sample state

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;


		hr = g_pd3dDevice.A_CreateSamplerState(&samplerDesc, &g_normalMapSampler);
		if (FAILED(hr))
			return hr;
		hr = g_pd3dDevice.A_CreateSamplerState(&samplerDesc, &g_SpecularSampler);
		if (FAILED(hr))
			return hr;
		hr = g_pd3dDevice.A_CreateSamplerState(&samplerDesc, &g_DiffuseSampler);
		if (FAILED(hr))
			return hr;


		// Initialize the world matrices
		g_World = XMMatrixIdentity();

		
		camera->setEye(AVector(0.0f, 3.0f, -6.0f, 0));
		camera->setAt(AVector(0.0f, 1.0f, 0.0f, 0));
		camera->setUp(AVector(0.0f, 1.0f, 0.0f, 0));


		camera->setviewMLookL(camera->getEye(), camera->getAt(), camera->getUp());

		g_View = camera->getviewMLookLDirectX();

		cbNeverChanges.mView = XMMatrixTranspose(g_View);

		g_Projection = camera->ViewPerspective(XM_PIDIV4, width / (FLOAT)height, 0.01f, 1000.0f);

		g_pImmediateContext.A_UpdateSubresource(m_pCBNeverChanges.getBufferDX11(), 0, NULL, &cbNeverChanges, 0, 0);

		
		g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 1000.0f);

	

	

#endif


		return S_OK;
	}

	void Test::Render()
	{

	

#if defined(DX11)


		// Clear the back buffer

		float ClearColor[4] = { 0.0f, 1.125f, 0.3f, 1.0f }; // red, green, blue, alpha


		g_View = camera->getviewMLookLDirectX();
		cbNeverChanges.mView = XMMatrixTranspose(g_View);
		cb.mWorld = XMMatrixTranspose(g_World);
		g_pImmediateContext.A_IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);

		g_pImmediateContext.A_UpdateSubresource(m_pCBChangeOnResize.getBufferDX11(), 0, NULL, &cbChangesOnResize, 0, 0);
		//DirLight
		g_pImmediateContext.A_UpdateSubresource(m_LigthBuffer.getBufferDX11(), 0, NULL, &m_LigthBufferStruct, 0, 0);
		//Point Light
		g_pImmediateContext.A_UpdateSubresource(m_PointLightBuffer.getBufferDX11(), 0, NULL, &m_PointLightBufferStruct, 0, 0);
		//SpotLight
		g_pImmediateContext.A_UpdateSubresource(m_SpotLightBuffer.getBufferDX11(), 0, NULL, &m_SpotLightBufferStruct, 0, 0);
		//Ambient
		g_pImmediateContext.A_UpdateSubresource(m_AmbientBuffer.getBufferDX11(), 0, NULL, &m_AmbientBufferStruct, 0, 0);
		//specular
		g_pImmediateContext.A_UpdateSubresource(m_SpecularBuffer.getBufferDX11(), 0, NULL, &m_SpecularBufferStruct, 0, 0);
		//shinies
		g_pImmediateContext.A_UpdateSubresource(m_ShiniesBuffer.getBufferDX11(), 0, NULL, &m_ShiniesBufferStruct, 0, 0);
		//diffuse
		g_pImmediateContext.A_UpdateSubresource(m_DiffuseBuffer.getBufferDX11(), 0, NULL, &m_DiffuseBufferStruct, 0, 0);
		//sao
		g_pImmediateContext.A_UpdateSubresource(m_SaoBuffer.getBufferDX11(), 0, NULL, &m_SaoBufferStruct, 0, 0);
		//toone
		g_pImmediateContext.A_UpdateSubresource(m_TooneMaBuffer.getBufferDX11(), 0, NULL, &m_TooneMaBufferStruct, 0, 0);
		//bone
		g_pImmediateContext.A_UpdateSubresource(m_BonetransformBuffer.getBufferDX11(), 0, NULL, &m_BonetransformBufferStruct, 0, 0);
		//camara


		//dirLight
		g_pImmediateContext.A_VSSetConstantBuffers(3, 1, &m_LigthBuffer.getBufferDX11());
		//PointLight
		g_pImmediateContext.A_VSSetConstantBuffers(4, 1, &m_PointLightBuffer.getBufferDX11());
		//SpotLight
		g_pImmediateContext.A_VSSetConstantBuffers(5, 1, &m_SpotLightBuffer.getBufferDX11());
		//Ambient
		g_pImmediateContext.A_VSSetConstantBuffers(6, 1, &m_AmbientBuffer.getBufferDX11());
		//specular
		g_pImmediateContext.A_VSSetConstantBuffers(7, 1, &m_SpecularBuffer.getBufferDX11());
		//shinies
		g_pImmediateContext.A_VSSetConstantBuffers(8, 1, &m_ShiniesBuffer.getBufferDX11());
		//diffuse
		g_pImmediateContext.A_VSSetConstantBuffers(9, 1, &m_DiffuseBuffer.getBufferDX11());
		//sao
		g_pImmediateContext.A_VSSetConstantBuffers(10, 1, &m_SaoBuffer.getBufferDX11());

		//dirLight
		g_pImmediateContext.PSSetConstantBuffers(4, 1, &m_LigthBuffer.getBufferDX11());
		//PointLight
		g_pImmediateContext.PSSetConstantBuffers(5, 1, &m_PointLightBuffer.getBufferDX11());
		//SpotLight
		g_pImmediateContext.PSSetConstantBuffers(6, 1, &m_SpotLightBuffer.getBufferDX11());
		//Ambient
		g_pImmediateContext.PSSetConstantBuffers(7, 1, &m_AmbientBuffer.getBufferDX11());
		//specular
		g_pImmediateContext.PSSetConstantBuffers(8, 1, &m_SpecularBuffer.getBufferDX11());
		//shinies
		g_pImmediateContext.PSSetConstantBuffers(9, 1, &m_ShiniesBuffer.getBufferDX11());
		//diffuse
		g_pImmediateContext.PSSetConstantBuffers(10, 1, &m_DiffuseBuffer.getBufferDX11());
		//sao
		g_pImmediateContext.PSSetConstantBuffers(11, 1, &m_SaoBuffer.getBufferDX11());

		g_pImmediateContext.PSSetConstantBuffers(12, 1, &m_TooneMaBuffer.getBufferDX11());


		g_pImmediateContext.A_PSSetSamplers(0, 1, &g_DiffuseSampler);

		g_pImmediateContext.A_PSSetSamplers(1, 1, &g_normalMapSampler);
		g_pImmediateContext.A_PSSetSamplers(2, 1, &g_SpecularSampler);

		///---------------wordl settings---------//
		
		g_pImmediateContext.A_VSSetConstantBuffers(0, 1, &m_pCBNeverChanges.getBufferDX11());
		g_pImmediateContext.A_VSSetConstantBuffers(1, 1, &m_pCBChangeOnResize.getBufferDX11());
		g_pImmediateContext.A_VSSetConstantBuffers(2, 1, &m_pCBChangesEveryFrame.getBufferDX11());
		g_pImmediateContext.A_UpdateSubresource(m_pCBChangesEveryFrame.getBufferDX11(), 0, NULL, &cb, 0, 0);

		//-------------------------set sampler to shader-------------------------------///
		g_pImmediateContext.A_CSSetConstantBuffers(2, 1, &m_pCBChangesEveryFrame.getBufferDX11());
		
		
		//-------------------------back buffer-------------------------------///
	

		g_pImmediateContext.A_ClearRenderTargetView(g_pRenderTargetView, ClearColor);
		g_pImmediateContext.A_ClearDepthStencilView(g_pDepthStencilView,1 , 1.0f, 0);
		//g_pImmediateContext.A_OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

		
		
	#endif	
	}

	void Test::CleanupDevice()
	{
	
#if defined(DX11)
		if (g_pImmediateContext.m_devicecontext) g_pImmediateContext.A_ClearState();
		if (m_pCBChangesEveryFrame.getBufferDX11()) m_pCBChangesEveryFrame.Release();		
		if (m_pCBChangeOnResize.getBufferDX11()) m_pCBChangeOnResize.Release();
		if (m_pCBNeverChanges.getBufferDX11()) m_pCBNeverChanges.Release();
		if (g_pDepthStencil.getAtexture2D()) g_pDepthStencil.getAtexture2D()->Release();
		if(m_LigthBuffer.getBufferDX11())m_LigthBuffer.Release();
		if(m_PointLightBuffer.getBufferDX11())m_PointLightBuffer.Release();
		if(m_SpotLightBuffer.getBufferDX11())m_SpotLightBuffer.Release();
		if(m_AmbientBuffer.getBufferDX11())m_AmbientBuffer.Release();
		if(m_SpecularBuffer.getBufferDX11())m_SpecularBuffer.Release();
		if(m_ShiniesBuffer.getBufferDX11())m_ShiniesBuffer.Release();
		if(m_DiffuseBuffer.getBufferDX11())m_DiffuseBuffer.Release();
		if(m_SaoBuffer.getBufferDX11())m_SaoBuffer.Release();
		if(m_TooneMaBuffer.getBufferDX11())m_TooneMaBuffer.Release();
		if (g_pDepthStencilView) g_pDepthStencilView->Release();
		if (g_pRenderTargetView) g_pRenderTargetView->Release();
		if (g_pSwapChain.m_swapchain) g_pSwapChain.A_Release();
		if (g_pImmediateContext.m_devicecontext) g_pImmediateContext.A_Release();
		if (g_pd3dDevice.m_device) g_pd3dDevice.A_Release();
		#endif
	}

	

	
	void Test::Update()
	{
#if defined(DX11)
		
#endif
	}


	/**
 * @brief   Esta funcion regenera el size de la pantalla limpiando los buffer y viewports
 * @return  #HRESULT: retorna un resultado de la funcion como si todo salio a la perfeccion o si hubo fallos
 * @bug     No know Bugs.
 * @return  #LRESULT: Status code.
 */
#if defined(DX11)
	
	HRESULT Test::ReloadBuffer(unsigned int width, unsigned int height) {
		
		HRESULT hr = S_OK;
			// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = g_pSwapChain.A_GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		hr = g_pd3dDevice.A_CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = g_pd3dDevice.A_CreateTexture2D(&descDepth, NULL, &g_pDepthStencil.getAtexture2D());
		if (FAILED(hr))
			return hr;

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = g_pd3dDevice.A_CreateDepthStencilView(g_pDepthStencil.getAtexture2D(), &descDSV, &g_pDepthStencilView);
		if (FAILED(hr))
			return hr;

		// and the resource view for the shader
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1; // same as orig texture
		hr = g_pd3dDevice.A_CreateShaderResourceView(g_pDepthStencil.getAtexture2D(), &srvDesc, &g_pDepthStencilSRV);
		if (FAILED(hr))
			return hr;

		g_pImmediateContext.A_OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

		// Setup the viewport

		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_pImmediateContext.A_RSSetViewports(1, &vp);

		/// ESTO DEBE ESTA EN EL MANAGER

	//TEXTURA CON LA QUE SE HACE LA DESCRIPCION
		ID3D11Texture2D* Text2D = NULL;

		D3D11_TEXTURE2D_DESC descTextRT;
		ZeroMemory(&descTextRT, sizeof(descTextRT));
		descTextRT.Width = width;
		descTextRT.Height = height;
		descTextRT.MipLevels = 1;
		descTextRT.ArraySize = 1;
		descTextRT.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		descTextRT.SampleDesc.Count = 1;
		descTextRT.SampleDesc.Quality = 0;
		descTextRT.Usage = D3D11_USAGE_DEFAULT;
		descTextRT.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		descTextRT.CPUAccessFlags = 0;
		descTextRT.MiscFlags = 0;
		//CAMBIAR EN EL MANAGER
		hr = g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &Text2D);
		if (FAILED(hr))
			return hr;

	}	
#endif
	//static_cast<D>(TITO);
extern 	Test& GetTestObj(HWND _hwnd)
	{
		static Test* pTest;
		if (pTest == nullptr)
		{
			pTest = new Test();
			pTest->InitDevice( _hwnd);
		}
		return *pTest;
	}
}