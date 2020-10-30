#include "Includes.h"
#include "Window.h";
#include "Application.h"

#pragma once
#define _CRTDBG_MAP_ALLOC


extern Armageddon::Application* Armageddon::CreateApplication();

int main(int argc, char** argv)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF  | _CRTDBG_LEAK_CHECK_DF);
#endif
	Armageddon::Log::init();
	Armageddon::Log::GetLogger()->trace("Logger Dit Hello ");

	auto application = Armageddon::CreateApplication();

	//wind.SetEventCallback(std::bind(&application->GetInstance()->OnEvent()),,std::placeholders::_1));

	
	application->ApplicationRun();
	delete application;
}
