#include "APase.h"
#include "ARenderManager.h"
APase::APase()
{

}

APase::APase(bool _PostProceso)
{
	m_PostProceso = _PostProceso;
}

APase::APase(int _typePase)
{
	m_TypePase=_typePase;
}

APase::~APase()
{
}


HRESULT APase::Init()
{
	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/
	
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM= RManager::SingletonRM();
	HRESULT hr = S_OK;
	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("VertexShaderD11.fx", RM.m_macros.data(), "VS_main", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = testObj.g_pd3dDevice.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		
	}

	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		pVSBlob->Release();
		return hr;
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
	hr = testObj.g_pd3dDevice.A_CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	pVSBlob->Release();


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("PixelShaderD11.fx",RM.m_macros.data(), "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = testObj.g_pd3dDevice.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	if (FAILED(hr)) {


		pPSBlob->Release();
		return hr;
	}





}

void APase::Render()
{

	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();
	m_ModelList	=	RM.m_ModelList;
	testObj.g_pImmediateContext.A_RSSetState(m_Rasterizador);
	testObj.g_pImmediateContext.A_IASetInputLayout(g_pVertexLayout);
	testObj.g_pImmediateContext.A_VSSetShader(g_pVertexShader, NULL, 0);
	testObj.g_pImmediateContext.A_PSSetShader(g_pPixelShader, NULL, 0);


	testObj.g_pImmediateContext.A_PSSetSamplers(0, 1, &RM.m_generico);
	testObj.g_pImmediateContext.A_PSSetSamplers(1, 1, &RM.m_generico);
	testObj.g_pImmediateContext.A_PSSetSamplers(2, 1, &RM.m_generico);
	testObj.g_pImmediateContext.A_PSSetSamplers(3, 1, &RM.m_generico);
	testObj.g_pImmediateContext.A_PSSetSamplers(4, 1, &RM.m_generico);
	testObj.g_pImmediateContext.A_PSSetSamplers(5, 1, &RM.m_generico);
	testObj.g_pImmediateContext.A_PSSetSamplers(6, 1, &RM.m_generico);
	
	if (m_PostProceso) {
		
		
		
		
		testObj.g_pImmediateContext.A_PSSetShaderResources(0, 1, &RM.m_AlbedoSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, &RM.m_NormalSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(2, 1, &RM.m_SpecularSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(3, 1, &RM.m_PositionSRV);
		//testObj.g_pImmediateContext.A_PSSetShaderResources(4, 1, &RM.m_SSaoSRV);

		//testObj.g_pImmediateContext.A_PSSetShaderResources(5, 1, &RM.m_FinalSRV);
		//testObj.g_pImmediateContext.A_PSSetShaderResources(6, 1, &RM.m_SkyboxMapSRV);
		
		//testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &testObj.g_pRenderTargetView, NULL);

		RM.m_ScreenAlignedQuad.Rendersaq();

		
		




	}else if(m_TypePase==1){
	/*
		testObj.g_pImmediateContext.A_PSSetShaderResources(0, 1, &RM.m_AlbedoSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, &RM.m_NormalSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(2, 1, &RM.m_SpecularSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(3, 1, &RM.m_PositionSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(5, 1, &RM.m_FinalSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(4, 1, &RM.m_SSaoSRV);
		testObj.g_pImmediateContext.A_PSSetShaderResources(6, 1, &RM.m_SkyboxMapSRV);

		//testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &testObj.g_pRenderTargetView, NULL);

		RM.m_ScreenAlignedQuad.Rendersaq();
		
	*/
	}else{
	
		
		for (int i = 0; i < RM.m_RTVList.size(); i++)
		{

			testObj.g_pImmediateContext.A_ClearRenderTargetView(RM.m_RTVList[i], ClearColor);
		}


		
		//testObj.g_pImmediateContext.A_ClearDepthStencilView(testObj.g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		testObj.g_pImmediateContext.A_OMSetRenderTargets(RM.m_RTVList.size(), RM.m_RTVList.data(), testObj.g_pDepthStencilView);

		for (int i = 0; i < m_ModelList.size(); i++)
		{
			m_ModelList[i]->Render();
		}
	
		testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &testObj.g_pRenderTargetView, testObj.g_pDepthStencilView);

	}


	/*
	ID3D11ShaderResourceView* const pSRV[1] = { NULL };

	testObj.g_pImmediateContext.A_PSSetShaderResources(0, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(2, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(3, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(4, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(5, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(6, 1, pSRV);
	*/


	
}

HRESULT APase::InitGBuffer()
{

	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();
	
	
	
	HRESULT hr = S_OK;
	
	D3D11_RASTERIZER_DESC dr;
	dr.CullMode=D3D11_CULL_BACK;
	dr.FillMode=D3D11_FILL_SOLID;
	dr.FrontCounterClockwise=false;
	dr.DepthBiasClamp =0;
	dr.DepthClipEnable=0;
	dr.SlopeScaledDepthBias=0;
	dr.DepthClipEnable=FALSE;
	dr.MultisampleEnable=TRUE;
	dr.ScissorEnable=FALSE;
	dr.AntialiasedLineEnable=FALSE;
	dr.DepthBias=0;
	
	hr= testObj.g_pd3dDevice.A_CreateRasterizerState(&dr,&m_Rasterizador);
	if (FAILED(hr))
	{

	}
	// Compile the vertex shader
	
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("Gbuffervs.fx", RM.m_macros.data(), "vs_main", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = testObj.g_pd3dDevice.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{

	}

	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		pVSBlob->Release();
		return hr;
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
	hr = testObj.g_pd3dDevice.A_CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	pVSBlob->Release();


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("Gbufferps.fx", RM.m_macros.data(), "ps_main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = testObj.g_pd3dDevice.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	if (FAILED(hr)) {


		pPSBlob->Release();
		return hr;
	}



	return S_OK;




}

HRESULT APase::InitCopy()
{
	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();



	HRESULT hr = S_OK;


	D3D11_RASTERIZER_DESC dr;
	dr.CullMode = D3D11_CULL_NONE;
	dr.FillMode = D3D11_FILL_SOLID;
	dr.FrontCounterClockwise = true;
	dr.DepthBiasClamp = 0;
	dr.DepthClipEnable = 0;
	dr.SlopeScaledDepthBias = 0;
	dr.DepthClipEnable = FALSE;
	dr.MultisampleEnable = TRUE;
	dr.ScissorEnable = FALSE;
	dr.AntialiasedLineEnable = FALSE;
	dr.DepthBias = 0;

	hr = testObj.g_pd3dDevice.A_CreateRasterizerState(&dr, &m_Rasterizador);
	if (FAILED(hr))
	{

	}

	// Compile the vertex shader

	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("GTCopyvs.fx", RM.m_macros.data(), "vs_main", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = testObj.g_pd3dDevice.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{

	}

	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		pVSBlob->Release();
		return hr;
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
	hr = testObj.g_pd3dDevice.A_CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	pVSBlob->Release();


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("GTCopyps.fx", RM.m_macros.data(), "ps_main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = testObj.g_pd3dDevice.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	if (FAILED(hr)) {


		pPSBlob->Release();
		return hr;
	}



	return S_OK;



}

HRESULT APase::InitLight()
{

	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();



	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC dr;
	dr.CullMode = D3D11_CULL_NONE;
	dr.FillMode = D3D11_FILL_SOLID;
	dr.FrontCounterClockwise = true;
	dr.DepthBiasClamp = 0;
	dr.DepthClipEnable = 0;
	dr.SlopeScaledDepthBias = 0;
	dr.DepthClipEnable = FALSE;
	dr.MultisampleEnable = TRUE;
	dr.ScissorEnable = FALSE;
	dr.AntialiasedLineEnable = FALSE;
	dr.DepthBias = 0;

	hr = testObj.g_pd3dDevice.A_CreateRasterizerState(&dr, &m_Rasterizador);
	if (FAILED(hr))
	{

	}


	// Compile the vertex shader

	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("GLightvs.fx", RM.m_macros.data(), "vs_main", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = testObj.g_pd3dDevice.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{

	}

	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		pVSBlob->Release();
		return hr;
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
	hr = testObj.g_pd3dDevice.A_CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	pVSBlob->Release();


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("GLightps.fx", RM.m_macros.data(), "ps_main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = testObj.g_pd3dDevice.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	if (FAILED(hr)) {


		pPSBlob->Release();
		return hr;
	}



	return S_OK;



}

HRESULT APase::InitSao()
{

	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();



	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC dr;
	dr.CullMode = D3D11_CULL_NONE;
	dr.FillMode = D3D11_FILL_SOLID;
	dr.FrontCounterClockwise = true;
	dr.DepthBiasClamp = 0;
	dr.DepthClipEnable = 0;
	dr.SlopeScaledDepthBias = 0;
	dr.DepthClipEnable = FALSE;
	dr.MultisampleEnable = TRUE;
	dr.ScissorEnable = FALSE;
	dr.AntialiasedLineEnable = FALSE;
	dr.DepthBias = 0;

	hr = testObj.g_pd3dDevice.A_CreateRasterizerState(&dr, &m_Rasterizador);
	if (FAILED(hr))
	{

	}


	// Compile the vertex shader

	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("GSSAOvs.fx", RM.m_macros.data(), "vs_main", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = testObj.g_pd3dDevice.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{

	}

	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		pVSBlob->Release();
		return hr;
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
	hr = testObj.g_pd3dDevice.A_CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	pVSBlob->Release();


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("GSSAOps.fx", RM.m_macros.data(), "ps_main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = testObj.g_pd3dDevice.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	if (FAILED(hr)) {


		pPSBlob->Release();
		return hr;
	}



	return S_OK;

}

HRESULT APase::InitTooneMap()
{
	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();



	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC dr;
	dr.CullMode = D3D11_CULL_NONE;
	dr.FillMode = D3D11_FILL_SOLID;
	dr.FrontCounterClockwise = true;
	dr.DepthBiasClamp = 0;
	dr.DepthClipEnable = 0;
	dr.SlopeScaledDepthBias = 0;
	dr.DepthClipEnable = FALSE;
	dr.MultisampleEnable = TRUE;
	dr.ScissorEnable = FALSE;
	dr.AntialiasedLineEnable = FALSE;
	dr.DepthBias = 0;

	hr = testObj.g_pd3dDevice.A_CreateRasterizerState(&dr, &m_Rasterizador);
	if (FAILED(hr))
	{

	}


	// Compile the vertex shader

	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("GTooneMapvs.fx", RM.m_macros.data(), "vs_main", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = testObj.g_pd3dDevice.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{

	}

	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		pVSBlob->Release();
		return hr;
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
	hr = testObj.g_pd3dDevice.A_CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	pVSBlob->Release();


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("GTooneMapps.fx", RM.m_macros.data(), "ps_main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = testObj.g_pd3dDevice.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	if (FAILED(hr)) {


		pPSBlob->Release();
		return hr;
	}



	return S_OK;
}

HRESULT APase::InitSkybox()
{
	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();



	HRESULT hr = S_OK;


	D3D11_RASTERIZER_DESC dr;
	dr.CullMode = D3D11_CULL_NONE;
	dr.FillMode = D3D11_FILL_SOLID;
	dr.FrontCounterClockwise = true;
	dr.DepthBiasClamp = 0;
	dr.DepthClipEnable = 0;
	dr.SlopeScaledDepthBias = 0;
	dr.DepthClipEnable = FALSE;
	dr.MultisampleEnable = TRUE;
	dr.ScissorEnable = FALSE;
	dr.AntialiasedLineEnable = FALSE;
	hr = testObj.g_pd3dDevice.A_CreateRasterizerState(&dr, &m_Rasterizador);
	if (FAILED(hr))
	{

	}

	// Compile the vertex shader

	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("GSkyboxvs.fx", RM.m_macros.data(), "vs_main", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = testObj.g_pd3dDevice.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{

	}

	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		pVSBlob->Release();
		return hr;
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
	hr = testObj.g_pd3dDevice.A_CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	pVSBlob->Release();


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("GSkyboxps.fx", RM.m_macros.data(), "ps_main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = testObj.g_pd3dDevice.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	if (FAILED(hr)) {


		pPSBlob->Release();
		return hr;
	}

	RM.m_Skybox.transform.scale[0]=1;
	RM.m_Skybox.transform.scale[1]=1;
	RM.m_Skybox.transform.scale[2]=1;


	return S_OK;
}




HRESULT APase::CompileShaderFromFile(const char* szFileName,const D3D10_SHADER_MACRO* _Macros, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFileA(szFileName, _Macros, NULL, szEntryPoint, szShaderModel,
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

	

