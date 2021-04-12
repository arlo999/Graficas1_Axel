
#include <windows.h>
#include <Windowsx.h>
#include "imgui.h"
#include <iostream>
#include <stdio.h>

#if  defined(OGL)
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif
#include "ATextura.h"
#include "AModel.h"
#include "ACamera.h"


/////
#include "GraphicsModule.h"
/////  

#if defined(DX11)
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>  // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/mesh.h>
#include <assimp/cimport.h>
#endif 

// -----------------Global var-----------------------------------------------------------------
HWND g_hwnd;
bool first = false;
bool perspective = false;
POINT Cursor;
int cursorx, cursory;
#if  defined(OGL)
ATextura tex;
bool firstMouse = true;
float lastX = 1270 / 2.0f;
float lastY = 720/ 2.0f;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Model ourModel;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLFWwindow* window;
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#endif
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
#if defined(OGL)
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
    #endif
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
#if defined(OGL)
	camera.ProcessMouseScroll(yoffset);
#endif
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
#if defined(OGL)
	glViewport(0, 0, width, height);
    #endif
}
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

    testObj.g_pSwapChain.m_swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    testObj.g_pImmediateContext.m_devicecontext->Flush();
    testObj.ReloadBuffer(width, height);
    #endif
}

std::string OpenWindowFile(HWND _hwnd)
{
	// common dialog box structure, setting all fields to 0 is important
	OPENFILENAME ofn = { 0 };
	char szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = ("All\0*.*\0Text\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{

		return (std::string)szFile;
	}
	return NULL;
}

void LoadMesh(const std::string& _Filename)
{
#if defined (DX11) 

    auto& testObj = GraphicsModule::GetTestObj(g_hwnd);

    testObj.m_pVertexBuffer.Release();
    testObj.m_pIndexBuffer.Release();
    testObj.arrSimpleVertex.clear();
    testObj.numVertex = 0;
	Assimp::Importer imp;
	const aiScene* pScene = imp.ReadFile(_Filename, aiProcess_Triangulate);
	if (!pScene)
	{
		std::cout << "Error to load assimp file" << _Filename << ": " << std::endl;

	}
	for (std::uint32_t meshIndex = 0u; meshIndex < pScene->mNumMeshes; meshIndex++) {

		aiMesh* mesh = pScene->mMeshes[meshIndex];
        testObj.numVertex += mesh->mNumVertices;

		for (std::uint32_t vertIndex = 0u; vertIndex < mesh->mNumVertices; vertIndex++) {


			aiVector3D vert = mesh->mVertices[vertIndex];
			aiVector3D tex = mesh->mTextureCoords[0][vertIndex];


			aiVector3D norm = mesh->mNormals[vertIndex];

            testObj.m_pos.x = vert.x;
            testObj.m_pos.y = vert.y;
            testObj.m_pos.z = vert.z;

            testObj.m_normal.x = norm.x;
            testObj.m_normal.y = norm.y;
            testObj.m_normal.z = norm.z;

            testObj.m_vertex.x = tex.x;
            testObj.m_vertex.y = tex.y;

            testObj.arrSimpleVertex.push_back(AsimpleVertexV2{ testObj.m_pos,testObj.m_vertex,testObj.m_normal });


		}

	}
	for (int i = 0; i < testObj.numVertex; i++)
	{
        testObj.m_indices.push_back(i);
	}
	// Create vertex buffer

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AsimpleVertexV2) * testObj.numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = testObj.arrSimpleVertex.data();
    testObj.g_pd3dDevice.A_CreateBuffer(&bd, &InitData, &testObj.m_pVertexBuffer.getBufferDX11());
	//need valid




	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * testObj.numVertex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = testObj.m_indices.data();
    testObj.g_pd3dDevice.A_CreateBuffer(&bd, &InitData, &testObj.m_pIndexBuffer.getBufferDX11());

#endif // 
#if defined(OGL)
	 ourModel.loadModel(_Filename);

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
    case WM_LBUTTONUP: {
    /*
        auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
        
        Cursor.x = GET_X_LPARAM(_wParam);
        Cursor.y = GET_Y_LPARAM(_wParam);
        #if defined(DX11)
       testObj.camera->rotate(cursorx, cursory, 0);
    */

    }
        
        
        break;
    case WM_LBUTTONDOWN: {
        auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
		/*
        Cursor.x = GET_X_LPARAM(_wParam);
		cursorx = Cursor.x;
		Cursor.y = GET_Y_LPARAM(_wParam);
		cursory = Cursor.y;
        */
		

    }
        
        
        
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
#if  defined(OGL)

	if (!glfwInit())
		return 1;
	window = glfwCreateWindow(_width, _height, "Graficas 1 AXEL", NULL, NULL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_NORMAL);
	// Initialize OpenGL loader glad
	if (!gladLoadGL()) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	glViewport(0, 0, _width,_height);
    auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
    testObj.InitDeviceOGL(g_hwnd);
    
