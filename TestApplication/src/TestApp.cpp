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
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Component.h"

class TestApp : public Armageddon::Application
{
public:

	 void OnUpdate() override;
	 void OnRender() override;	
	 void ImGuiRender() override;
	 void Init() override;
	
	 void onMouseEvent(MouseEvent::MEventType);
	 Entity testEntity;
	 Entity testEntity2;
	 void CameraControls();
	TestApp()
	{
	
	}

	~TestApp()
	{
	}
private:
	ImVec2 CurrentWindowSize;
	Scene m_scene;

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
	/*Armageddon::Log::GetLogger()->info("Projection Maxtrix  X : {0}     Y : {1}       Z : {2}       ",
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetPos().x,
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetPos().y,
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetPos().z);*/
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
		ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.0f, 2.0f);
		ImGui::Checkbox("WireFrame",&WireFrame);
		ImGui::Text("Current Cube Rendered %f", a);
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();




		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });
		ImGui::Begin("Scene", 0);
		ImGui::PopStyleVar(3);
		ImVec2 WindowSize = ImGui::GetWindowSize();
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;
		
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
	//ImGui::SetCurrentContext();
	Armageddon::Application::GetWindow()->SetMouseCallBack(std::bind(&TestApp::onMouseEvent, this, std::placeholders::_1));
	ImGui::SetCurrentContext(Armageddon::Application::GetContext());
	
	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->SetPosition(0.0f, 0.0f, -1.0f);
	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->SetProjectionValues(static_cast<float>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowWidth()), 
		 static_cast<float>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowHeight()), 0.1f, 1000.0f);
	
	
	/*auto r = std::make_unique<RenderCube>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(r));
	
	Armageddon::Log::GetLogger()->trace("CREATE A NEW ENITTTYYY");

	auto test_entity = m_scene.CreateEntity();
	test_entity.SetModel("..\\TestApplication\\assets\\Models\\suzanne.obj");
	//	test_entity.AddComponent<ModelComponent>("..\\TestApplication\\assets\\Models\\suzanne.obj");
	test_entity.AddComponent<TagComponent>("TestComponent");
	test_entity.AddComponent<TransFormComponent>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	//testEntity = test_entity;
	/*RenderCube r(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(r.GetObjectW());*/
	//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->DrawSprite(L"..\\TestApplication\\assets\\Textures\\Ship.png",960,
		///540);

/*	auto m = std::make_unique<RenderModels>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(m));
	*/
	//Armageddon::Log::GetLogger()->trace("ADRESS B : {0}", &o[0]->b);


	//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects()->at(0).get().GetInstance()->GetName(); //TOUT EST BON 
	a++;
}

void TestApp::onMouseEvent(MouseEvent::MEventType e)
{
	switch (e)
	{
	case  MouseEvent::MEventType::Wheeldown:
		CameraZoom -= CameraAmplificatior;
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetBackwardVec() * cameraSpeed);

		/*if (CameraZoom > 0) {
		/*	Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->SetZoomLevel(static_cast<float>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowWidth())
				, static_cast<float>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowHeight()), CameraZoom);*/
		//}
		break;
	case  MouseEvent::MEventType::Wheelup:
		CameraZoom += CameraAmplificatior;
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetForwardVec() * cameraSpeed);

	
		break;

	case MouseEvent::MEventType::RAW_MOVE:

		MouseEvent me = Armageddon::Application::GetInstance()->GetWindow()->GetNativeMouse().ReadEvent();

		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustRotation(me.GetY() * 0.01f, me.GetY() * 0.01f, 0);
		break;
	}
	
		


	
}






void TestApp::CameraControls()
{
	//Armageddon::Log::GetLogger()->trace(" {0}      {1}", Armageddon::Application::GetInstance()->GetWindow()->GetNativeMouse().GetPos().x
	//	, Armageddon::Application::GetInstance()->GetWindow()->GetNativeMouse().GetPos().y);
	
		if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_E))
		{
			auto r = std::make_unique<RenderCube>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(r));
			a++;
		}
		if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_R))
		{
			/*auto m = std::make_unique<RenderModels>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(m));*/

			//auto test_entity = m_scene.CreateEntity();
			//test_entity.SetModel("..\\TestApplication\\assets\\Models\\suzanne.obj");
		//	test_entity.AddComponent<ModelComponent>("..\\TestApplication\\assets\\Models\\suzanne.obj");
			//test_entity.AddComponent<TagComponent>("TestComponent");
		//	test_entity.AddComponent<TransFormComponent>(DirectX::XMFLOAT3(0.0f,0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
			//testEntity = test_entity;


			/*auto test_entity2 = m_scene.CreateEntity();
			test_entity2.SetModel("..\\TestApplication\\assets\\Models\\suzanne.obj");
			//	test_entity.AddComponent<ModelComponent>("..\\TestApplication\\assets\\Models\\suzanne.obj");
			test_entity2.AddComponent<TagComponent>("efef");
			test_entity2.AddComponent<TransFormComponent>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
			//testEntity2 = test_entity2;*/
		
		}
	

	if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_S))
	{
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetUpVec() * cameraSpeed);

	}
	if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_C))
	{
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustRotation(1 * CameraAmplificatior,0,0);

	}
	if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_V))
	{
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustRotation(0,1 * CameraAmplificatior, 0);

	}
	if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_B))
	{
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustRotation(0,0,1 * CameraAmplificatior);

	}

	if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_Q))
	{
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetRightVec() * cameraSpeed);

	}

	if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_Z))
	{
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetDownVec() * cameraSpeed);
	}

	if (Armageddon::Application::GetInstance()->GetWindow()->GetNativeKeyBoard().KeyIsPressed(AG_KEY_D))
	{
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->AdjustPosition(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera()->GetLeftVec() * cameraSpeed);
	}
}

