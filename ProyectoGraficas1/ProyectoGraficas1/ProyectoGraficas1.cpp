
#include <windows.h>
#include <Windowsx.h>
#include "imgui.h"
#include <iostream>
#include <stdio.h>
#include "ARenderManager.h"
#if  defined(OGL)
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif
#include "AModel.h"

#include "GraphicsModule.h"

#if defined(DX11)
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#endif 

// -----------------Global var-----------------------//
HWND g_hwnd;
bool first = false;
bool perspective = false;

AModel* ourModel;
vector<AModel*> models;


//---------------Mosuse variables------------------//
POINT Cursor;
int cursorx, cursory;
bool firstMouse = true;
float lastX = 1270 / 2.0f;
float lastY = 720 / 2.0f;
//-------------Screen variables-------------------//
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.0f, 0.125f, 0.3f, 1.0f);

//-------------------------- timing------------------//

float deltaTime = 0.0f;
float lastFrame = 0.0f;


#if defined(OGL)
GLFWwindow* window;
#endif



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
//------Mouse Function Opgl--------------------------//
#if defined(OGL)
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
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

	testObj.camera->ProcessMouseMovement(xoffset, yoffset);
}
    #endif
//--------Mouse Function Opngl----------------------//
#if defined(OGL)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);

	testObj.camera->ProcessMouseScroll(yoffset);
}
#endif
//----------------------Screen function frame buffer----------------//
#if defined(OGL)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
    #endif

//-------------------resize function ---------------------------//
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

