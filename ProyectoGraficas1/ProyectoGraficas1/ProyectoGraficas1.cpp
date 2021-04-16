
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
#include "AModel.h"
#include "ACamera.h"
#include "GraphicsModule.h"

#if defined(DX11)
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#endif 

// -----------------Global var-----------------------------------------------------------------
HWND g_hwnd;
bool first = false;
bool perspective = false;
POINT Cursor;
int cursorx, cursory;
bool firstMouse = true;
float lastX = 1270 / 2.0f;
float lastY = 720 / 2.0f;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
AModel* ourModel;
//Camera camera(AVector(0.0f, 0.0f, 3.0f));


bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.0f, 0.125f, 0.3f, 1.0f);
vector<AModel*> models;
#if defined(OGL)
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLFWwindow* window;
#endif



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

#if defined(OGL)
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
    #endif

#if defined(OGL)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
#endif

#if defined(OGL)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
    #endif
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

void LoadMesh(const std::string& _Filename, unsigned int type)
{
    if (type == 1) {

    ourModel=new AModel;
    ourModel->bgr=true;
	 ourModel->loadModel(_Filename);
     models.push_back(ourModel);
    }else if(type ==2){
		ourModel = new AModel;
		ourModel->rgb = true;
		ourModel->loadModel(_Filename);
		models.push_back(ourModel);
    }else if(type==3){
    
		ourModel = new AModel;
		ourModel->wire = true;
		ourModel->loadModel(_Filename);
		models.push_back(ourModel);
    }else if(type == 4){
    
		ourModel = new AModel;
		ourModel->point = true;
		ourModel->loadModel(_Filename);
		models.push_back(ourModel);
    }else{
    
		ourModel = new AModel;
		ourModel->rgb = true;
		ourModel->loadModel(_Filename);
		models.push_back(ourModel);
    }
     
    
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
    
        auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
        
        Cursor.x = GET_X_LPARAM(_wParam);
        Cursor.y = GET_Y_LPARAM(_wParam);
#if defined(DX11)
       testObj.camera->rotate(cursorx, cursory, 0);
#endif 

    }
        
        
        break;
    case WM_LBUTTONDOWN: {
        auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
		
        Cursor.x = GET_X_LPARAM(_wParam);
		cursorx = Cursor.x;
		Cursor.y = GET_Y_LPARAM(_wParam);
		cursory = Cursor.y;
        
		

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

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
#if  defined(DX11)
   ImGui_ImplWin32_Init(g_hwnd);
#endif
#if  defined(OGL)  
  ImGuiIO& io = ImGui::GetIO();
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
 void UIRender(AShader& _shader)
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
    ImGui::Begin("Luz", NULL,0);
   static float dir[3]{};
   if (ImGui::DragFloat3("Direccion de luz", dir,0.001,-1,1)){
       auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
#if defined(DX11)
            testObj.m_LigthBufferStruct.dir= XMFLOAT4(dir[0],dir[1],dir[2],0.0f);
#endif
#if defined(OGL)
           glUniform4f(glGetUniformLocation(_shader.ID,"adirLight"), dir[0], dir[1], dir[2], 0.0f);
#endif
        }
   if (ImGui::BeginCombo("Carga de Modelo", NULL))
   {
	   if (ImGui::Button("Tipo BGR")) {


		   LoadMesh(OpenWindowFile(g_hwnd),1);

	   }
       if (ImGui::Button("Tipo RGB")) {


		   LoadMesh(OpenWindowFile(g_hwnd), 2);

	   }
	   if (ImGui::Button("Tipo wire")) {


		   LoadMesh(OpenWindowFile(g_hwnd), 3);

	   }
	   if (ImGui::Button("Tipo Points")) {


		   LoadMesh(OpenWindowFile(g_hwnd), 4);

	   }
        ImGui::EndCombo();
   }
  
        
        for(int i= 0;i < models.size();i++){
            ImGui::PushID(i);
            if(ImGui::CollapsingHeader("info"+i, NULL)){

				if (ImGui::DragFloat3("Scale", models[i]->transform.scale, 0.001, -10, 10)) {}
				if (ImGui::DragFloat3("Translation", models[i]->transform.traslation, 0.001, -10, 10)) {}
				if (ImGui::DragFloat3("Rotation", models[i]->transform.rotation, 0.001, -10, 10)) {}
				float my_tex_w = 256;
				float my_tex_h = 256;
                
#if defined(DX11) 
				ImTextureID my_tex_id = models[i]->g_pTextureRV;
                #endif
#if defined(OGL) 
                ImTextureID my_tex_id = (void*)models[i]->textures_loaded[0].id;
#endif             
                ImVec2 pos = ImGui::GetCursorScreenPos();
				ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
				ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
				ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
                
            }
            ImGui::PopID();
           
            
        }
#if defined(DX11)
  
  
   
   
#endif
   // ImGui::ShowDemoWindow();  
   
   ImGui::End();

 
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
	
    /*
	_shader.setFloat4("projection", camera.ViewPerspective(glm::radians(camera.Zoom), 1270.0f / 720.0f, 0.1f, 100.0f));
	_shader.setFloat4("view", camera.GetViewMatrix());
    */
    
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1270.0f / 720.0f, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrixGlm();
    _shader.setMat4("projection",projection);
    _shader.setMat4("view",view);

	

   
   // ourModel.Draw(_shader); 
	for (int i = 0; i < models.size(); i++)
	{
        models[i]->Draw(_shader);
		
	}
     UIRender(_shader);
	 ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
     glfwSwapBuffers(window);
     glfwPollEvents();
#endif
#if defined(DX11) 
	 auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	 testObj.Render();
    
     for (int i =0;i<models.size();i++)
     {
         models[i]->Draw(_shader);
     }
     UIRender(_shader); 
     testObj.g_pSwapChain.m_swapchain->Present(0, 0);
 #endif
 }


 //inputs Opengl
#if defined(OGL)
 void processInput(GLFWwindow* window)
 {
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
 }
#endif

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

   //return (int)msg.wParam; 
    return 0;
}