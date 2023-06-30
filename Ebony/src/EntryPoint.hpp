#pragma once



extern Ebony::Application* Ebony::CreateApplication();

int main(int argc, char** argv)
{
	Ebony::Log::Init();
	EB_CORE_WARN("Initialized Log!");

	auto app = Ebony::CreateApplication();
	app->Run();
	delete app;
}