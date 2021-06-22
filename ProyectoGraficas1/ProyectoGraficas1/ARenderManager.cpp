#include "ARenderManager.h"
#include "GraphicsModule.h"
#include <FreeImage/Dist/x64/FreeImage.h>

namespace RManager {
ARenderManager::ARenderManager()
{

	
}

ARenderManager::~ARenderManager()
{
}

bool ARenderManager::Init()
{
	std::cout<<"Init render manager"<<std::endl;
	
	LoadDependences();
	ForwardInit();
	DefferedInit();
	return true;
	
}

void ARenderManager::LoadDependences()
{
#if defined(DX11)
	m_ScreenAlignedQuad.loadModel("C:\\Graficos1_recursos\\ProyectoGraficas1\\bin\\SAQ\\ScreenAlignedQuad.3ds");
	m_Skybox.loadModel("C:\\Graficos1_recursos\\ProyectoGraficas1\\bin\\Skybox\\Sphere.3ds");
#endif
#if defined(OGL)

	//m_ScreenAlignedQuad.loadModel("C:\\Graficos1_recursos\\ProyectoGraficas1\\bin\\SAQ\\ScreenAlignedQuad.3ds");

	m_ScreenAlignedQuad.loadModel("C:\\Graficos1_recursos\\ProyectoGraficas1\\bin\\SAQ\\SAQ.obj");
	m_Skybox.loadModel("C:\\Graficos1_recursos\\ProyectoGraficas1\\bin\\Skybox\\SkyboxMesh.obj");
	#endif
	
	ApplyEfects();
	
}

HRESULT ARenderManager::ApplyEfects()
{
#if defined(DX11)
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	HRESULT hr = S_OK;
	ID3D11Texture2D* _position;
	ID3D11Texture2D* _normals;
	ID3D11Texture2D* _specular;
	ID3D11Texture2D* _albedo;
	ID3D11Texture2D* _Ssao;
	ID3D11Texture2D* _Light;
	ID3D11Texture2D* _ToonMap;
	ID3D11Texture2D* _Final;
	ID3D11Texture2D* _Skybox;
	ID3D11Texture2D* _Skyboxnormal;


	// Create rt texture
	D3D11_TEXTURE2D_DESC descTextRT;
	ZeroMemory(&descTextRT, sizeof(descTextRT));
	descTextRT.Width = (FLOAT)testObj.width;
	descTextRT.Height = (FLOAT)testObj.height;
	descTextRT.MipLevels = 1;
	descTextRT.ArraySize = 1;
	descTextRT.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	descTextRT.SampleDesc.Count = 1;
	descTextRT.SampleDesc.Quality = 0;
	descTextRT.Usage = D3D11_USAGE_DEFAULT;
	descTextRT.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	descTextRT.CPUAccessFlags = 0;
	descTextRT.MiscFlags = 0;

	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_position);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_normals);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_specular);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_albedo);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_Light);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_Ssao);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_ToonMap);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_Final);
	if (FAILED(hr))
		return hr;
	// create the rt Shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC descViewRT;
	ZeroMemory(&descViewRT, sizeof(descViewRT));
	descViewRT.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	descViewRT.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descViewRT.Texture2D.MostDetailedMip = 0;
	descViewRT.Texture2D.MipLevels = 1;
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_position, &descViewRT, &m_PositionSRV);
	if (FAILED(hr))
		return hr;
///
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_normals, &descViewRT, &m_NormalSRV);
	if (FAILED(hr))
		return hr;
///
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_specular, &descViewRT, &m_SpecularSRV);
	if (FAILED(hr))
		return hr;
///
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_albedo, &descViewRT, &m_AlbedoSRV);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_Ssao, &descViewRT, &m_SSaoSRV);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_Light, &descViewRT, &m_LightSRV);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_ToonMap, &descViewRT, &m_ToonMapSRV);
	if (FAILED(hr))
		return hr;

	//create the texture cube map
	hr = D3DX11CreateTextureFromFile(testObj.g_pd3dDevice.m_device, "Mars.dds", NULL, NULL, (ID3D11Resource**)&_Skybox, NULL);
	if (FAILED(hr))
		return hr;
	hr = D3DX11CreateTextureFromFile(testObj.g_pd3dDevice.m_device, "Diffuse_Mars.dds", NULL, NULL, (ID3D11Resource**)&_Skyboxnormal, NULL);
	if (FAILED(hr))
		return hr;
	D3D11_TEXTURE2D_DESC texture2dEmpty;
	_Skybox->GetDesc(&texture2dEmpty);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC cubedesc;
	ZeroMemory(&cubedesc, sizeof(cubedesc));
	cubedesc.Format = texture2dEmpty.Format;
	cubedesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
	cubedesc.Texture2D.MostDetailedMip = 0;
	cubedesc.Texture2D.MipLevels = texture2dEmpty.MipLevels;


	D3D11_TEXTURE2D_DESC texture2dEmpty2;
	_Skyboxnormal->GetDesc(&texture2dEmpty2);
	D3D11_SHADER_RESOURCE_VIEW_DESC cubedesc2;
	ZeroMemory(&cubedesc2, sizeof(cubedesc2));
	cubedesc2.Format = texture2dEmpty2.Format;
	cubedesc2.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
	cubedesc2.Texture2D.MostDetailedMip = 0;
	cubedesc2.Texture2D.MipLevels = texture2dEmpty2.MipLevels;

	
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_Skybox, &cubedesc, &m_SkyboxMapSRV);
	if (FAILED(hr))
		return hr;


	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_Skyboxnormal, &cubedesc2, &m_SkyboxNormalMapSRV);
	if (FAILED(hr))
		return hr;



	// Create the render target view	
	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_albedo, NULL, &m_AlbedoRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_AlbedoRT);

