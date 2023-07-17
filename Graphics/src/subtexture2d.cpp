#include "subtexture2d.hpp"


SubTexture2D::SubTexture2D(Texture2D& texture, const glm::vec2& min, const glm::vec2& max) : m_Texture(texture)
{
	m_Uv[0] = { min.x, min.y };
	m_Uv[1] = { max.x, min.y };
	m_Uv[2] = { max.x, max.y };
	m_Uv[3] = { min.x, max.y };

}


SubTexture2D SubTexture2D::CreateFromCoords(Texture2D& texture, const glm::vec2& coords, const glm::vec2& spriteSize)
{
	glm::vec2 min = { (coords.x * spriteSize.x) / texture.Width, (coords.y * spriteSize.y) / texture.Height };
	glm::vec2 max = { ((coords.x + 1) * spriteSize.x) / texture.Width, ((coords.y + 1) * spriteSize.y) / texture.Height };


	return SubTexture2D(texture, min, max);
}