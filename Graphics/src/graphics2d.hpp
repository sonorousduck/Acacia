#pragma once

#include "../Graphics/ThirdParty/imgui/imgui.h"
#include "../Graphics/ThirdParty/imgui/imgui_impl_opengl3.h"


#include <SDL.h>
#include "glad/glad.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <map>
#include <functional>
#include <optional>

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
#include "../Ebony/src/misc/resourceManager.hpp"

#include <memory>

namespace Ebony
{
	class Graphics2d : Graphics
	{
	public:
		// Graphics2d();

		static void Initialize(const char* windowName, int width, int height);
		static void Initialize(const char* windowName, int width, int height, int majorVersion, int minorVersion);
		static void InitializeTextDrawing(Shader& textShader);

		//// Need to load fonts as well

		static void DrawString(Shader& s, std::shared_ptr<SpriteFont> spriteFont, std::string text, glm::vec2 position, glm::vec2 size, float rotate, Color color, Color outlineColor, float depth = 0.0f, bool isUI = false);
		//void DrawString(SpriteFont& spriteFont, std::string text, float x, float y, float scale, glm::vec3 color);

		static void SetMainCamera(std::shared_ptr<Camera> camera);

		static void BeginDraw(Color clearColor);
		static void EndDraw();

		static void InitializeImgui();

		static void Draw(const std::shared_ptr<Texture2D> texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 rotationAxis, Color color, float depth = 0.0f, bool isUI = false);
		static void Draw(Shader& s, std::shared_ptr<Texture2D> texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 rotationAxis, Color color, float depth = 0.0f, bool isUI = false);
		static void DrawAnimation(Shader& s, std::shared_ptr<Texture2D> texture, std::uint16_t layer, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 rotationAxis, Color color, float depth, bool isUI = false);
		static void DrawInstanced(Shader& s, std::shared_ptr<Texture2D> texture, unsigned int VAO, std::uint32_t count);
		static void DrawRenderTarget(Shader& s, RenderTarget2D& renderTarget);

		static void BeginImgui();
		static void DrawWindow(std::string_view windowName);
		//void ImguiSlider();
		static void CompleteWindow();
		static void ImguiText(std::string_view text);
		static void EndImgui();

		static void Cleanup();


		//void InstancedDraw();
		//void InstancedDraw(Shader& s);

		static void SetRenderTarget(RenderTarget2D& renderTarget, Color clearColor);
		static void UnbindRenderTarget(Color clearColor);

		static void onFramebufferSizeChange(int width, int height);

		static void setNewWindowSize(int width, int height);

		static Window window;
		static int screenWidth;
		static int screenHeight;
		static bool firstMouse;
		static bool cursorDisabled;

		static float lastMosX;
		static float lastMosY;
		static std::shared_ptr<Camera> mainCamera;
		static unsigned int versionMajor;
		static unsigned int versionMinor;
		static const char* windowName;

		static int activeShaderId;
		static int activeTextureId;


		static unsigned int textVAO;
		static unsigned int textVBO;


		static glm::mat4 projection;
		static unsigned int quadVAO;
		static unsigned int instancedVAO;
		static unsigned int quadRenderTarget;
		static unsigned int particlePositionBuffer, particleColorBuffer;

		static ImGuiIO io;

		static bool resized;


	private:
		static void Initialize();
		static void initRenderData();
		static void SetupCallback();

		static bool hasCamera;


	};


}