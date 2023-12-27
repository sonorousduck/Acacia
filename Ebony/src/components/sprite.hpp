#pragma once

#include "component.hpp"

#include "colors.hpp"
#include <texture.hpp>
#include <shader.hpp>

#include <optional>
#include <memory>

namespace components
{
	class Sprite : public PolymorphicComparable<Component, Sprite>
	{
	public:
		Sprite(Shader& shader, std::shared_ptr<Texture2D> texture, Ebony::Color spriteColor, float depth = 0.0f, bool isUI = false) : texture(texture), spriteColor(spriteColor), depth(depth), shader(shader), isUI(isUI)
		{}


		void SetTexture(std::shared_ptr<Texture2D> texture)
		{
			this->texture = texture;
		}

		std::shared_ptr<Texture2D> texture;
		Ebony::Color spriteColor;
		float depth;
		Shader& shader;
		bool isUI;
	};
}