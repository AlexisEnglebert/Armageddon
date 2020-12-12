#include <iostream>
#include <Application.h>
#include <StartPoint.h>
#include <Graphics/3D/Camera.h>
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "Log.h"
#include "Graphics/3D/Renderable/RenderCube.h"
#include "Graphics/3D/Renderable//RenderModels.h"
#include "Graphics/3D/Renderable//RenderSkyBox.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Component.h"
#include <WICTextureLoader.h>

class TestApp : public Armageddon::Application
{
public:

	 void OnUpdate() override;
	 void OnRender() override;	
	 void ImGuiRender() override;
	 void Init() override;
	
	 void onMouseEvent(MouseEvent::MEventType e , float x, float y);
	 Entity testEntity;
	 Entity testEntity2;
	 float Metalness;
	 float RoughNess;
	 float FrameData[60];
	 int values_offset = 0;
	 float i = 0;
	 void CameraControls();
	TestApp()
	{
	
	}

	~TestApp()
	{
	}
private:
	ImVec2 CurrentWindowSize;
	Scene m_scene = Scene(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
	bool cameraControlsActive = false;
};
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static float cameraSpeed = 0.5f;
static float CameraZoom = 1.0f;
static float CameraAmplificatior = 0.01f;
float a = 0;	



Armageddon::Application* Armageddon::CreateApplication()
{


	return new TestApp();
}
bool WireFrame = false;
void TestApp::OnUpdate()
{
	CameraControls();

	Armageddon::Application::GetWindow()->GetWindowGraphics()->SetWireFrame(WireFrame);

}


void TestApp::OnRender()
{
	
	m_scene.UpdateScene();
	
}
char buf[5000] = { 0 };

void TestApp::ImGuiRender()
{

		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		if (opt_fullscreen)
		{
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", 0, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Text", ""))
				{

				}
				
				ImGui::Separator();
				
				ImGui::EndMenu();
			}
		

			ImGui::EndMenuBar();
		}

		ImGui::End();



		ImGui::Begin("Debug", 0, ImGuiWindowFlags_AlwaysAutoResize );                          // Create a window called "Hello, world!" and append into it.
		ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.0f, 10.0f);
		ImGui::SliderFloat("Metal Ness", &Metalness, 0.0f, 100.0f);
		ImGui::SliderFloat("Rough Ness", &RoughNess, 0.0f, 5.0f);
		
		//LightDirection
	
		ImGui::Checkbox("WireFrame",&WireFrame);
		ImGui::Text("Current Cube Rendered %f", a);
		/*ImGui::Text("Current Delta Time : %f  Current FPS %f", Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetDeltaTime(),
		1.0f/ Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetDeltaTime());*/
		//values[values_offset] = cosf(phase);
		//values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);

		FrameData[(int)i] = Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetDeltaTime();
		i++;
		char overlay[32];
		sprintf_s(overlay, "Environs %f FPS", 1.0f / Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetDeltaTime());
		ImGui::PlotLines("Frame per second", FrameData, ARRAYSIZE(FrameData), i, overlay, -1.0f,1.0f, ImVec2(0.0f,50.0f ));
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();




		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });
		ImGui::Begin("Scene", 0);
		//Armageddon::Log::GetLogger()->trace("Window Id: {0}", ImGui::GetID("Scene"));
		ImGui::PopStyleVar(3);
		ImVec2 WindowSize = ImGui::GetWindowSize();
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;
		

		//Armageddon::Log::GetLogger()->trace("Width {0} Height {1}",WindowSize.x, WindowSize.y);
		if ((ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) && GetAsyncKeyState(AG_KEY_shift) && GetAsyncKeyState(AG_KEY_ctrl))
		{
			//Armageddon::Application::GetInstance()->GetWindow()->RawInput = true;
			//ShowCursor(false);
			cameraControlsActive = true;
		}
		else
		{
			//Armageddon::Application::GetInstance()->GetWindow()->RawInput = false;
			//ShowCursor(true);
			cameraControlsActive = false;

		}
		if (CurrentWindowSize.x != WindowSize.x || CurrentWindowSize.y != WindowSize.y)
		{
		
			Armageddon::Log::GetLogger()->trace("ON CHANGE DE TAILLE");
			Armageddon::Application::GetWindow()->GetWindowGraphics()->GetOfsRender()->ResizeRenderTargetView(
				Armageddon::Application::GetWindow()->GetWindowGraphics()->Getdevice(),
				Armageddon::Application::GetWindow()->GetWindowGraphics()->GetSwapChain(),
				vMax.x - vMin.x, vMax.y - vMin.y,
				nullptr );

			CurrentWindowSize = ImGui::GetWindowSize();
		}
		
		ImGui::Image(Armageddon::Application::GetWindow()->GetWindowGraphics()->GetOfsRender()->GetShaderRessource(), { Armageddon::Application::GetWindow()->GetWindowGraphics()->GetOfsRender()->GetImageX(), 
			Armageddon::Application::GetWindow()->GetWindowGraphics()->GetOfsRender()->GetImageY()});
		ImGui::End();

		m_scene.DrawGameObjectScene();



}

