#include <windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "GraphicsModule.h"

// -----------------Global var-----------------------------------------------------------------
HWND g_hwnd;
bool first = false;
bool perspective = false;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);


/**
 * @brief   
 * @param   #unsigned int: es el width de la pantalla.
 * @param   #unsigned int: es el height de la pantalla .
 */
void OnSize(unsigned int width, unsigned int height) {
#if defined(DX11)
    auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
    if (testObj.g_pRenderTargetView)
    {
        testObj.g_pRenderTargetView->Release();
    }
    if (testObj.g_pDepthStencilView)
    {
        testObj.g_pDepthStencilView->Release();
    }

    testObj.g_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    testObj.g_pImmediateContext->Flush();
    testObj.ReloadBuffer(width, height);
    #endif
}

/**
 * @brief   Message bomb.
 * @param   #HWND: A handle to the window.
 * @param   #UINT: The message.
 * @param   #WPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @param   #LPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @return  #LRESULT: The return value is the result of the message processing and depends on the message sent..
 */
LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
   
   
    // Handle UI inputs
    if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
        return 1;

    // Handle Window inputs
    switch (_msg)
    {
        
    case WM_SIZE:
      
      if (first) {


            RECT rc;
            GetWindowRect(_hwnd, &rc);
            OnSize(rc.right, rc.bottom);
        }
       
        return 0;
    break;

    case WM_SYSCOMMAND:
        if ((_wParam & 0xfff0) == SC_KEYMENU)
        {
            return 0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_KEYDOWN: {
#if defined(DX11)
        auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
		UINT a = LOWORD(_wParam);
		switch (a) {
			//flechas izquierda
		case 37:
			testObj.camera->move(-1, 0, 0);
			break;
			//flechas arriba
		case 38:
            testObj.camera->move(0, 0, 1);
			break;
			//flecha derecha
		case 39:
            testObj.camera->move(1, 0, 0);
			break;
			//flechas abaja
		case 40:
            testObj.camera->move(0, 0, -1);
			break;
		case 83:
            testObj.camera->move(0, -1, 0);
			break;

		case 87:
            testObj.camera->move(0, 1, 0);
			break;
		case 9:
			if (perspective == true)
			{
				testObj.g_Projection = testObj.camera->ViewPerspective(XM_PIDIV4, 640 / (FLOAT)480, 0.01f, 100.0f);
				perspective = false;
			}
	    		else {

                testObj.g_Projection = testObj.camera->ViewOrtographic(640, (FLOAT)480, 0.01f, 100.0f);
				perspective = true;
			}


			break;
		default:
			break;
           } 
#endif
        }
    }
    return ::DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}

/**
 * @brief   Set the style for the main window and init it.
 * @param   #unsigned int: First window width.
 * @param   #unsigned int: First window height.
 * @bug     No know Bugs.
 * @return  #HRESULT: Status code.
 */
HRESULT InitWindow(LONG _width, LONG _height)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = nullptr;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "TutorialWindowClass";
    wcex.hIconSm = nullptr;
    if (!RegisterClassEx(&wcex))
    {
        return E_FAIL;
    }

    // Create window
    RECT rc = { 0, 0, _width, _height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hwnd = CreateWindow("TutorialWindowClass", "Graficos 1", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, NULL, NULL);
    if (!g_hwnd)
    {
        return E_FAIL;
    }
    ShowWindow(g_hwnd, SW_SHOWNORMAL);
    auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
    
    return S_OK;
}

/**
 * @brief   Init the UI.
 * @bug     No know Bugs.
 * @return  #HRESULT: Status code.
 */
 HRESULT InitImgUI()
 {
   // Setup Dear ImGui context
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();

   // Setup Dear ImGui style
   ImGui::StyleColorsDark();

   // Setup Platform/Renderer back ends
   ImGui_ImplWin32_Init(g_hwnd);
#if defined(DX11)
    auto& testObj=GraphicsModule::GetTestObj(g_hwnd);
    ImGui_ImplDX11_Init(testObj.g_pd3dDevice, testObj.g_pImmediateContext);
   #endif
   return S_OK;
 }
 
 void UIRender()
 {
   // Start the Dear ImGui frame
#if defined(DX11)
   ImGui_ImplDX11_NewFrame();
   #endif

   ImGui_ImplWin32_NewFrame();
   ImGui::NewFrame();

   // example window
   if (ImGui::Begin("Another Window", nullptr))
   {
   }
   ImGui::End();

   // render UI
   ImGui::Render();
#if defined(DX11)
   ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
 #endif
 }


 void Render() {
     auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
     testObj.Render();
#if defined(DX11) || defined(OGL)
     UIRender(); 
     testObj.g_pSwapChain->Present(0, 0);
 #endif
 }
 /**
 * @brief   Entry point.
 * @bug     No know Bugs.
 * @return  #int: Status code.
 */
 void Update() {

     auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
     testObj.Update();
 }
int main()
{
    
    // create the window and console
    if (FAILED(InitWindow(1280, 720)))
    {
        DestroyWindow(g_hwnd);
        return 0;
    }
    first = true;
    // create Graphic API interface
   /*if (FAILED(testObj.InitDevice(g_hwnd)))
    {
       
        testObj.CleanupDevice();
        return 0;
    }*/
    
     //create UI
    if (FAILED(InitImgUI()))
    {
      ImGui_ImplDX11_Shutdown();
      ImGui_ImplWin32_Shutdown();
      ImGui::DestroyContext();
      return 0;
    }

    // main loop
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Update();
            Render();
        }
    }
    auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
    // clean resources
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    testObj.CleanupDevice();
    DestroyWindow(g_hwnd);
    return (int)msg.wParam;
}