#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Ebony
{
	class RenderTarget2D
	{
	public:
		RenderTarget2D() {}

		static RenderTarget2D Create(std::uint16_t width, std::uint16_t height, GLint minFilter, GLint magFilter, bool depthStencil = true, bool usesCamera = true); // Eventually, this should be able to take in the filtering

		unsigned int GetFramebuffer() const
		{
			return m_Framebuffer;
		}

		unsigned int GetTextureColorBuffer() const
		{
			return m_TextureColorBuffer;
		}

		glm::vec2 GetSize()
		{
			return glm::vec2(m_Width, m_Height);
		}


		void Resize(std::uint16_t newWidth, std::uint16_t newHeight);

	private:
		RenderTarget2D(std::uint16_t width, std::uint16_t height) : m_Width(width), m_Height(height)
		{}

		RenderTarget2D(std::uint16_t width, std::uint16_t height, unsigned int framebuffer, unsigned int textureColorBuffer, unsigned int rbo, bool depthStencil, bool usesCamera) : 
			m_Width(width), m_Height(height), 
			m_Framebuffer(framebuffer), 
			m_TextureColorBuffer(textureColorBuffer),
			m_Rbo(rbo), 
			depthStencil(depthStencil),
			usesCamera(usesCamera)
		{}

		std::uint16_t m_Width = 0;
		std::uint16_t m_Height = 0;
		unsigned int m_Framebuffer = 0;
		unsigned int m_TextureColorBuffer = 0;
		unsigned int m_Rbo = 0;
		bool depthStencil = true;
		bool usesCamera = true;

		



	};


}