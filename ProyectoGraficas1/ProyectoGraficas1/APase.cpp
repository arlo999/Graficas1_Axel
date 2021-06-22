#include "APase.h"
#include "ARenderManager.h"
APase::APase()
{

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
#if defined(DX11)
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

#endif

return S_OK;

}

void APase::Render()
{

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();
	m_ModelList	=	RM.m_ModelList;
#if defined(DX11)
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	float red[4] = { 1.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

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
	testObj.g_pImmediateContext.A_PSSetSamplers(7, 1, &RM.m_generico);
	testObj.g_pImmediateContext.A_PSSetSamplers(8, 1, &RM.m_generico);
#endif
	if (RM.m_Forward) {
	
		if (m_TypePase == RManager::LIGHT) {
		/*
			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &m_ListRenderTV[0], NULL);
		*/
#if defined(DX11)
			testObj.g_pImmediateContext.A_PSSetShaderResources(0, 1, &RM.m_AlbedoSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, &RM.m_NormalSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(2, 1, &RM.m_SpecularSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(7, 1, &RM.m_SkyboxMapSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(8, 1, &RM.m_SkyboxNormalMapSRV);

			for (int i = 0; i < m_ModelList.size(); i++)
			{
				m_ModelList[i]->Render(RM.m_shaderLight);
			}
#endif
#if defined(OGL)	


			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, RM.LightForwardRT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			RM.m_shaderlightF.Use();
			
			//position
			RM.m_shaderlightF.setInt("texture_skyboxmap", 3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, RM.Skyboxsrv);

			RM.m_shaderlightF.setFloat4("viewPosition", testObj.camera->getEye().getX(), testObj.camera->getEye().getY(), testObj.camera->getEye().getZ(), testObj.camera->getEye().getM());

			glm::mat4 projection = glm::perspective(glm::radians(testObj.camera->Zoom), 1270.0f / 720.0f, 0.1f, 100.0f);
			glm::mat4 view = testObj.camera->GetViewMatrixGlm();
			RM.m_shaderlightF.setMat4("proj", projection);
			RM.m_shaderlightF.setMat4("view", view);

			for (int i = 0; i < m_ModelList.size(); i++)
			{
				m_ModelList[i]->Draw(RM.m_shaderlightF);
			}
		
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

#endif

		}
		else if (m_TypePase == RManager::SKYBOX) {

#if defined(DX11)
			testObj.g_pImmediateContext.A_ClearRenderTargetView(m_ListRenderTV[0], red);

			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &m_ListRenderTV[0], NULL);

			testObj.g_pImmediateContext.A_PSSetShaderResources(7, 1, &RM.m_SkyboxMapSRV);

			RM.m_Skybox.Rendersaq(RM.m_shaderSkybox);

#endif
#if defined(OGL)


			glBindFramebuffer(GL_FRAMEBUFFER, RM.LightForwardRT);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RM.LightForwardsrv, 0);

			unsigned int attachmentsCopy[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, attachmentsCopy);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;



			RM.m_shaderSkybox.Use();
			glm::mat4 projection = glm::perspective(glm::radians(testObj.camera->Zoom), 1270.0f / 720.0f, 0.1f, 100.0f);
			glm::mat4 view = testObj.camera->GetViewMatrixGlm();
			RM.m_shaderSkybox.setMat4("proj", projection);
			RM.m_shaderSkybox.setMat4("view", view);


			RM.m_shaderSkybox.setInt("samSkyboxMap", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, RM.Skyboxsrv);
			glClear(GL_DEPTH_BUFFER_BIT);
			//glViewport(0, 0, 1270, 720);

			//glBlitFramebuffer(0, 0, 1270, 720, 0, 0, 1270, 720, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			RM.m_Skybox.Render(RM.m_shaderSkybox);
			//RM.m_ScreenAlignedQuad.Render(RM.m_shaderSkybox);	




#endif
		}
		else if (m_TypePase == RManager::TONEMAP) {
#if defined(DX11)	
			testObj.g_pImmediateContext.A_ClearRenderTargetView(m_ListRenderTV[0], red);

			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &m_ListRenderTV[0], NULL);



			testObj.g_pImmediateContext.A_PSSetShaderResources(4, 1, &RM.m_LightSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(5, 1, &RM.m_SSaoSRV);


			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderToneMap);
			#endif

#if defined(OGL)

			glBindFramebuffer(GL_FRAMEBUFFER, RM.ToneMapForwardRT);

			RM.m_shaderToneMap.Use();
			//LIGHT
			glActiveTexture(GL_TEXTURE0);
			RM.m_shaderToneMap.setInt("texture_Light", 0);
			glBindTexture(GL_TEXTURE_2D, RM.LightForwardsrv);

		

			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderToneMap);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

		}
		else if (m_TypePase == RManager::COPY) {


#if defined(DX11)
			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &testObj.g_pRenderTargetView, NULL);

			testObj.g_pImmediateContext.A_PSSetShaderResources(6, 1, &RM.m_ToonMapSRV);



			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderCopy);
#endif
#if defined(OGL)


			glBindFramebuffer(GL_FRAMEBUFFER, RM.CopyForwardRT);

			RM.m_shaderCopy.Use();
			glActiveTexture(GL_TEXTURE0);
			RM.m_shaderCopy.setInt("texture_final", 0);

			glBindTexture(GL_TEXTURE_2D, RM.LightForwardsrv);



			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderCopy);





