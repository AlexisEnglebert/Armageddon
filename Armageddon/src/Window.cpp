#pragma once
#include "Window.h"
#include <WinUser.h>
#include <windef.h>
#include "includes.h"
#include <future>
#include <hidusage.h>

#include "Application.h"
#include "Events/Event.h"
#include "Graphics/3D/D3D_graphics.h"

#include "backends/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace Armageddon
{


	Window* Window::Init(int width, int height, std::string title, std::string w_class)
	{
		this->window_height = height;
		this->window_width = width;
		this->window_title = title;
		this->window_class = w_class;

		this->Instance = (HINSTANCE)GetModuleHandle(NULL);

		this->RegisterWindowClass();
		Armageddon::Log::GetLogger()->trace("Register Class ");

		this->Handle = CreateWindowEx(
			0,                              // Optional window styles.
			this->window_class.c_str(),                     // Window class
			this->window_title.c_str(),    // Window text
			WS_OVERLAPPEDWINDOW,            // Window style	
			CW_USEDEFAULT, CW_USEDEFAULT, 
			width, 
			height, // Size and position
			NULL,       // Parent window    
			NULL,       // Menu
			this->Instance,  // Instance handle
			this        // Additional application data
		);
		if (this->Handle == NULL)
		{
			Armageddon::Log::GetLogger()->trace( "CreateWindowEx a échoué pour la fenêtre : " + this->window_title);
			
		}
		Armageddon::Log::GetLogger()->trace("Window Created ");
		if (!graphics.Init(this->Handle, this->window_height, this->window_width))
		{
			Armageddon::Log::GetLogger()->trace("ERROR WHEN INIT GRAPHICS ");

		}

		

		ShowWindow(this->Handle, SW_MAXIMIZE);
		SetForegroundWindow(this->Handle);
		SetFocus(this->Handle);
		Armageddon::Log::GetLogger()->trace("Window Finished ");
		
		
		
		return this;
		
	}
	

	



	Window::~Window()
	{
		if (this->Handle != NULL)
		{
			UnregisterClass(this->window_class.c_str(), this->Instance);
			DestroyWindow(this->Handle);
		}
	}

	
	
	LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wparam, lparam))
		{
			return true;
		}

		switch (uMsg)
		{

		case WM_SIZE:
		{
			float width = LOWORD(lparam);
			float height = HIWORD(lparam);
			if (wparam != SIZE_MINIMIZED && graphics.Getdevice() != NULL)
			{	
				graphics.ResizeBuffer(width, height);
			}

			this->window_height = height;
			this->window_width = width;
			return 0;
		}
		case WM_CHAR:
		{	
		
			
			unsigned char letter = static_cast<unsigned char>(wparam); // on récupère la lettre via le param Wparam
			Keyboard.OnChar(letter);
			Event e;
			e.type = EventType::KeyPressed;
			//Callback(e);


			return 0;
		}
		case WM_KEYDOWN:
		{	

		
			unsigned char keytype = static_cast<unsigned char>(wparam); 
			Event e;
			e.type = EventType::KeyPressed;
			Keyboard.OnKeyPressed(keytype);
			Callback(e);

			return 0;
		}

		case WM_KEYUP:  
		{

			unsigned char keytype = static_cast<unsigned char>(wparam);
			Event e;
			e.type = EventType::KeyReleased;
			Keyboard.OnKeyReleased(keytype);
			Callback(e);

			return 0;

		}
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.onMouseMove(x, y);
			//Armageddon::Log::GetLogger()->trace("Mouse Moved : {0} , {1} ",x,y);
			MouseCallBack(MouseEvent::MEventType::Move);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.onLeftPressed(x, y);
			MouseCallBack(MouseEvent::MEventType::Lpress);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.onRightPressed(x, y);
			MouseCallBack(MouseEvent::MEventType::Rpress);

			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.onMiddlePressed(x, y);
			MouseCallBack(MouseEvent::MEventType::Mpress);

			return 0;
		}
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.onLeftReleased(x, y);
			MouseCallBack(MouseEvent::MEventType::Lrelease);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.onRightReleased(x, y);
			MouseCallBack(MouseEvent::MEventType::Rrelease);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.onMiddleReleased(x, y);
			MouseCallBack(MouseEvent::MEventType::Mpress);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
			{

				mouse.onWheelUp(x, y);
				MouseCallBack(MouseEvent::MEventType::Wheelup);

			}
			else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
			{

				mouse.onWheelDown(x, y);
				MouseCallBack(MouseEvent::MEventType::Wheeldown);

			}
			break;
		}
		case WM_INPUT:
		{
			

			UINT DataSize;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, NULL, &DataSize, sizeof(RAWINPUTHEADER));

			if (DataSize > 0)
			{
				std::unique_ptr<BYTE[]> raw_data = std::make_unique<BYTE[]>(DataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, raw_data.get(), &DataSize, sizeof(RAWINPUTHEADER)) == DataSize)
				{
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(raw_data.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						//mouse.onRawMouseMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					//	MouseCallBack(MouseEvent::MEventType::RAW_MOVE);
					}
				}
			}
			return DefWindowProc(hwnd, uMsg, wparam, lparam);
		}

		
			default:
				return DefWindowProc(hwnd, uMsg, wparam, lparam);
			
		}

	}
	
	

	bool Window::PorcessMessage()
	{
		// Handle the windows messages.
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG)); 
		graphics.RenderFrame();
		if (PeekMessage(&msg, 
			this->Handle, 
			0,    
			0,   
			PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}

		// Check if the window was closed
		if (msg.message == WM_NULL)
		{
			if (!IsWindow(this->Handle))
			{
				this->Handle = NULL; 
				UnregisterClass(this->window_class.c_str(), Instance);
				return false;
			}
		}
		return true;
	}
	
	LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
	{
		
		if (hwnd == NULL)
		{
			Log::GetLogger()->error("HWND est null");
		}
		switch (uMsg)
		{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}

		default:
			Window* const ptrWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (ptrWindow == NULL)
			{
				Log::GetLogger()->error("ERREUR CRITIQUE  PTRWINDOW  EST NULL");
			}
			return ptrWindow->WindowProc(hwnd, uMsg, wparam, lparam);

		}
	}
	LRESULT CALLBACK CustomWindowProcSetup(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)   
	{
		switch (uMsg)
		{
			case WM_NCCREATE:
			{
				Log::GetLogger()->trace("WINDOW CREATE EVENT.");
				
				const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
				Window* ptrWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
				if (ptrWindow == nullptr)
				{
					Log::GetLogger()->error("Critical Error: Pointer to window is null.");
	
					exit(-1);
				}

				

				SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptrWindow));
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));

				
				
				return ptrWindow->WindowProc(hwnd, uMsg, wparam, lparam);
	
			}

		default:
			return DefWindowProc(hwnd, uMsg, wparam, lparam);
		}

		

	}

	std::string Window::openfilename(const char* filter, HWND owner)
	{
	
			OPENFILENAME ofn;
			char fileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = owner;
			ofn.lpstrFilter = filter;
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = "";
			std::string fileNameStr;
			if (GetOpenFileName(&ofn))
				fileNameStr = fileName;
			return fileNameStr;
		
	}

	void Window::RegisterWindowClass()
	{
		WNDCLASSEX wc; //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
		wc.style = CS_HREDRAW | CS_VREDRAW; //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
		wc.lpfnWndProc = CustomWindowProcSetup; //Pointer to Window Proc function for handling messages from this window
		wc.cbClsExtra = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
		wc.cbWndExtra = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
		wc.hInstance = this->Instance; //Handle to the instance that contains the Window Procedure
		wc.hIcon = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
		wc.hIconSm = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
		wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
		wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
		wc.lpszMenuName = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
		wc.lpszClassName = this->window_class.c_str(); //Pointer to null terminated string of our class name for this window.
		wc.cbSize = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize
		RegisterClassEx(&wc); // Register the class so that it is usable.

	}

}
