//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "resource.h"

//mis includes
#include "ACamera.h"
#include "AVector.h"
#include "AMatriz4.h"
#include <windowsx.h>
#include "Mesh.h"
#include "ADevice.h"
#include "ADeviceContext.h"
#include "ASwapChain.h"

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
};

struct CBNeverChanges
{
    XMMATRIX mView;
};

struct CBChangeOnResize
{
    XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*                       g_pd3dDevice = NULL;
ID3D11DeviceContext*                g_pImmediateContext = NULL;
IDXGISwapChain*                     g_pSwapChain = NULL;
ID3D11RenderTargetView*             g_pRenderTargetView = NULL;
ID3D11Texture2D*                    g_pDepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11VertexShader*                 g_pVertexShader = NULL;
ID3D11PixelShader*                  g_pPixelShader = NULL;
ID3D11InputLayout*                  g_pVertexLayout = NULL;
ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11SamplerState*                 g_pSamplerLinear = NULL;
XMMATRIX                            g_World;
XMMATRIX                            g_cubo2;
XMMATRIX                            g_cubo3;
XMMATRIX                            g_cubo4;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor( 0.7f, 0.7f, 0.7f, 1.0f );


CBChangeOnResize cbChangesOnResize;
CBNeverChanges cbNeverChanges;
UINT offset = 0;
UINT stride;
D3D11_VIEWPORT vp;
bool perspective = false;
ACamera* camera;
//primera forma de mover la camara 'rotacion'
POINT Cursor;
POINT CursorFinal;
int cursorx, cursory;
///////////////////////////

// segunda forma 
LPPOINT p = new POINT;

//MESH INCLUSION
Mesh mesh;
//MY new variables

ADevice device;
ADeviceContext deviceContext;
ASwapChain swapchain;


// creating camera

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();
void Update();

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Update();
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


/*
Function que hace reload de buffer para el resize
*/
HRESULT ReloadBuffer(unsigned int width, unsigned int height) {

    HRESULT hr = S_OK;
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
     hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
     if (FAILED(hr))
     {

		return hr;
    }


	//old function
	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	// hr = device.A_CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		
        {
        return hr;
        }
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	// hr= device.A_CreateTexture2D(&descDepth,NULL,&g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	//  hr= device.A_CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	// Setup the viewport

	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
}




/*
function que te da el resize
*/



void OnSize(unsigned int width, unsigned int height) {

    if(g_pRenderTargetView)
    {
    g_pRenderTargetView->Release();
    }
    if (g_pDepthStencilView)
    {
        g_pDepthStencilView->Release();
    }
    
    g_pSwapChain->ResizeBuffers(1,width,height, DXGI_FORMAT_R8G8B8A8_UNORM,0); 
    g_pImmediateContext->Flush();
    ReloadBuffer(width,height);
}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{


  
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
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
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        
        
        hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        
        /*
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
        D3D11_SDK_VERSION, &sd, &g_pSwapChain, &device.m_device, &g_featureLevel, &g_pImmediateContext);
        */
        
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

   
   //old function
   hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
    // hr = device.A_CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
   
   hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
  // hr= device.A_CreateTexture2D(&descDepth,NULL,&g_pDepthStencil);
   if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
//  hr= device.A_CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
  if( FAILED( hr ) )
        return hr;

  

    // Setup the viewport
   
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
   
  

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial07.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Create the vertex shader
    hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
  // hr= device.A_CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);

   if( FAILED( hr ) )
    {    
        pVSBlob->Release();
        return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
    hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),   pVSBlob->GetBufferSize(), &g_pVertexLayout );
 // hr= device.A_CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
  pVSBlob->Release();
    if( FAILED( hr ) )
        return hr;



    // Compile the pixel shader
    ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Create the pixel shader
    hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
   // hr=device.A_CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;




