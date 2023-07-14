#pragma once
#include "texture.hpp"
#include <glm/glm.hpp>
#include <unordered_map>

namespace Ebony 
{
    struct Character
    {
        unsigned int TextureID;     // ID handle of the glyph texture
        glm::ivec2   Size;          // Size of the glyph
        glm::ivec2   Bearing;       // Offset from baseline to left/top of glyph
        unsigned int Advance;       // Offset to advance the next glyph
    };

    class SpriteFont
    {
    public:
        SpriteFont() {};

        //Texture2D texture{};
        std::unordered_map<char, Character> characters{};


        void LoadFont(const char* fontLocation);

	    // glyph bounds
	    // cropping

    };
}
