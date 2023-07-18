#pragma once
#include <cstdint>
#include <glad/glad.h>

namespace Ebony
{
	class RenderTarget2D
	{
	public:
		RenderTarget2D() {}

		static RenderTarget2D Create(std::uint16_t width, std::uint16_t height, GLint minFilter, GLint magFilter, bool depthStencil = true); // Eventually, this should be able to take in the filtering

		unsigned int GetFramebuffer()
		{
			return m_Framebuffer;
		}

		unsigned int GetTextureColorBuffer()
		{
			return m_TextureColorBuffer;
		}


	private:
		RenderTarget2D(std::uint16_t width, std::uint16_t height) : m_Width(width), m_Height(height)
		{}

		RenderTarget2D(std::uint16_t width, std::uint16_t height, unsigned int framebuffer, unsigned int textureColorBuffer, unsigned int rbo) : m_Width(width), m_Height(height), m_Framebuffer(framebuffer), m_TextureColorBuffer(textureColorBuffer), m_Rbo(rbo)
		{}

		std::uint16_t m_Width = 0;
		std::uint16_t m_Height = 0;
		unsigned int m_Framebuffer = 0;
		unsigned int m_TextureColorBuffer = 0;
		unsigned int m_Rbo = 0;

		



	};


}