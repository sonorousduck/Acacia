#pragma once

// This is an error I believe spdlog is throwing.
#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING


extern Ebony::Application* Ebony::CreateApplication();

int main(int argc, char** argv)
{
	//Ebony::Log::Init();
	//EB_CORE_WARN("Initialized Log!");

	auto app = Ebony::CreateApplication();
	app->Run();
	delete app;
}