#pragma once
#include "Includes.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
namespace Armageddon
{
	class DLL Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger()
		{ 
		return c_Logger;
		}

	private : 
		static std::shared_ptr<spdlog::logger> c_Logger;

	};
}

//#define AG_LOG_TRACE(...)		Logger::GetLogger()->trace(__VA__ARGS)

