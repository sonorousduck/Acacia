#pragma once

#include "component.hpp"

#include "colors.hpp"
#include <texture.hpp>
#include <shader.hpp>

#include <optional>

namespace components
{
	class ColliderSprite : public PolymorphicComparable<Component, ColliderSprite>
	{
	public:
		ColliderSprite(Shader& shader, Texture2D& texture, Ebony::Color spriteColor, float depth = 0.0f) : texture(texture), spriteColor(spriteColor), depth(depth), shader(shader)
		{}


		Texture2D& texture;
		Ebony::Color spriteColor;
		float depth;
		Shader& shader;
	};
}