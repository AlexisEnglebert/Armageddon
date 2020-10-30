#pragma once
#include "Includes.h"
#include "Window.h"
#include "Events/Event.h"
#include "imgui.h"
//int main(int argc, char** argv);

int main(int argc, char** argv);


namespace Armageddon
{


	class DLL Application 
	{

	public:
		Application();
		virtual ~Application();

		inline Armageddon::Window* GetWindow() { return wind; }
		inline static Application* GetInstance() { return Instance; };
		void ApplicationRun();
		void OnEvent(Event& e);
		void CameraControls();
		virtual void OnUpdate() = 0 ;
		virtual void OnRender() = 0;
		virtual void ImGuiRender() = 0;
		virtual void Init() = 0;

		ImGuiContext* GetContext() { return context; };
		

	  void ImGuiInit();
	  void ImGuiBegin();
	  void ImGuiEnd();
	private:
		static Application* Instance;
		Armageddon::Window* wind = new Armageddon::Window;
		ImGuiContext* context;
		//friend int ::main(int argc, char** argv);
		friend int ::main(int argc, char** argv);

		

	};

	 Application* CreateApplication();
	 

}