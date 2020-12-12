#pragma once

#include <string>
#include "Includes.h"
#include"Events/Event.h"
#include "KeyboardClass.h"
#include "Events/MouseEvent.h";
#include "Graphics/3D/D3D_graphics.h"
namespace Armageddon
{

	class DLL Window
	{
		
	public:

		Window* Init(int height, int width, std::string title, std::string w_class);
		bool PorcessMessage();
		void SetEventCallBack(const std::function<void(Event&)>& callbackfunc) { Callback = callbackfunc; };
		void SetMouseCallBack(const std::function<void(MouseEvent::MEventType e, float x, float y)>& callbackfunc) { MouseCallBack = callbackfunc; };

		~Window();
		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam);
		inline KeyboardClass GetNativeKeyBoard() { return Keyboard; }
		inline D3D_graphics* GetWindowGraphics() { return &graphics; }
		inline HWND* GetHandleWindow() { return &Handle; };
		float GetWindowHeight() { return this->window_height; };
		float GetWindowWidth() { return this->window_width; };
		bool RawInput = false;
		std::string openfilename(const char* filter = "All Files (*.*)\0*.*\0", HWND owner = NULL);
	protected:
		KeyboardClass Keyboard;
	private:
		void RegisterWindowClass();
		float window_height =0 ;
		float window_width = 0;


		float m_old_mouse_posX = 0.0f;
		float m_old_mouse_posY = 0.0f;

		std::function<void(Event&)> Callback;
		std::function<void(MouseEvent::MEventType e , float x, float y)> MouseCallBack;

		HWND Handle = NULL;
		std::string window_title = "";
		std::string window_class = "";
		HINSTANCE Instance = NULL;
		D3D_graphics graphics;
		friend class D3D_graphics;


	};


}
