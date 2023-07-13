#pragma once
#include "texture.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <utility>


class SpriteSheet : Texture2D
{
public:
	int numDivisions;
	std::vector<float> timings;
	// Use emplace_back(x, y) to automatically create the std::pair
	std::vector<std::pair<glm::vec2, glm::vec2>> divisions;


};