    // Create vertex buffer
    /*
    AsimpleVertex Avertices[] = {

		{ (-1.0f, 1.0f, -1.0f), (0.0f, 0.0f) },
		{ (1.0f, 1.0f, -1.0f),  (1.0f, 0.0f) },
		{ (1.0f, 1.0f, 1.0f),   (1.0f, 1.0f) },
		{ (-1.0f, 1.0f, 1.0f),  (0.0f, 1.0f) },

		{ (-1.0f, -1.0f, -1.0f),(0.0f, 0.0f) },
		{ (1.0f, -1.0f, -1.0f), (1.0f, 0.0f) },
		{ (1.0f, -1.0f, 1.0f),  (1.0f, 1.0f) },
		{ (-1.0f, -1.0f, 1.0f), (0.0f, 1.0f) },

		{ (-1.0f, -1.0f, 1.0f), (0.0f, 0.0f) },
		{ (-1.0f, -1.0f, -1.0f),(1.0f, 0.0f) },
		{ (-1.0f, 1.0f, -1.0f), (1.0f, 1.0f) },
		{ (-1.0f, 1.0f, 1.0f),  (0.0f, 1.0f) },

		{ (1.0f, -1.0f, 1.0f),  (0.0f, 0.0f) },
		{ (1.0f, -1.0f, -1.0f), (1.0f, 0.0f) },
		{ (1.0f, 1.0f, -1.0f),  (1.0f, 1.0f) },
		{ (1.0f, 1.0f, 1.0f),   (0.0f, 1.0f)  },

		{ (-1.0f, -1.0f, -1.0f),(0.0f, 0.0f) },
		{ (1.0f, -1.0f, -1.0f), (1.0f, 0.0f) },
		{ (1.0f, 1.0f, -1.0f),  (1.0f, 1.0f) },
		{ (-1.0f, 1.0f, -1.0f), (0.0f, 1.0f) },

		{ (-1.0f, -1.0f, 1.0f), (0.0f, 0.0f) },
		{ (1.0f, -1.0f, 1.0f),  (1.0f, 0.0f) },
		{ (1.0f, 1.0f, 1.0f),   (1.0f, 1.0f) },
		{ (-1.0f, 1.0f, 1.0f),  (0.0f, 1.0f) },




    };
   
    */

	AsimpleVertexV2 Avertices[] = {

       { Afloat3{-1.0f, 1.0f, -1.0f}, Afloat2{0.0f, 0.0f} },
       { Afloat3{1.0f, 1.0f, -1.0f},  Afloat2{1.0f, 0.0f} },
       { Afloat3{1.0f, 1.0f, 1.0f},   Afloat2{1.0f, 1.0f} },
       { Afloat3{-1.0f, 1.0f, 1.0f},  Afloat2{0.0f, 1.0f} },
                                                        
       { Afloat3{-1.0f, -1.0f, -1.0f},Afloat2{0.0f, 0.0f} },
       { Afloat3{1.0f, -1.0f, -1.0f}, Afloat2{1.0f, 0.0f} },
       { Afloat3{1.0f, -1.0f, 1.0f},  Afloat2{1.0f, 1.0f} },
       { Afloat3{-1.0f, -1.0f, 1.0f}, Afloat2{0.0f, 1.0f} },
                                                        
       { Afloat3{-1.0f, -1.0f, 1.0f}, Afloat2{0.0f, 0.0f} },
       { Afloat3{-1.0f, -1.0f, -1.0f},Afloat2{1.0f, 0.0f} },
       { Afloat3{-1.0f, 1.0f, -1.0f}, Afloat2{1.0f, 1.0f} },
       { Afloat3{-1.0f, 1.0f, 1.0f},  Afloat2{0.0f, 1.0f} },
                                                        
       { Afloat3{1.0f, -1.0f, 1.0f},  Afloat2{0.0f, 0.0f} },
       { Afloat3{1.0f, -1.0f, -1.0f}, Afloat2{1.0f, 0.0f} },
       { Afloat3{1.0f, 1.0f, -1.0f},  Afloat2{1.0f, 1.0f} },
       { Afloat3{1.0f, 1.0f, 1.0f},   Afloat2{0.0f, 1.0f}  },
                                                        
       { Afloat3{-1.0f, -1.0f, -1.0f},Afloat2{0.0f, 0.0f} },
       { Afloat3{1.0f, -1.0f, -1.0f}, Afloat2{1.0f, 0.0f} },
       { Afloat3{1.0f, 1.0f, -1.0f},  Afloat2{1.0f, 1.0f} },
       { Afloat3{-1.0f, 1.0f, -1.0f}, Afloat2{0.0f, 1.0f} },
                                                        
       { Afloat3{-1.0f, -1.0f, 1.0f}, Afloat2{0.0f, 0.0f} },
       { Afloat3{1.0f, -1.0f, 1.0f},  Afloat2{1.0f, 0.0f} },
       { Afloat3{1.0f, 1.0f, 1.0f},   Afloat2{1.0f, 1.0f} },
       { Afloat3{-1.0f, 1.0f, 1.0f},  Afloat2{0.0f, 1.0f} },
                                                        



	};

