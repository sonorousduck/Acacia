#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <map>
#include <functional>

#include <ft2build.h>
#include FT_FREETYPE_H


#include "shader.hpp"
#include "camera.hpp"
#include "asset_load.hpp"
#include "texture_asset.hpp"
#include "texture.hpp"
#include "input.hpp"
#include "colors.hpp"
#include "window.hpp"
#include "graphics.hpp"
#include "spritefont.hpp"
#include "colors.hpp"

namespace Ebony
{
	class Graphics2d : Graphics
	{
	public:
		Graphics2d();

		void Initialize(const char* windowName, int width, int height);
		void Initialize(const char* windowName, int width, int height, int majorVersion, int minorVersion);
		void InitializeTextDrawing(Shader& textShader);

		//// Need to load fonts as well

		void DrawString(Shader& s, SpriteFont& spriteFont, std::string text, float x, float y, float scale, Color color);
		//void DrawString(SpriteFont& spriteFont, std::string text, float x, float y, float scale, glm::vec3 color);

		void SetMainCamera(Camera& camera);

		void BeginDraw(Color clearColor);
		void EndDraw();

		void Draw(const Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color);
		void Draw(Shader& s, Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color, int layer);


		void Cleanup();


		//void InstancedDraw();
		//void InstancedDraw(Shader& s);

		//void SetRenderTarget();

		void onFramebufferSizeChange(int width, int height);

		Window window;
		int screenWidth;
		int screenHeight;
		bool firstMouse = true;
		bool cursorDisabled = true;

		float lastMosX;
		float lastMosY;
		Camera mainCamera;
		unsigned int versionMajor;
		unsigned int versionMinor;
		const char* windowName;

		unsigned int textVAO = 0, textVBO = 0;


		glm::mat4 projection;

	private:
		void Initialize();
		void initRenderData();
		void SetupCallback();

		unsigned int quadVAO = 0;
		bool hasCamera = false;


	};


}