//////////////
	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_normals, NULL, &m_NormalRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_NormalRT);
//////////////
	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_specular, NULL, &m_SpecularRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_SpecularRT);
//////////
	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_position, NULL, &m_PositionRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_PositionRT);
	
//////
	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_Light, NULL, &m_LightRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_LightRT);

	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_Ssao, NULL, &m_SsaoRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_SsaoRT);

	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_ToonMap, NULL, &m_ToonRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_SsaoRT);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = FLT_MAX;


	hr = testObj.g_pd3dDevice.A_CreateSamplerState(&samplerDesc, &m_generico);
	if (FAILED(hr))
		return hr;

#endif
#if defined(OGL) 

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//- albedo color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAlbedo, 0);


	// - normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// - color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	// - position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gPosition, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);
	// create and attach depth buffer (renderbuffer)

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	//------------------light
	glGenFramebuffers(1, &LightRT);
	glBindFramebuffer(GL_FRAMEBUFFER, LightRT);

	//- albedo color buffer
	glGenTextures(1, &Lightsrv);
	glBindTexture(GL_TEXTURE_2D, Lightsrv);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Lightsrv, 0);

	unsigned int attachmentsLight[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachmentsLight);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//--------------------tonemap


	glGenFramebuffers(1, &TonemapRT);
	glBindFramebuffer(GL_FRAMEBUFFER, TonemapRT);

	
	glGenTextures(1, &toneMapsrv);
	glBindTexture(GL_TEXTURE_2D, toneMapsrv);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, toneMapsrv, 0);

	unsigned int attachmentstone[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachmentstone);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//--------------------ssao

	glGenFramebuffers(1, &SAORT);
	glBindFramebuffer(GL_FRAMEBUFFER, SAORT);


	glGenTextures(1, &SAOsrv);
	glBindTexture(GL_TEXTURE_2D, SAOsrv);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SAOsrv, 0);

	unsigned int attachmentssao[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachmentssao);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

//-------------------------copy

	glGenFramebuffers(1, &CopyRT);
	glBindFramebuffer(GL_FRAMEBUFFER, CopyRT);

	


//-----------------------------------------------------------skybox
	Skyboxsrv=loadCubemap(faces);
	
	
	//-------------------------------------------------forward
	glGenFramebuffers(1, &LightForwardRT);
	glBindFramebuffer(GL_FRAMEBUFFER, LightForwardRT);

	
	glGenTextures(1, &LightForwardsrv);
	glBindTexture(GL_TEXTURE_2D, LightForwardsrv);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, LightForwardsrv, 0);

	unsigned int attachmentsLightForward[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachmentsLightForward);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

//----------------------------------------------------------tonemap

	glGenFramebuffers(1, &ToneMapForwardRT);
	glBindFramebuffer(GL_FRAMEBUFFER, ToneMapForwardRT);

	glGenTextures(1, &ToneMapForwardsrv);
	glBindTexture(GL_TEXTURE_2D, ToneMapForwardsrv);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ToneMapForwardsrv, 0);

	unsigned int attachmentsToneForward[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachmentsToneForward);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

//----------------------------------copy
	glGenFramebuffers(1, &CopyForwardRT);
	glBindFramebuffer(GL_FRAMEBUFFER, CopyForwardRT);



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	#endif
	return S_OK;
}



void ARenderManager::Render(std::vector<AModel*>& _ModelList)
{

	
	m_ModelList=_ModelList;
	if (m_Forward) {

	Forward();
	}
	else {
		Deferred();

	}
	
	

}

void ARenderManager::Forward()
{
	
	
		m_EfectoList[0]->Render();
	

}

void ARenderManager::Deferred()
{
	

	
		m_EfectoList[1]->Render();
	

}

unsigned int ARenderManager::loadCubemap(vector<std::string> faces)
{
	unsigned int textureID=0;
#if defined(OGL)
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{

		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		
		
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
		
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	#endif
	return textureID;
}

bool ARenderManager::ForwardInit()
{
	AEfecto* forward= new AEfecto;
	forward->InitForward();
	m_EfectoList.push_back(forward);

	return true;
}

bool ARenderManager::DefferedInit()
{

	AEfecto* defferd=new AEfecto;
	defferd->Init();
	m_EfectoList.push_back(defferd);

	return true;
}

extern ARenderManager& SingletonRM()
{
	static ARenderManager* m_SingletonRM;
	if (m_SingletonRM == nullptr)
	{
		m_SingletonRM = new ARenderManager();
		m_SingletonRM->Init();
	}
	return *m_SingletonRM;
}

}