#pragma once
//#include "Core.hpp"

#include <memory>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Ebony {
	class Log
	{
	
	public: 
		static void Init();


		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }
	private:

		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	
	};

}
// Core log macros
#define EB_CORE_TRACE(...)    ::Ebony::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EB_CORE_INFO(...)     ::Ebony::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EB_CORE_WARN(...)     ::Ebony::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EB_CORE_ERROR(...)    ::Ebony::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EB_CORE_CRITICAL(...)    ::Ebony::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define EB_TRACE(...)	      ::Ebony::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EB_INFO(...)	      ::Ebony::Log::GetClientLogger()->info(__VA_ARGS__)
#define EB_WARN(...)	      ::Ebony::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EB_ERROR(...)	      ::Ebony::Log::GetClientLogger()->error(__VA_ARGS__)
#define EB_CRITICAL(...)	      ::Ebony::Log::GetClientLogger()->critical(__VA_ARGS__)


