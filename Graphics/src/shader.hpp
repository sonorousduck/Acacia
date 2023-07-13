#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// The program ID
	unsigned int ID = 0;

	// Constructor reads and builds the shader
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);

	void LoadShader(const char* vertexPath, const char* fragmentPath);

	// Use/Activate the shader
	void use();

	// Utility Uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, glm::vec4 value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, glm::mat2 value) const;
	void setMat3(const std::string& name, glm::mat3 value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;

	static bool checkSuccessfulShaderCompilation(const unsigned int& shader, const std::string& type);
	static bool checkSuccessfulShaderProgramLinking(const unsigned int& program);


};