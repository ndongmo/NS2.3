/**
 * @file GLSLProgram.cpp
 * @brief High-performance OpenGL Shading Language (GLSL) management implementation.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated to C++20/3D Layouts for NS2 Architecture
 */

#include "GLSLProgram.h"
#include "IOManager.h"
#include "Logger.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp> // Required for unpacking uniform matrices

namespace NS2
{
	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		std::string vertexSource;
		std::string fragSource;

		// Maintain seamless compatibility with your custom IO engine layer file-buffer reader
		IOManager::readFileToBuffer(vertexShaderFilePath, vertexSource);
		IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);

		compileShadersFromSource(vertexSource.c_str(), fragSource.c_str());
	}

	void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource)
	{
		// Allocate the hardware tracking program container if it doesn't exist
		m_programID = glCreateProgram();

		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) {
			logE << "SHADER_SYSTEM: Vertex shader failed to be created!" << std::endl;
		}

		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) {
			logE << "SHADER_SYSTEM: Fragment shader failed to be created!" << std::endl;
		}

		// Compile independent sub-stage generation passes smoothly
		compileShaders(vertexSource, "Vertex shader", m_vertexShaderID);
		compileShaders(fragmentSource, "Fragment shader", m_fragmentShaderID);
	}

	void GLSLProgram::linkShaders() {
		// Attach our shaders to our program container
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		// Link our program pipelines down into active hardware configurations
		glLinkProgram(m_programID);

		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength value includes the terminating NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			// Flush allocations instantly to prevent driver leaks
			glDeleteProgram(m_programID);
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);

			logE << &(errorLog[0]) << std::endl;
			logE << "SHADER_SYSTEM: Shader pipeline failed to link properly!" << std::endl;
			Logger::exitGame();
		}

		// Always detach shaders cleanly after a successful hardware link pass
		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName) {
		// Map explicit input layout location indices utilizing your exact spelling parameter
		glBindAttribLocation(m_programID, m_numAttributtes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			logE << "SHADER_SYSTEM: Uniform variable '" << uniformName << "' not found in active pipeline layout!" << std::endl;
			Logger::exitGame();
		}
		return location;
	}

	void GLSLProgram::use() {
		glUseProgram(m_programID);
		// Retain your custom loop design enabling layout attribute bounds automatically
		for (int i = 0; i < m_numAttributtes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		// Retain your custom loop disabling layout attribute bounds automatically
		for (int i = 0; i < m_numAttributtes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLProgram::dispose()
	{
		if (m_programID) {
			glDeleteProgram(m_programID);
			m_programID = 0;
		}
		m_numAttributtes = 0;
	}

	void GLSLProgram::compileShaders(const char* source, const std::string& name, GLuint& id) {
		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);

		GLint isCompiled = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(id); // Prevent graphics hardware memory allocation leaks

			logE << &(errorLog[0]) << std::endl;
			logE << "SHADER_SYSTEM: " << name << " failed to compile successfully!" << std::endl;
			Logger::exitGame();
		}
	}

	/* 🚀 MODERN 3D EXTENSION: MATRICES AND VECTOR DATA UNIFORM UPLOADERS */
	void GLSLProgram::setUniform(const std::string& name, float value) { glUniform1f(getUniformLocation(name), value); }
	void GLSLProgram::setUniform(const std::string& name, int value) { glUniform1i(getUniformLocation(name), value); }
	void GLSLProgram::setUniform(const std::string& name, const glm::vec2& vector) { glUniform2f(getUniformLocation(name), vector.x, vector.y); }
	void GLSLProgram::setUniform(const std::string& name, const glm::vec3& vector) { glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z); }
	void GLSLProgram::setUniform(const std::string& name, const glm::vec4& vector) { glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w); }

	void GLSLProgram::setUniform(const std::string& name, const glm::mat4& matrix)
	{
		// Safely unpack floating point matrix blocks straight into active GLSL vertex pipelines
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