#endif



		}	
		
	
	
	}
	else
	{ 



		if (m_TypePase==RManager::LIGHT) {

#if defined(DX11)			
			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &m_ListRenderTV[0], NULL);

			testObj.g_pImmediateContext.A_PSSetShaderResources(0, 1, &RM.m_AlbedoSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, &RM.m_NormalSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(2, 1, &RM.m_SpecularSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(3, 1, &RM.m_PositionSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(7, 1, &RM.m_SkyboxMapSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(8, 1, &RM.m_SkyboxNormalMapSRV);
		
			
	

			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderLight);

			#endif
#if defined(OGL)	

		
			


			RM.m_shaderLight.Use();
			//albedo
			glActiveTexture(GL_TEXTURE0 );
			RM.m_shaderLight.setInt("texture_albedo",0);
			glBindTexture(GL_TEXTURE_2D, RM.gAlbedo);
			//normal
			glActiveTexture(GL_TEXTURE1 );
			RM.m_shaderLight.setInt("texture_normalMap", 1);
			glBindTexture(GL_TEXTURE_2D, RM.gNormal);
			//specular
			glActiveTexture(GL_TEXTURE2);
			RM.m_shaderLight.setInt("texture_specular", 2);
			glBindTexture(GL_TEXTURE_2D, RM.gAlbedoSpec);
			//position
			glActiveTexture(GL_TEXTURE3);
			RM.m_shaderLight.setInt("texture_position", 3);
			glBindTexture(GL_TEXTURE_2D, RM.gPosition);
			glActiveTexture(GL_TEXTURE4);
			RM.m_shaderLight.setInt("texture_skyboxmap", 4);
			glBindTexture(GL_TEXTURE_CUBE_MAP, RM.Skyboxsrv);
		
			RM.m_shaderLight.setFloat4("viewPosition", testObj.camera->getEye().getX(), testObj.camera->getEye().getY(), testObj.camera->getEye().getZ(), testObj.camera->getEye().getM());
			
			
			glClear(GL_DEPTH_BUFFER_BIT);
			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderLight);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			#endif

		}
		else if (m_TypePase == RManager::SKYBOX) {

#if defined(DX11)

			testObj.g_pImmediateContext.A_ClearRenderTargetView(m_ListRenderTV[0], red);

			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &m_ListRenderTV[0], NULL);

			testObj.g_pImmediateContext.A_PSSetShaderResources(7, 1, &RM.m_SkyboxMapSRV);

			RM.m_Skybox.Rendersaq(RM.m_shaderSkybox);
		
		#endif
#if defined(OGL)
		
		
			glBindFramebuffer(GL_FRAMEBUFFER, RM.LightRT);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RM.Lightsrv, 0);

			unsigned int attachmentsCopy[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, attachmentsCopy);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
		

			
			RM.m_shaderSkybox.Use();
			glm::mat4 projection = glm::perspective(glm::radians(testObj.camera->Zoom), 1270.0f / 720.0f, 0.1f, 100.0f);
			glm::mat4 view = testObj.camera->GetViewMatrixGlm();
			RM.m_shaderSkybox.setMat4("proj", projection);
			RM.m_shaderSkybox.setMat4("view", view);
			

			RM.m_shaderSkybox.setInt("samSkyboxMap", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, RM.Skyboxsrv);
			glClear( GL_DEPTH_BUFFER_BIT);
			glViewport(0,0,1270,720);
		
			//glBlitFramebuffer(0, 0, 1270, 720, 0, 0, 1270, 720, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			RM.m_Skybox.Render(RM.m_shaderSkybox);
			//RM.m_ScreenAlignedQuad.Render(RM.m_shaderSkybox);	
		
		
			
		
#endif
		}
		else if (m_TypePase == RManager::AO) {
#if defined(DX11)
			testObj.g_pImmediateContext.A_ClearRenderTargetView(m_ListRenderTV[0], red);

			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &m_ListRenderTV[0], NULL);

			testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, &RM.m_NormalSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(3, 1, &RM.m_PositionSRV);

			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderSAO);

			#endif
