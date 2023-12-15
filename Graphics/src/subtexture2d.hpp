#pragma once

#include "texture.hpp"
#include <glm/glm.hpp>
#include <memory>

// https://www.youtube.com/watch?v=raj1Y5bF_wk


class SubTexture2D
{
public:
	SubTexture2D(std::shared_ptr<Texture2D> texture, const glm::vec2& min, const glm::vec2& max);

	const std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
	const glm::vec2* GetUv() const { return m_Uv; }

	static SubTexture2D CreateFromCoords(std::shared_ptr<Texture2D> texture, const glm::vec2& coords, const glm::vec2& spriteSize);

private:
	std::shared_ptr<Texture2D> m_Texture;
	glm::vec2 m_Uv[4];

};

