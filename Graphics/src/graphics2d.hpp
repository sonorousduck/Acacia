#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
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

namespace Ebony
{
	class Graphics2d
	{
	public:
		Graphics2d();

		void Initialize(const char* windowName, int width, int height);
		//
		//void Initialize(
		//	const char* windowName, 
		//	unsigned int width, 
		//	unsigned int height, 
		//	std::function<void(GLFWwindow*)> input_callback,
		//	std::function<void(GLFWwindow*, double, double)> mouse_callback,
		//	std::function<void(GLFWwindow*, double, double)> scroll_callback
		//);

		//void Initialize(
		//	const char* windowName,
		//	unsigned int width,
		//	unsigned int height,
		//	std::function<void(GLFWwindow*)> input_callback
		//);
		//
		//void Initialize(
		//	const char* windowName,
		//	unsigned int width,
		//	unsigned int height,
		//	std::function<void(GLFWwindow*, double, double)> mouse_callback
		//);


		//void Initialize(
		//	const char* windowName,
		//	unsigned int width,
		//	unsigned int height,
		//	std::function<void(GLFWwindow*)> input_callback,
		//	std::function<void(GLFWwindow*, double, double)> mouse_callback
		//);


		//void Initialize(
		//	const char* windowName,
		//	unsigned int width,
		//	unsigned int height,
		//	std::function<void(GLFWwindow*, double, double)> mouse_callback,
		//	std::function<void(GLFWwindow*, double, double)> scroll_callback
		//);

		//void Initialize(char* windowName, unsigned int width, unsigned int height, unsigned int majorVersion, unsigned int minorVersion);


		//Texture2D loadTexture(char const* path);
		//Shader loadShader(char const* path, bool extensionIncluded = false); // Just a wrapper. Assumes .vert and .frag extensions and that the path didn't contain an extension and the vert and frag shaders are named the same
		//Shader loadShader(char const* vertPath, char const* fragPath, bool extensionIncluded = false); // Just a wrapper. Assumes .vert and .frag extensions and that the path didn't contain an extension


		//// Need to load fonts as well

		//void DrawText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);
		//void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);

		//void BeginDraw();
		//void EndDraw();

		//void Draw();
		//void Draw(Shader& s);

		//void InstancedDraw();
		//void InstancedDraw(Shader& s);

		//void SetRenderTarget();



		//void framebuffer_size_callback(GLFWwindow* window, int width, int height);

		//void input_callback(GLFWwindow* window);

		//void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);

		//void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

		void onCursorPos(double x, double y);
		void onFramebufferSizeChange(int width, int height);
		void onScroll(double xOffset, double yOffset);
		void onMouseButton(int button, int action, int mods);
		void onKeyInput(int key, int scancode, int action, int mods);

		GLFWwindow* window{};
		Input* input;
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
	private:
		void Initialize();

	};


}