#endif
#if  defined(DX11)
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
#endif
    return S_OK;
}

/**
 * @brief   Init the UI.
 * @bug     No know Bugs.
 * @return  #HRESULT: Status code.
 */
 HRESULT InitImgUI()
 {
#if  defined(OGL)
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  
   #endif
   // Setup Platform/Renderer back ends
	// Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
#if  defined(DX11)
   ImGui_ImplWin32_Init(g_hwnd);
#endif
#if  defined(OGL)  
   ImGui_ImplGlfw_InitForOpenGL(window,true);
   ImGui_ImplOpenGL3_Init(glsl_version);
#endif  

#if defined(DX11)
    auto& testObj=GraphicsModule::GetTestObj(g_hwnd);
    ImGui_ImplDX11_Init(testObj.g_pd3dDevice.m_device, testObj.g_pImmediateContext.m_devicecontext);
   #endif
   return S_OK;
 }
 //UI Render 
 void UIRender()
 {
   // Start the Dear ImGui frame
#if defined(DX11)
   ImGui_ImplDX11_NewFrame();
   
   #endif
#if  defined(OGL) 

   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   #endif
#if defined(DX11)
   ImGui_ImplWin32_NewFrame();
   #endif
   ImGui::NewFrame();



   // example window
   if (ImGui::Begin("Another Window", nullptr))
   {
       
   static float dir[3]{};
   if (ImGui::DragFloat3("Direccion de luz", dir,0.001,-1,1)){
       auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
#if defined(DX11)
            testObj.m_LigthBufferStruct.dir= XMFLOAT4(dir[0],dir[1],dir[2],0.0f);
#endif
        }
   if (ImGui::Button("open file")) {

      
        LoadMesh(OpenWindowFile(g_hwnd));
       
        }

#if defined(DX11)
   auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
   float my_tex_w = 256;
   float my_tex_h = 256;
   ImTextureID my_tex_id = testObj.g_pTextureRV;
   ImVec2 pos = ImGui::GetCursorScreenPos();
   ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
   ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
   ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
   ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
   ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
#endif
   
   ImGui::End();
   }

 
   // render UI
   ImGui::Render();
  

  
#if defined(DX11)
   ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

 #endif
 }


 void Render(AShader& _shader) {
#if  defined(OGL)
     auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	 float currentFrame = glfwGetTime();
	 deltaTime = currentFrame - lastFrame;
	 lastFrame = currentFrame;

    // render
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader.Use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1270.0f / 720.0f, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	_shader.setMat4("projection", projection);
	_shader.setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	_shader.setMat4("model", model);

	ourModel.Draw(_shader); 
    
     UIRender();
	 ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
     glfwSwapBuffers(window);
     glfwPollEvents();
#endif
#if defined(DX11) 
	 auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	 testObj.Render();
    UIRender(); 
     testObj.g_pSwapChain.m_swapchain->Present(0, 0);
 #endif
 }

 //inputs Opengl
 void processInput(GLFWwindow* window)
 {
#if defined(OGL)
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		 glfwSetWindowShouldClose(window, true);

	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 camera.ProcessKeyboard(FORWARD, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 camera.ProcessKeyboard(BACKWARD, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 camera.ProcessKeyboard(LEFT, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 camera.ProcessKeyboard(RIGHT, deltaTime);
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
   //  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
	AShader ourShader("C://Graficos1_recursos//ProyectoGraficas1//bin//1.model_loading.vs", "C://Graficos1_recursos//ProyectoGraficas1//bin//1model_loading.fs");

     //create UI
    if (FAILED(InitImgUI()))
    {
#if  defined(OGL)
	  ImGui_ImplOpenGL3_Shutdown();
	  ImGui_ImplGlfw_Shutdown();
#endif
#if  defined(DX11)
      ImGui_ImplDX11_Shutdown();
      ImGui_ImplWin32_Shutdown();
#endif
      ImGui::DestroyContext();
      return 0;
    }
#if defined(OGL)
    while (!glfwWindowShouldClose(window))
	{
        processInput(window);
		Update();
        Render(ourShader);

	}
    #endif
#if  defined(DX11)
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
            Render(ourShader);
        }
    }
#endif    
    auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
    // clean resources
#if  defined(OGL)
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
#endif



#if  defined(DX11)
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
#endif
    ImGui::DestroyContext();
#if  defined(OGL)
    glfwTerminate();
    #endif
    testObj.CleanupDevice();
    DestroyWindow(g_hwnd);

   // return (int)msg.wParam; 
    return 0;
}