#if defined(OGL)
		
			glBindFramebuffer(GL_FRAMEBUFFER, RM.SAORT);
		
			RM.m_shaderSAO.Use();
			//normal
			glActiveTexture(GL_TEXTURE0);
			RM.m_shaderSAO.setInt("sampnormalMap", 0);
			glBindTexture(GL_TEXTURE_2D, RM.gNormal);
			//position
			glActiveTexture(GL_TEXTURE1);
			RM.m_shaderSAO.setInt("samplerPosition", 1);
			glBindTexture(GL_TEXTURE_2D, RM.gPosition);
			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderSAO);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif


		}
		else if (m_TypePase == RManager::TONEMAP) {
#if defined(DX11)
			testObj.g_pImmediateContext.A_ClearRenderTargetView(m_ListRenderTV[0], red);

			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &m_ListRenderTV[0], NULL);



			testObj.g_pImmediateContext.A_PSSetShaderResources(4, 1, &RM.m_LightSRV);
			testObj.g_pImmediateContext.A_PSSetShaderResources(5, 1, &RM.m_SSaoSRV);


			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderToneMap);
			#endif
			#if defined(OGL)
			
		
			glBindFramebuffer(GL_FRAMEBUFFER, RM.TonemapRT);
		
			RM.m_shaderToneMap.Use();
			//LIGHT
			glActiveTexture(GL_TEXTURE0 );
			RM.m_shaderToneMap.setInt("texture_Light", 0);
			glBindTexture(GL_TEXTURE_2D, RM.Lightsrv);
			
			glActiveTexture(GL_TEXTURE1);
			RM.m_shaderToneMap.setInt("texture_Sao", 1);
			glBindTexture(GL_TEXTURE_2D, RM.SAOsrv);

			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderToneMap);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
			#endif


		}
		else if (m_TypePase == RManager::COPY) {
#if defined(DX11)
			testObj.g_pImmediateContext.A_OMSetRenderTargets(1, &testObj.g_pRenderTargetView, NULL);
			
			testObj.g_pImmediateContext.A_PSSetShaderResources(6, 1, &RM.m_ToonMapSRV);

			RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderCopy);

			#endif
#if defined(OGL)	
			


		
			glBindFramebuffer(GL_FRAMEBUFFER, RM.CopyRT);
		
			RM.m_shaderCopy.Use();
			glActiveTexture(GL_TEXTURE0);
			RM.m_shaderCopy.setInt("texture_final",0);
			
			glBindTexture(GL_TEXTURE_2D, RM.toneMapsrv);
			
		
		
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		RM.m_ScreenAlignedQuad.Rendersaq(RM.m_shaderCopy);
	
		
#endif
		}else if (m_TypePase == RManager::GBUFFER) {

		
#if defined(DX11)

			for (int i = 0; i < m_ListRenderTV.size(); i++)
			{

				testObj.g_pImmediateContext.A_ClearRenderTargetView(m_ListRenderTV[i], ClearColor);
			}

			testObj.g_pImmediateContext.A_ClearDepthStencilView(testObj.g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
			testObj.g_pImmediateContext.A_OMSetRenderTargets(m_ListRenderTV.size(), m_ListRenderTV.data(), testObj.g_pDepthStencilView);

			for (int i = 0; i < m_ModelList.size(); i++)
			{
				m_ModelList[i]->Render(RM.m_shaderGBuffer);
			}
			
		#endif	
#if defined(OGL)	
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, RM.gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RM.m_shaderGBuffer.Use();	

			glm::mat4 projection = glm::perspective(glm::radians(testObj.camera->Zoom), 1270.0f / 720.0f, 0.1f, 100.0f);
			glm::mat4 view = testObj.camera->GetViewMatrixGlm();
			RM.m_shaderGBuffer.setMat4("proj",projection);
			RM.m_shaderGBuffer.setMat4("view",view);
		
			for (int i = 0; i < m_ModelList.size(); i++)
			{
				m_ModelList[i]->Draw(RM.m_shaderGBuffer);
			}
		
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


		

#endif
		}

	}

	
