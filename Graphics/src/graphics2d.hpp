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
#include "renderTarget2d.hpp"

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

		void Draw(const Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color, float depth = 0.0f);
		void Draw(Shader& s, Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color, float depth = 0.0f);
		void DrawInstanced(Shader& s, Texture2D& texture, unsigned int VAO, std::uint32_t count);
		void DrawRenderTarget(Shader& s, RenderTarget2D& renderTarget);


		void Cleanup();


		//void InstancedDraw();
		//void InstancedDraw(Shader& s);

		void SetRenderTarget(RenderTarget2D& renderTarget, Color clearColor);
		void UnbindRenderTarget(Color clearColor);

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

		int activeShaderId = -1;
		int activeTextureId = -1;


		unsigned int textVAO = 0, textVBO = 0;


		glm::mat4 projection;
		unsigned int quadVAO = 0;
		unsigned int instancedVAO = 0;
		unsigned int quadRenderTarget = 0;
		unsigned int particlePositionBuffer = 0, particleColorBuffer = 0;




	private:
		void Initialize();
		void initRenderData();
		void SetupCallback();

		bool hasCamera = false;


	};


}