//----------------------Open file to create model------------// 
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
//--------------------------load model---------------------//
void LoadModel(const std::string& _Filename, unsigned int type)
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
	}
	else if (type == 5) {

		ourModel = new AModel;
		ourModel->rgb = true;
		ourModel->loadModelSkeleton(_Filename);
		models.push_back(ourModel);
	}
	else{
    
		ourModel = new AModel;
		ourModel->rgb = true;
		ourModel->loadModel(_Filename);
		models.push_back(ourModel);
    }
     
    
}
//-------------------------windows proc-------------------//
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
				testObj.g_Projection = testObj.camera->ViewPerspective(XM_PIDIV4, 1264 / (FLOAT)681, 0.01f, 1000.0f);
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
//-----------------------Function to init window---------------//
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
//-----------------------Function to init Imgui---------------//
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
 
 //-------------------Update Data function-------------------//
 /**
 * @brief   Entry point.
 * @bug     No know Bugs.
 * @return  #int: Status code.
 */
 void Update() {

     auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
     testObj.Update();
     // camera thing add 
 }

 //--------------------render ui----------------------------//
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

   auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
   auto& RM = RManager::SingletonRM();
  // ImGui::ShowDemoWindow();
   ImGui::Begin("Modelo", NULL, 0);
   if (ImGui::BeginCombo("Carga de Modelo", NULL))
   {
	   if (ImGui::Button("Tipo BGR")) {


           LoadModel(OpenWindowFile(g_hwnd),1);

	   }
       if (ImGui::Button("Tipo RGB")) {


           LoadModel(OpenWindowFile(g_hwnd), 2);

	   }
	   if (ImGui::Button("Tipo wire")) {


           LoadModel(OpenWindowFile(g_hwnd), 3);

	   }
	   if (ImGui::Button("Tipo Points")) {


           LoadModel(OpenWindowFile(g_hwnd), 4);

	   }
	   if (ImGui::Button("Skeleto")) {


		   LoadModel(OpenWindowFile(g_hwnd), 5);

	   }
        ImGui::EndCombo();
   }
        for(int i= 0;i < models.size();i++){
            ImGui::PushID(i);
            if(ImGui::CollapsingHeader("Mesh Info", NULL)){

				if (ImGui::DragFloat3("Scale", models[i]->transform.scale, 0.001, -10, 10)) {}
				if (ImGui::DragFloat3("Translation", models[i]->transform.traslation, 0.001, -10, 10)) {}
				if (ImGui::DragFloat3("Rotation", models[i]->transform.rotation, 0.001, -10, 10)) {}
				float my_tex_w = 256;
				float my_tex_h = 256;
                
#if defined(DX11) 
				ImTextureID my_tex_id = (void*)RM.m_AlbedoSRV;
				ImTextureID my_tex_id2 = (void*)RM.m_NormalSRV;
				ImTextureID my_tex_id3 = (void*)RM.m_SpecularSRV;
				ImTextureID my_tex_id4 = (void*)RM.m_PositionSRV;
				ImTextureID my_tex_id5 = (void*)RM.m_SkyboxMapSRV;

                #endif
#if defined(OGL) 
                ImTextureID my_tex_id = (void*)RM.gAlbedo;
				ImTextureID my_tex_id2 = (void*)RM.gNormal;
				ImTextureID my_tex_id3 = (void*)RM.gAlbedoSpec;
				ImTextureID my_tex_id4 = (void*)RM.gPosition;
				ImTextureID my_tex_id5 = (void*)RM.SAOsrv;

#endif             
                ImVec2 pos = ImGui::GetCursorScreenPos();
				ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
				ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
				
				
				ImGui::ImageButton(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				ImGui::ImageButton(my_tex_id2, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				ImGui::ImageButton(my_tex_id3, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				ImGui::ImageButton(my_tex_id4, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				ImGui::ImageButton(my_tex_id5, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
#if defined(DX11)
				ImGui::ImageButton(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				ImGui::ImageButton(my_tex_id2, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				ImGui::ImageButton(my_tex_id3, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				ImGui::ImageButton(my_tex_id4, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				ImGui::ImageButton(my_tex_id5, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max);
				
#endif


                
            }
            ImGui::PopID();
           
            
        }
#if defined(DX11)
  
  
   
   
#endif
   // ImGui::ShowDemoWindow();  
		if (ImGui::BeginCombo("Forward", NULL)) {


			if (ImGui::RadioButton("Forward", RM.m_Forward == true)) {
				RM.m_Forward =true;
			} ImGui::SameLine();
			if (ImGui::RadioButton("Deffered", RM.m_Forward == false)) {
				RM.m_Forward = false;
			} ImGui::SameLine();
			ImGui::EndCombo();
		}

        if (ImGui::BeginCombo("Vertex", NULL)) {

       
		if (ImGui::RadioButton("Vertex", RM.m_typeTecnicaRender == RManager::TypeTecnicas::VERTEX)) {
			RM.m_typeTecnicaRender = RManager::TypeTecnicas::VERTEX;
		} ImGui::SameLine();
		if (ImGui::RadioButton("Vertex Blinn", RM.m_typeTecnicaRender == RManager::TypeTecnicas::VERTEX_AND_BLINN)) {
			RM.m_typeTecnicaRender = RManager::TypeTecnicas::VERTEX_AND_BLINN;
		} ImGui::SameLine();
		if (ImGui::RadioButton("Vertex Blinn Phong", RM.m_typeTecnicaRender == RManager::TypeTecnicas::VERTEX_AND_PHONG)) {
			RM.m_typeTecnicaRender = RManager::TypeTecnicas::VERTEX_AND_PHONG;
		} ImGui::SameLine();

            ImGui::EndCombo();
        }

		if (ImGui::BeginCombo("Pixel", NULL)) {


			if (ImGui::RadioButton("Pixel", RM.m_typeTecnicaRender == RManager::TypeTecnicas::PIXEL)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::PIXEL;
			} ImGui::SameLine();
			if (ImGui::RadioButton("Pixel Blinn", RM.m_typeTecnicaRender == RManager::TypeTecnicas::PIXEL_AND_BLINN)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::PIXEL_AND_BLINN;
			} ImGui::SameLine();
			if (ImGui::RadioButton("Pixel Blinn Phong", RM.m_typeTecnicaRender == RManager::TypeTecnicas::PIXEL_AND_PHONG)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::PIXEL_AND_PHONG;
			} ImGui::SameLine();

			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("PixelMap", NULL)) {


			if (ImGui::RadioButton("PixelMap", RM.m_typeTecnicaRender == RManager::TypeTecnicas::NORMALMAP)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::NORMALMAP;
			} ImGui::SameLine();
			if (ImGui::RadioButton("PixelMap Blinn", RM.m_typeTecnicaRender == RManager::TypeTecnicas::NORMALMAP_AND_BLINN)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::NORMALMAP_AND_BLINN;
			} ImGui::SameLine();
			if (ImGui::RadioButton("PixelMap Blinn Phong", RM.m_typeTecnicaRender == RManager::TypeTecnicas::NORMALMAP_AND_PHONG)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::NORMALMAP_AND_PHONG;
			} ImGui::SameLine();

			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Specular", NULL)) {


			if (ImGui::RadioButton("Specular and Pixel", RM.m_typeTecnicaRender == RManager::TypeTecnicas::NORMAL_SPECULAR)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::NORMAL_SPECULAR;
			} ImGui::SameLine();
			if (ImGui::RadioButton("Specular and Pixel Blinn", RM.m_typeTecnicaRender == RManager::TypeTecnicas::NORMAL_SPECULAR_AND_BLINN)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::NORMAL_SPECULAR_AND_BLINN;
			} ImGui::SameLine();
			if (ImGui::RadioButton("Specular and Pixel Blinn Phong", RM.m_typeTecnicaRender == RManager::TypeTecnicas::NORMAL_SPECULAR_AND_PHONG)) {
				RM.m_typeTecnicaRender = RManager::TypeTecnicas::NORMAL_SPECULAR_AND_PHONG;
			} ImGui::SameLine();

			ImGui::EndCombo();
		}
   ImGui::End();
   //dir

//------------------------------------------------------------------------------------

   ImGui::Begin("Luz Dir", NULL, 0);
   static float dir[3]{};
   static float color[3]{};
   if (ImGui::DragFloat3("Direccion de luz", dir, 0.001, -1, 1)) {

	   

#if defined(DX11)
	   testObj.m_LigthBufferStruct.dir = XMFLOAT4(dir[0], dir[1], dir[2], 0.0f);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat4("dirLight",dir[0],dir[1],dir[2],0.0f);

	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat4("dirLight", dir[0], dir[1], dir[2], 0.0f);
#endif
   }
   if (ImGui::DragFloat3("RGB", color, 0.001, -1, 1)) {
	  
#if defined(DX11)
	   testObj.m_LigthBufferStruct.lightDirColor = XMFLOAT4(color[0], color[1], color[2], 0.0f);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat4("dirLightColor", color[0],color[1],color[2],0.0f);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat4("dirLightColor", color[0], color[1], color[2], 0.0f);
#endif
   }
   ImGui::End();


//------------------------------------------------------------------------------------------point light


   /////////////////////////////////////
//----------------------------------------Point lIGHT
   ImGui::Begin("PointLight", NULL, 0);
   static float pointLightColor[3]{};
   static float pointLightPos[3]{};
   static float pointLightAtt;
   if (ImGui::DragFloat3("Direccion de PointLight", pointLightPos, 1)) {
	  
#if defined(DX11)
	   testObj.m_PointLightBufferStruct.pointLightPos = XMFLOAT3(pointLightPos[0], pointLightPos[1], pointLightPos[2]);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat4("pointLightPos", pointLightPos[0], pointLightPos[1], pointLightPos[2], 0.0f);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat4("pointLightPos", pointLightPos[0], pointLightPos[1], pointLightPos[2], 0.0f);

#endif
   }
   if (ImGui::DragFloat3("Color Light", pointLightColor, 0.001, -1, 1)) {
	  
#if defined(DX11)
	   testObj.m_PointLightBufferStruct.pointLightColor = XMFLOAT4(pointLightColor[0], pointLightColor[1], pointLightColor[2], 0.0f);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat4("pointLightColor", pointLightColor[0], pointLightColor[1], pointLightColor[2], 0.0f);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat4("pointLightColor", pointLightColor[0], pointLightColor[1], pointLightColor[2], 0.0f);
#endif
   }
   if (ImGui::DragFloat("pointLightAtt", &pointLightAtt, 0.001, 1, 1000)) {
	  
#if defined(DX11)
	   testObj.m_PointLightBufferStruct.pointLightAtt = FLOAT(pointLightAtt);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat("pointLightAtt", pointLightAtt);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat("pointLightAtt", pointLightAtt);
#endif
   }
   ImGui::End();




   ///------------------------------------------------------------------------------------ SpotLight
   ImGui::Begin("SpotLight", NULL, 0);
   static float spotLightColor[3]{};
   static float spotLightPos[3]{};
   static float spotLightDir[3]{};
   static float SpotlightAtt;
   static float spotLightInner;
   static float spotLightOutner;
   if (ImGui::DragFloat3("Position de PointLight", spotLightPos, 1)) {
	   
#if defined(DX11)
	   testObj.m_SpotLightBufferStruct.spotLightPos = XMFLOAT4(spotLightPos[0], spotLightPos[1], spotLightPos[2], 0.0f);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat4("spotLightPos", spotLightPos[0], spotLightPos[1], spotLightPos[2], 0.0f);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat4("spotLightPos", spotLightPos[0], spotLightPos[1], spotLightPos[2], 0.0f);

#endif
   }
   if (ImGui::DragFloat3("Color Light", spotLightColor, 0.001, -1, 1)) {
	  
#if defined(DX11)
	   testObj.m_SpotLightBufferStruct.spotLightColor = XMFLOAT4(spotLightColor[0], spotLightColor[1], spotLightColor[2], 0.0f);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat4("spotLightColor", spotLightColor[0], spotLightColor[1], spotLightColor[2], 0.0f);

	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat4("spotLightColor", spotLightColor[0], spotLightColor[1], spotLightColor[2], 0.0f);
#endif
   }
   if (ImGui::DragFloat3("Dir Light", spotLightDir, 0.001, -1, 1)) {
	   
#if defined(DX11)
	   testObj.m_SpotLightBufferStruct.spotLightDir = XMFLOAT4(spotLightDir[0], spotLightDir[1], spotLightDir[2], 0.0f);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat4("spotLightDir", spotLightDir[0], spotLightDir[1], spotLightDir[2], 0.0f);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat4("spotLightDir", spotLightDir[0], spotLightDir[1], spotLightDir[2], 0.0f);

#endif
   }
   if (ImGui::DragFloat("spotLightAtt", &SpotlightAtt, 1)) {
	  
#if defined(DX11)
	   testObj.m_SpotLightBufferStruct.SpotlightAtt = FLOAT(SpotlightAtt);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat("spotLightAtt", SpotlightAtt);

	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat("spotLightAtt", SpotlightAtt);
#endif
   }
   if (ImGui::DragFloat("spotLightInner", &spotLightInner, 1)) {
	   
#if defined(DX11)
	   testObj.m_SpotLightBufferStruct.spotLightInner = FLOAT(spotLightInner);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat("spotLightInner", spotLightInner);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat("spotLightInner", spotLightInner);
#endif
   }
   if (ImGui::DragFloat("spotLightOuter", &spotLightOutner, 1)) {
	  
#if defined(DX11)
	   testObj.m_SpotLightBufferStruct.spotLightOutner = FLOAT(spotLightOutner);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat("spotLightOuter", spotLightOutner);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat("spotLightOuter", spotLightOutner);
#endif
   }
   ImGui::End();

///-------------------------------------------------------------------------------------------------amnbient

   // render UI
   ImGui::Begin("Ambient", NULL, 0);
   static float ambientColor[3]{};
   static  float kAmbient;
   static float kSpecular;
   static float shininess;
   static float kDiffuse;
   if (ImGui::DragFloat3("Ambient Color", ambientColor, 1)) {
	  
#if defined(DX11)
	   testObj.m_AmbientBufferStruct.ambientColor = XMFLOAT4(ambientColor[0], ambientColor[1], ambientColor[2], 0.0f);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	 RM.m_shaderLight.setFloat4("ambientColor",ambientColor[0], ambientColor[1], ambientColor[2],0.0f);
	 RM.m_shaderlightF.Use();
	 RM.m_shaderlightF.setFloat4("ambientColor", ambientColor[0], ambientColor[1], ambientColor[2], 0.0f);
#endif
   }
   if (ImGui::DragFloat("kAmbient", &kAmbient, 1)) {
	   
#if defined(DX11)
	   testObj.m_AmbientBufferStruct.kAmbient = FLOAT(kAmbient);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat("kAmbient", kAmbient);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat("kAmbient", kAmbient);
#endif
   }
   if (ImGui::DragFloat("kSpecular", &kSpecular, 1)) {
	  
#if defined(DX11)
	   testObj.m_SpecularBufferStruct.kSpecular = FLOAT(kSpecular);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat("kSpecular", kSpecular);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat("kSpecular", kSpecular);
#endif
   }
   if (ImGui::DragFloat("shininess", &shininess, 1)) {
	
#if defined(DX11)
	   testObj.m_ShiniesBufferStruct.shininess = FLOAT(shininess);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   RM.m_shaderLight.setFloat("shininess", shininess);
	   RM.m_shaderlightF.Use();
	   RM.m_shaderlightF.setFloat("shininess", shininess);
#endif
   }
   if (ImGui::DragFloat("kDiffuse", &kDiffuse, 1)) {
	  
#if defined(DX11)
	   testObj.m_DiffuseBufferStruct.kDiffuse = FLOAT(kDiffuse);
#endif
#if defined(OGL)
	   RM.m_shaderLight.Use();
	   float i = glGetUniformLocation(RM.m_shaderLight.ID, "kDiffuse");
	   glUniform1f(i, kDiffuse);
	   RM.m_shaderlightF.Use();
	   float i2 = glGetUniformLocation(RM.m_shaderlightF.ID, "kDiffuse");
	   glUniform1f(i2, kDiffuse);
	 //  RM.m_shaderLight.setFloat("kDiffuse", kDiffuse);
#endif
   }
   ImGui::End();


   /// ////////////////////////////////////
   /*
   * SSAo
   */
   //----------------------------------------------------------------
   ImGui::Begin("SSAO", NULL, 0);
   static float sampleIterations[3]{};
   static float sampleRadius;
   static float scale;
   static float bias;
   static float intensity;
   static float exposure[3]{};
   if (ImGui::DragFloat("Sample Iteration", sampleIterations, 0.001, 0, 5)) {
	   RM.m_shaderSAO.Use();
#if defined(DX11)
	   testObj.m_SaoBufferStruct.sampleIterations = XMFLOAT4(sampleIterations[0], 0,0,0);
#endif
#if defined(OGL)
	   RM.m_shaderSAO.setFloat("sampleIterations", sampleIterations[0]);
		
#endif
   }
   if (ImGui::DragFloat("exposure", exposure, 0.001, 0, 5)) {
	   RM.m_shaderToneMap.Use();
#if defined(DX11)
	   testObj.m_TooneMaBufferStruct.exposur = FLOAT(exposure[0]);
#endif
#if defined(OGL)
	   RM.m_shaderToneMap.setFloat("exposure",exposure[0]);

#endif
   }
   if (ImGui::DragFloat("sampleRadius", &sampleRadius, 0.001, 0, 5)) {
	   RM.m_shaderSAO.Use();
#if defined(DX11)
	   testObj.m_SaoBufferStruct.sampleRadius = FLOAT(sampleRadius);
#endif
#if defined(OGL)
	   RM.m_shaderSAO.setFloat("sampleRadius", sampleRadius);
#endif
   }
   if (ImGui::DragFloat("scale", &scale, 0.001, 0, 5)) {
	   RM.m_shaderSAO.Use();
#if defined(DX11)
	   testObj.m_SaoBufferStruct.scale = FLOAT(scale);
#endif
#if defined(OGL)
	   RM.m_shaderSAO.setFloat("scale", scale);
#endif
   }
   if (ImGui::DragFloat("bias", &bias, 0.001, 0, 5)) {
	   RM.m_shaderSAO.Use();
#if defined(DX11)
	   testObj.m_SaoBufferStruct.bias = FLOAT(bias);
#endif
#if defined(OGL)
	   RM.m_shaderSAO.setFloat("bias", bias);
#endif
   }
   if (ImGui::DragFloat("intensity", &intensity, 0.001, 0, 5)) {
	   RM.m_shaderSAO.Use();
#if defined(DX11)
	   testObj.m_SaoBufferStruct.intensity = FLOAT(intensity);
#endif
#if defined(OGL)
	   RM.m_shaderSAO.setFloat("intensity", intensity);
#endif
   }
   ImGui::End();

   ImGui::Render();
  
  
#if defined(DX11)
   ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

 #endif
#if defined(OGL)

   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
 }

//--------------------render function ----------------------//
 void Render() {
#if  defined(OGL)
     auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	 auto& RM = RManager::SingletonRM();
	 float currentFrame = glfwGetTime();
	 deltaTime = currentFrame - lastFrame;
	 lastFrame = currentFrame;

	
	 RM.Render(models);
	
     UIRender();
	

	 glfwSwapBuffers(window);
     glfwPollEvents();
#endif
#if defined(DX11) 
	 auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	 auto& RM = RManager::SingletonRM();
     testObj.Render();
    //render manager
  
    RM.Render(models);
	
   
     UIRender();
	
     testObj.g_pSwapChain.m_swapchain->Present(0, 0);
 #endif
 }


 //inputs Opengl
#if defined(OGL)
 void processInput(GLFWwindow* window)
 {
	 auto& testObj = GraphicsModule::GetTestObj(g_hwnd);


	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		 glfwSetWindowShouldClose(window, true);

	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 testObj.camera->ProcessKeyboard(FORWARD, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 testObj.camera->ProcessKeyboard(BACKWARD, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 testObj.camera->ProcessKeyboard(LEFT, deltaTime);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 testObj.camera->ProcessKeyboard(RIGHT, deltaTime);
 }
#endif
//-----main-----------------------------------//
int main()
{
    // create the window and console
  
    if (FAILED(InitWindow(1280, 720)))
	{
		DestroyWindow(g_hwnd);
		return 0;
	}
	first = true;
	

  
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
        Render();
        
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
            Render();
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

  
    return 0;
}