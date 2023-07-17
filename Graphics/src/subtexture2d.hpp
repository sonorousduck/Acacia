#pragma once

#include "texture.hpp"
#include <glm/glm.hpp>

// https://www.youtube.com/watch?v=raj1Y5bF_wk


class SubTexture2D
{
public:
	SubTexture2D(Texture2D& texture, const glm::vec2& min, const glm::vec2& max);

	const Texture2D& GetTexture() const { return m_Texture; }
	const glm::vec2* GetUv() const { return m_Uv; }

	static SubTexture2D CreateFromCoords(Texture2D& texture, const glm::vec2& coords, const glm::vec2& spriteSize);

private:
	Texture2D m_Texture;
	glm::vec2 m_Uv[4];

};