    mesh.setMesh2(Avertices, 24);
    
    SimpleVertex vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ),  XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ),   XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ),  XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ),XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ),  XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ),XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ),  XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( 1.0f, -1.0f, 1.0f ),  XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ),  XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ),   XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ),XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ),  XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ),  XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ),   XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ),  XMFLOAT2( 0.0f, 1.0f ) },
    };




    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( AsimpleVertexV2 ) * 24;
    
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = mesh.getMesh2();
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
  //  hr= device.A_CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
   stride = sizeof( AsimpleVertexV2 );
 
	

    // Create index buffer
    // Create vertex buffer
    unsigned short indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };



    mesh.setIndice(indices,36);

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( unsigned short ) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = mesh.getIndice();
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
   // hr= device.A_CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
  if( FAILED( hr ) )
        return hr;
	
    // Create the constant buffers
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBNeverChanges);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBNeverChanges );
  //  hr = device.A_CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
  if( FAILED( hr ) )
        return hr;
    
    bd.ByteWidth = sizeof(CBChangeOnResize);
    hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBChangeOnResize );
  //  hr = device.A_CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
 if( FAILED( hr ) )
        return hr;
    
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBChangesEveryFrame );
  //  hr = device.A_CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
    if( FAILED( hr ) )
        return hr;

    // Load the Texture
    hr = D3DX11CreateShaderResourceViewFromFile( g_pd3dDevice, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL );
  //  hr = D3DX11CreateShaderResourceViewFromFile(device.m_device, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
    if( FAILED( hr ) )
        return hr;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerLinear );
  //  hr = device.A_CreateSamplerState(&sampDesc, &g_pSamplerLinear);
   if( FAILED( hr ) )
        return hr;

    // Initialize the world matrices
    
    g_World = mesh.getMatrixTransFormacion();
    
    g_cubo2 = mesh.getMatrixTransFormacion();
    g_cubo2 = XMMatrixTranslation(0, 5, 0);
   
    g_cubo3 = mesh.getMatrixTransFormacion();
    g_cubo3 = XMMatrixTranslation(5, 0, 0);

    g_cubo4 = mesh.getMatrixTransFormacion();
    g_cubo4 = XMMatrixTranslation(-5, 0, 0);


    // Initialize the view matrix
    camera = new ACamera();
    camera->setEye(AVector(0.0f, 3.0f, -6.0f,0));
    camera->setAt(AVector(0.0f, 1.0f, 0.0f,0));
    camera->setUp(AVector(0.0f, 1.0f, 0.0f,0));
    
    /*
    * direcX Formula
    XMVECTOR Eye = XMVectorSet( 0.0f, 3.0f, -6.0f, 0.0f );
    XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    */
  
   
   
   
   //direcX forma
