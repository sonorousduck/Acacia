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

namespace Ebony
{
	class Graphics2d
	{
	public:
		Graphics2d();

		void Initialize(const char* windowName, int width, int height);
		void Initialize(const char* windowName, int width, int height, int majorVersion, int minorVersion);

		//// Need to load fonts as well

		//void DrawText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);
		//void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);

		void SetMainCamera(Camera& camera);

		void BeginDraw(Color clearColor);
		void EndDraw();

		void Draw(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color);
		void Draw(Shader& s, Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color);

		void Cleanup();


		//void InstancedDraw();
		//void InstancedDraw(Shader& s);

		//void SetRenderTarget();

		//void onCursorPos(double x, double y);
		void onFramebufferSizeChange(int width, int height);
		//void onScroll(double xOffset, double yOffset);
		//void onMouseButton(int button, int action, int mods);
		//void onKeyInput(int key, int scancode, int action, int mods);

		Window window;
		//Input input;
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

		glm::mat4 projection;

	private:
		void Initialize();
		void initRenderData();
		void SetupCallback();

		unsigned int quadVAO = 0;
		bool hasCamera = false;


	};


}