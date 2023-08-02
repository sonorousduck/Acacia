#pragma once
#include "texture.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <utility>
#include <chrono>

class SpriteSheet
{
public:
	SpriteSheet(Texture2D& spritesheet, std::uint8_t numDivisions, std::vector<std::chrono::microseconds> timings) : spritesheet(spritesheet), numDivisions(numDivisions), timings(timings)
	{
		divisions = std::vector<std::pair<glm::vec2, glm::vec2>>();
	}

	SpriteSheet(Texture2D& spritesheet, std::vector<std::pair<glm::vec2, glm::vec2>> divisions, std::vector<std::chrono::microseconds> timings) : spritesheet(spritesheet), divisions(divisions), timings(timings)
	{
		numDivisions = static_cast<std::uint16_t>(divisions.size());
	}

	Texture2D& spritesheet;
	std::uint16_t numDivisions;
	std::vector<std::chrono::microseconds> timings;
	// Use emplace_back(x, y) to automatically create the std::pair
	std::vector<std::pair<glm::vec2, glm::vec2>> divisions;

	// Probably the easiest way to check. If the texture IDs match, then they are the same sprite sheet. (Though check division count in case)
	bool operator==(SpriteSheet& rhs)
	{
		return spritesheet.ID == rhs.spritesheet.ID && numDivisions == rhs.numDivisions;
	}


};