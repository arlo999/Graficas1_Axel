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
	
	//ForwardInit();
	LoadDependences();

	DefferedInit();
	return true;
	
}

void ARenderManager::LoadDependences()
{
	m_ScreenAlignedQuad.loadModel("C:\\Graficos1_recursos\\ProyectoGraficas1\\bin\\SAQ\\ScreenAlignedQuad.3ds");
	m_Skybox.loadModel("C:\\Graficos1_recursos\\ProyectoGraficas1\\bin\\Skybox\\Sphere.3ds");
	ApplyEfects();
	
}

HRESULT ARenderManager::ApplyEfects()
{

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	HRESULT hr = S_OK;
	ID3D11Texture2D* _position;
	ID3D11Texture2D* _normals;
	ID3D11Texture2D* _specular;
	ID3D11Texture2D* _albedo;
	ID3D11Texture2D* _Ssao;
	ID3D11Texture2D* _Final;
	ID3D11Texture2D* _Skybox;


	// Create rt texture
	D3D11_TEXTURE2D_DESC descTextRT;
	ZeroMemory(&descTextRT, sizeof(descTextRT));
	descTextRT.Width = (FLOAT)testObj.width;
	descTextRT.Height = (FLOAT)testObj.height;
	descTextRT.MipLevels = 1;
	descTextRT.ArraySize = 1;
	descTextRT.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_Ssao);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_Final);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateTexture2D(&descTextRT, NULL, &_Skybox);
	if (FAILED(hr))
		return hr;
	// create the rt Shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC descViewRT;
	ZeroMemory(&descViewRT, sizeof(descViewRT));
	descViewRT.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_Final, &descViewRT, &m_FinalSRV);
	if (FAILED(hr))
		return hr;
	hr = testObj.g_pd3dDevice.A_CreateShaderResourceView(_Ssao, &descViewRT, &m_SSaoSRV);
	if (FAILED(hr))
		return hr;

	//create the texture cube map
	hr = D3DX11CreateShaderResourceViewFromFile(testObj.g_pd3dDevice.m_device, "Earth.dds", NULL, NULL, &m_SkyboxMapSRV, NULL);
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
	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_Ssao, NULL, &m_SsaoRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_SsaoRT);
	
////////
	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_Final, NULL, &m_FinalRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_FinalRT);
/////
	hr = testObj.g_pd3dDevice.A_CreateRenderTargetView(_Skybox, NULL, &m_SkyboxRT);
	if (FAILED(hr))
		return hr;
	m_RTVList.push_back(m_SkyboxRT);


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


	hr = testObj.g_pd3dDevice.A_CreateSamplerState(&samplerDesc, &m_generico);
	if (FAILED(hr))
		return hr;

	return S_OK;

}



void ARenderManager::Render(std::vector<AModel*>& _ModelList)
{

	
	m_ModelList=_ModelList;
	//Forward();
	Deferred();

}

void ARenderManager::Forward()
{
	
	for (int i = 0; i < m_EfectoList.size(); i++)
	{
		m_EfectoList[i]->Render();
	}

}

void ARenderManager::Deferred()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);

	for (int i = 0; i < m_EfectoList.size(); i++)
	{
		m_EfectoList[i]->Render();
	}

}

bool ARenderManager::ForwardInit()
{
	AEfecto* forward= new AEfecto;
	forward->Init();
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