#if defined(DX11)
	ID3D11ShaderResourceView* const pSRV[1] = { NULL };

	testObj.g_pImmediateContext.A_PSSetShaderResources(0, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(2, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(3, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(4, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(5, 1, pSRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(6, 1, pSRV);

	#endif
	
	
}

HRESULT APase::InitGBuffer()
{

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();
#if defined(DX11)
	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/

	m_ListRenderTV.push_back(RM.m_AlbedoRT);
	m_ListRenderTV.push_back(RM.m_NormalRT);
	m_ListRenderTV.push_back(RM.m_SpecularRT);
	m_ListRenderTV.push_back(RM.m_PositionRT);
	
	
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

	#endif
#if defined(OGL) 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	glFrontFace(GL_CW);
	
	RM.m_shaderGBuffer.Init("ShaderOGL\\GBuffer.vs", "ShaderOGL\\GBuffer.fs");
	#endif
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
#if defined(DX11)
	

	m_ListRenderTV.push_back(RM.m_ToonRT);


	HRESULT hr = S_OK;


	D3D11_RASTERIZER_DESC dr;
	dr.CullMode = D3D11_CULL_NONE;
	dr.FillMode = D3D11_FILL_SOLID;
	dr.FrontCounterClockwise = false;
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

	#endif
#if defined(OGL)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	
	RM.m_shaderCopy.Init("ShaderOGL\\Copy.vs", "ShaderOGL\\Copy.fs");
	
	#endif
	return S_OK;


}

HRESULT APase::InitLight()
{
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();

#if defined(DX11)
	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/

	

	m_ListRenderTV.push_back(RM.m_LightRT);


	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC dr;
	dr.CullMode = D3D11_CULL_NONE;
	dr.FillMode = D3D11_FILL_SOLID;
	dr.FrontCounterClockwise = false;
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




	#endif
#if defined(OGL)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glFrontFace(GL_CW);
	if (forward)
	{
		RM.m_shaderlightF.Init("ShaderOGL\\LightForward.vs", "ShaderOGL\\LightForward.fs");

	}
	else
	{
		RM.m_shaderLight.Init("ShaderOGL\\Light.vs", "ShaderOGL\\Light.fs");

	}
	#endif
	return S_OK;

}

HRESULT APase::InitSao()
{

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();
#if defined(DX11)
	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/


	m_ListRenderTV.push_back(RM.m_SsaoRT);

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



	#endif
#if defined(OGL)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glFrontFace(GL_CW);
	RM.m_shaderSAO.Init("ShaderOGL\\SAO.vs", "ShaderOGL\\SAO.fs");
	#endif
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

#if defined(DX11)
	

	m_ListRenderTV.push_back(RM.m_ToonRT);
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


	#endif
#if defined(OGL)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glFrontFace(GL_CW);
	
	
		RM.m_shaderToneMap.Init("ShaderOGL\\ToneMap.vs", "ShaderOGL\\ToneMap.fs");
	#endif
	
	return S_OK;
}

HRESULT APase::InitSkybox()
{
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	auto& RM = RManager::SingletonRM();

	/*
	* 1- paso cargar shader
	* 2-create render targets
	*/
#if defined(DX11)


	m_ListRenderTV.push_back(RM.m_LightRT);

	HRESULT hr = S_OK;


	D3D11_RASTERIZER_DESC dr;
	dr.CullMode = D3D11_CULL_NONE;
	dr.FillMode = D3D11_FILL_SOLID;
	dr.FrontCounterClockwise = false;
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

	#endif
#if defined(OGL)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	
	RM.m_shaderSkybox.Init("ShaderOGL\\SkyBox.vs", "ShaderOGL\\SkyBox.fs");
	
	

	
	RM.m_Skybox.transform.scale[0] = 10;
	RM.m_Skybox.transform.scale[1] = 10;
	RM.m_Skybox.transform.scale[2] = 10;
	#endif
	return S_OK;
}




HRESULT APase::CompileShaderFromFile(const char* szFileName,const D3D10_SHADER_MACRO* _Macros, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
#if defined(DX11)
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
	#endif
	return S_OK;

}

	

