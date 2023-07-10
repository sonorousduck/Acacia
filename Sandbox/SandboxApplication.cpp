#include "Ebony.hpp"
#include "camera.hpp"
#include "glm/glm.hpp"

class Sandbox : public Ebony::Application
{
public:
	int test = 0;
	Sandbox()
	{
		
	}

	~Sandbox() 
	{

	}

	void Update()
	{
		//std::cout << "Updating!" << std::endl;

	}

	void ProcessInput()
	{
		//std::cout << "Processing!" << std::endl;

	}

	void Draw()
	{
		//std::cout << "Drawing!" << std::endl;

	}

};


Ebony::Application* Ebony::CreateApplication()
{
	Camera camera(glm::vec3(0.3f, 1.0f, 2.0f));
	return new Sandbox();
}