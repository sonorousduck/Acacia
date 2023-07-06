#include "Ebony.hpp"
#include "camera.hpp"
#include "glm/glm.hpp"

class Sandbox : public Ebony::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox() 
	{

	}
};


Ebony::Application* Ebony::CreateApplication()
{
	Camera camera(glm::vec3(0.3f, 1.0f, 2.0f));
	return new Sandbox();
}