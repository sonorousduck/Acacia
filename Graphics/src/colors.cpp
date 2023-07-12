#include "colors.hpp"

namespace Ebony
{
	Color::Color()
	{
		rgba = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Color::Color(glm::vec3 color)
	{
		rgba = glm::vec4(color, 1.0f);
	}

	Color::Color(glm::vec4 color)
	{
		rgba = color;
	}

	Color::Color(float r, float g, float b)
	{
		rgba = glm::vec4(r, g, b, 1.0f);
	}

	Color::Color(float r, float g, float b, float a)
	{
		rgba = glm::vec4(r, g, b, a);
	}

	glm::vec3 Color::GetRGB()
	{
		return glm::vec3(rgba.r, rgba.g, rgba.b);
	}

	glm::vec4 Color::GetRGBA()
	{
		return rgba;
	}
}
