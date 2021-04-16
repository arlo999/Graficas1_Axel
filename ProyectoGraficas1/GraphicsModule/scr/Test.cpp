
#include "Test.h"
#include "..\Include\Test.h"



namespace GraphicsModule
{
	
#if defined(DX11)
	HRESULT Test::CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFileA(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			if (pErrorBlob) pErrorBlob->Release();
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();
		return S_OK;
	}
#endif

	HRESULT Test::InitDevice(HWND _hwnd)
	{
		

#if defined(DX11)
		m_hwnd = _hwnd;
		camera = new ACamera();
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

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

		// Compile the vertex shader
		ID3DBlob* pVSBlob = NULL;
		hr = CompileShaderFromFile("Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL,
				"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
			return hr;
		}

		// Create the vertex shader
		hr = g_pd3dDevice.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
		if (FAILED(hr))
		{
			pVSBlob->Release();
			return hr;
		}

		// Reflect shader info
		ID3D11ShaderReflection* pVertexShaderReflection = NULL;
		if (FAILED(D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
		{
			return S_FALSE;
		}

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		pVertexShaderReflection->GetDesc(&shaderDesc);

		// Read input layout description from shader info
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (UINT32 i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

			// fill out input element desc
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			//save element desc
			inputLayoutDesc.push_back(elementDesc);
		}

		// Try to create Input Layout
		 hr = g_pd3dDevice.A_CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

		//Free allocation shader reflection memory
		pVertexShaderReflection->Release();
		pVSBlob->Release();
		

		// Compile the pixel shader
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile("Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL,
				"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
			return hr;
		}

		// Create the pixel shader
		hr = g_pd3dDevice.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;



	
	
		
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		

		// Set primitive topology
		g_pImmediateContext.A_IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
			//light 
		bd.ByteWidth = sizeof(DirLigth);
		hr = g_pd3dDevice.A_CreateBuffer(&bd, NULL, &m_LigthBuffer.getBufferDX11());
		if (FAILED(hr))
			return hr;

		

		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = g_pd3dDevice.A_CreateSamplerState(&sampDesc, &g_pSamplerLinear);
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

		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
		
		g_pImmediateContext.A_UpdateSubresource(m_pCBChangeOnResize.getBufferDX11(), 0, NULL, &cbChangesOnResize, 0, 0);

	

#endif
		return S_OK;
	}

	void Test::Render()
	{

	

#if defined(DX11)
		// Update our time


		static float t = 0.0f;
		if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			t += (float)XM_PI * 0.0125f;
		}
		else
		{
			static DWORD dwTimeStart = 0;
			DWORD dwTimeCur = GetTickCount();
			if (dwTimeStart == 0)
				dwTimeStart = dwTimeCur;
			t = (dwTimeCur - dwTimeStart) / 1000.0f;
		}
		// Rotate cube around the origin
		g_View = XMMatrixRotationY(t);

		// Modify the color
		g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
		g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
		g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;


		// Clear the back buffer

		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha


		g_View = camera->getviewMLookLDirectX();
		cbNeverChanges.mView = XMMatrixTranspose(g_View);
		cb.mWorld = XMMatrixTranspose(g_World);
		cb.vMeshColor = g_vMeshColor;

		//g_pImmediateContext.A_UpdateSubresource(m_pCBChangesEveryFrame.getBufferDX11(), 0, NULL, &cb, 0, 0);

		g_pImmediateContext.A_UpdateSubresource(m_LigthBuffer.getBufferDX11(), 0, NULL, &m_LigthBufferStruct, 0, 0);

		



		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
		g_pImmediateContext.A_UpdateSubresource(m_pCBChangeOnResize.getBufferDX11(), 0, NULL, &cbChangesOnResize, 0, 0);


		///wordl settings---------//
		g_pImmediateContext.A_IASetInputLayout(g_pVertexLayout);
		g_pImmediateContext.A_VSSetShader(g_pVertexShader, NULL, 0);
		g_pImmediateContext.A_VSSetConstantBuffers(0, 1, &m_pCBNeverChanges.getBufferDX11());
		g_pImmediateContext.A_VSSetConstantBuffers(1, 1, &m_pCBChangeOnResize.getBufferDX11());
		g_pImmediateContext.A_VSSetConstantBuffers(2, 1, &m_pCBChangesEveryFrame.getBufferDX11());
		g_pImmediateContext.A_UpdateSubresource(m_pCBChangesEveryFrame.getBufferDX11(), 0, NULL, &cb, 0, 0);
		//light
		g_pImmediateContext.A_PSSetShader(g_pPixelShader, NULL, 0);
		g_pImmediateContext.A_VSSetConstantBuffers(3, 1, &m_LigthBuffer.getBufferDX11());
		//
		g_pImmediateContext.A_CSSetConstantBuffers(2, 1, &m_pCBChangesEveryFrame.getBufferDX11());
	

		g_pImmediateContext.A_PSSetSamplers(0, 1, &g_pSamplerLinear);
		//-------------------------
	
		g_pImmediateContext.A_ClearRenderTargetView(g_pRenderTargetView, ClearColor);
		g_pImmediateContext.A_ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		g_pImmediateContext.A_OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

		
		
	#endif	
	}

	void Test::CleanupDevice()
	{
	
#if defined(DX11)
		if (g_pImmediateContext.m_devicecontext) g_pImmediateContext.A_ClearState();
		if (g_pSamplerLinear) g_pSamplerLinear->Release();
		if (m_pCBChangesEveryFrame.getBufferDX11()) m_pCBChangesEveryFrame.Release();
		if (m_LigthBuffer.getBufferDX11()) m_LigthBuffer.Release();
		if (m_pCBChangeOnResize.getBufferDX11()) m_pCBChangeOnResize.Release();
		if (m_pCBNeverChanges.getBufferDX11()) m_pCBNeverChanges.Release();
		if (g_pVertexLayout) g_pVertexLayout->Release();
		if (g_pVertexShader) g_pVertexShader->Release();
		if (g_pPixelShader) g_pPixelShader->Release();
		if (g_pDepthStencil.getAtexture2D()) g_pDepthStencil.getAtexture2D()->Release();
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