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
		/*Sprite(Shader& shader, std::shared_ptr<Texture2D> texture, Ebony::Color spriteColor, float depth = 0.0f, bool isUI = false) : texture(texture), spriteColor(spriteColor), depth(depth), shader(shader), isUI(isUI), layer(0)
		{}*/

		Sprite(Shader& shader, std::shared_ptr<Texture2D> texture, Ebony::Color spriteColor, float depth = 0.0f, bool isUI = false, std::uint64_t layer = 0, bool isAtlas = false) : texture(texture), spriteColor(spriteColor), depth(depth), shader(shader), isUI(isUI), layer(layer), isAtlas(isAtlas)
		{}


		void SetTexture(std::shared_ptr<Texture2D> texture)
		{
			this->texture = texture;
		}

		std::shared_ptr<Texture2D> texture;
		Ebony::Color spriteColor;
		float depth;
		std::uint64_t layer;
		Shader& shader;
		bool isUI;
		bool isAtlas;
	};
}