void TestApp::Init()
{

	Armageddon::Application::GetWindow()->SetMouseCallBack(std::bind(&TestApp::onMouseEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	ImGui::SetCurrentContext(Armageddon::Application::GetContext());
	
	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->SetPosition(0.0f, 0.0f, -1.0f);
	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->SetProjectionValues(static_cast<float>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowWidth()), 
		 static_cast<float>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowHeight()), 0.1f, 1000.0f);



	auto ent = m_scene.CreateEntity();
	ent.AddComponent<TagComponent>("SkyBox");
	ent.AddComponent<TransFormComponent>();

	ent.AddComponent<SkyBoxComponent>(L"..\\TestApplication\\assets\\Textures\\skybox\\skybox1_espace.dds", L"..\\TestApplication\\assets\\Textures\\skybox\\skybox1IR.dds",m_scene.DrawableEntity,ent.GetHandle());
	Armageddon::Log::GetLogger()->trace("Create SkyBox");


	auto ment = m_scene.CreateEntity();
	ment.AddComponent<TagComponent>("Mesh");
	ment.AddComponent<TransFormComponent>(0.0f,0.0,0.0,0.0,0.0,0.0,100.0,2.0,100.0);
	ment.AddComponent<MeshComponent>(m_scene.DrawableEntity, ment.GetHandle());
	ment.AddComponent<MaterialComponent>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
	auto& mat = ment.GetComponent<MaterialComponent>();
	//mat.SetColorMap(L"..\\TestApplication\\assets\\Textures\\white.tif");
	mat.SetNormalMap(L"..\\TestApplication\\assets\\Textures\\default_normal.jpeg");
	mat.SetOcclusionMap(L"..\\TestApplication\\assets\\Textures\\white.tif");
	mat.SetSpecularMap(L"..\\TestApplication\\assets\\Textures\\white.tif");



	//Armageddon::Log::GetLogger()->trace("Create Mesh");*/


}


void TestApp::onMouseEvent(MouseEvent::MEventType e ,float x , float y)
{
	if (cameraControlsActive)
	{
		switch (e)
		{
		case  MouseEvent::MEventType::Wheeldown:
			CameraZoom -= CameraAmplificatior;
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetBackwardVec() * cameraSpeed);

			break;
		case  MouseEvent::MEventType::Wheelup:
			CameraZoom += CameraAmplificatior;
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetForwardVec() * cameraSpeed);


			break;

		case MouseEvent::MEventType::Move:

			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustRotation(y * 0.01f, x * 0.01f, 0.0f);
			break;
		}
	}
	
		


	
}


void TestApp::CameraControls()
{
	
	if (cameraControlsActive)
	{
		//GetAsyncKeyState();


		if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_S))
		{
			//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetUpVec() * cameraSpeed);
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetBackwardVec() * cameraSpeed);

		}
		if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_Space))
		{
			//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetUpVec() * cameraSpeed);
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetUpVec() * cameraSpeed);

		}


		if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_Q))
		{
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetLeftVec() * cameraSpeed);

		}

		if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_Z))
		{
			//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetDownVec() * cameraSpeed);
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetForwardVec() * cameraSpeed);

		}

		if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_D))
		{
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetRightVec() * cameraSpeed);
		}
	}
}


