#include "Log.hpp"

namespace Ebony {

	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::m_ClientLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // Timestamp: name of logger: then actual message
		m_CoreLogger = spdlog::stdout_color_mt("Ebony");
		m_CoreLogger->set_level(spdlog::level::trace);
		m_ClientLogger = spdlog::stdout_color_mt("Client");
		m_ClientLogger->set_level(spdlog::level::trace);
	}

}