//    g_View = XMMatrixLookAtLH( Eye, At, Up );
	
    
    camera->setviewMLookL(camera->getEye(), camera->getAt(), camera->getUp());

	g_View = camera->getviewMLookLget();
    

    cbNeverChanges.mView = XMMatrixTranspose( g_View );
  

    g_Projection = camera->ViewPerspective(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);


    
    // Initialize the projection matrix DirectX
    //g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f );
    //g_Projection = XMMatrixOrthographicLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);
    
   

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();

    if( g_pSamplerLinear ) g_pSamplerLinear->Release();
    if( g_pTextureRV ) g_pTextureRV->Release();
    if( g_pCBNeverChanges ) g_pCBNeverChanges->Release();
    if( g_pCBChangeOnResize ) g_pCBChangeOnResize->Release();
    if( g_pCBChangesEveryFrame ) g_pCBChangesEveryFrame->Release();
    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    if( g_pIndexBuffer ) g_pIndexBuffer->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pDepthStencil ) g_pDepthStencil->Release();
    if( g_pDepthStencilView ) g_pDepthStencilView->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
   // if (device.m_device) device.m_device->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;
    //vector translation de la camara
    AVector translationCamara;
    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;
       case WM_SIZE:
           if (g_pSwapChain) {


			   RECT rc;
			   GetWindowRect(hWnd, &rc);
			   OnSize(rc.right, rc.bottom);
       }
        break;
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
		case WM_LBUTTONDOWN:
			/*
            Cursor.x = GET_X_LPARAM(lParam);
            cursorx = Cursor.x;
            Cursor.y = GET_Y_LPARAM(lParam);
            cursory = Cursor.y;
            
            */
            break;
        case WM_LBUTTONUP:
            
            /*
            Cursor.x = GET_X_LPARAM(lParam);
            Cursor.y = GET_Y_LPARAM(lParam);
          
            */
            break;
        case WM_KEYDOWN: {
            UINT a = LOWORD(wParam);
            switch (a) {
                //flechas izquierda
            case 37:
                camera->move(-1, 0, 0);
                break;
                //flechas arriba
            case 38:
                camera->move(0, 0, 1);
                break;
                //flecha derecha
            case 39:
                camera->move(1, 0, 0);
                break;
                //flechas abaja
            case 40:
                camera->move(0, 0, -1);
                break;
            case 83:
                camera->move(0, -1, 0);
                break;

            case 87:
                camera->move(0, 1, 0);
                break;
          
            case 9:
                if (perspective==true)
                {
                    g_Projection = camera->ViewPerspective(XM_PIDIV4, 640 / (FLOAT)480, 0.01f, 100.0f);
                    perspective = false;
                }
                else {
                
                g_Projection = camera->ViewOrtographic(640, (FLOAT)480, 0.01f, 100.0f);
                perspective = true;
                }
                
                
                break;
            
            
            
            
            default:
                break;
            }
         
			

			break;
		}

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }
  
    return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_pImmediateContext->RSSetViewports(1, &vp);
	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	// Set index buffer
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

	g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);




    //
    // Clear the back buffer
    //
    g_View = camera->getviewMLookLget();
    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = XMMatrixTranspose(g_View);
    g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

    //proyeccion


    cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
    g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);



    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    //
    // Update variables that change once per frame
    //


    //primer cubito
    CBChangesEveryFrame cb;
    cb.mWorld = XMMatrixTranspose(g_World);
    cb.vMeshColor = g_vMeshColor;
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);

    g_pImmediateContext->DrawIndexed(36, 0, 0);

    //segundo cubito
    CBChangesEveryFrame cb_cubo2;
    cb_cubo2.mWorld = XMMatrixTranspose(g_cubo2);
    cb_cubo2.vMeshColor = g_vMeshColor;
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb_cubo2, 0, 0);

    g_pImmediateContext->DrawIndexed(36, 0, 0);

    //tercer cubito
    CBChangesEveryFrame cb_cubo3;
    cb_cubo3.mWorld = XMMatrixTranspose(g_cubo3);
    cb_cubo3.vMeshColor = g_vMeshColor;
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb_cubo3, 0, 0);

    g_pImmediateContext->DrawIndexed(36, 0, 0);



    //cuarto cubito
    CBChangesEveryFrame cb_cubo4;
    cb_cubo4.mWorld = XMMatrixTranspose(g_cubo4);
	cb_cubo4.vMeshColor = g_vMeshColor;
	g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb_cubo4, 0, 0);


    //
    // Render the cube
    //
    g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
    g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pCBNeverChanges );
    g_pImmediateContext->VSSetConstantBuffers( 1, 1, &g_pCBChangeOnResize );
    g_pImmediateContext->VSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
    g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
    g_pImmediateContext->PSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
    g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
    g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );
    g_pImmediateContext->DrawIndexed( 36, 0, 0 );

    //
    // Present our back buffer to our front buffer
    //
    g_pSwapChain->Present( 0, 0 );
}

void Update()
{
    
    GetCursorPos(p);
    camera->rotate(p->x, p->y, 0);
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


    g_World = XMMatrixRotationY(t);
   
      // Modify the color
    g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
    g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

}
