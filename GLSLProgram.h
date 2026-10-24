/**
 * @file GLSLProgram.h
 * @brief High-performance OpenGL Shading Language (GLSL) program coordinator.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated for NS2 Architecture (3D Shader Upgrades)
 * @version 2.0
 */

#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace NS2
{
	class GLSLProgram
	{
	public:
		GLSLProgram() = default;
		~GLSLProgram() { dispose(); }

		// Enforce unique ownership over native GPU shader program allocations
		GLSLProgram(const GLSLProgram&) = delete;
		GLSLProgram& operator=(const GLSLProgram&) = delete;

		/**
		 * @brief Loads and compiles a shader pipeline from source files on disk.
		 * @param vertexShaderFilePath Path to the vertex shader source code file.
		 * @param fragmentShaderFilePath Path to the fragment shader source code file.
		 */
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		/**
		 * @brief Compiles a shader pipeline directly from raw in-memory character strings.
		 */
		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

		/** @brief Registers an explicit input layout attribute location slot. */
		void addAttribute(const std::string& attributeName);

		/** @brief Links the compiled shader objects together into an active executable program. */
		void linkShaders();

		/** @brief Binds this shader program to the active OpenGL rendering context state. */
		void use();

		/** @brief Unbinds the active shader program from the OpenGL state machine. */
		void unuse();

		/** @brief Completely deletes the allocated program object and shaders from GPU memory. */
		void dispose();

		/** @brief Queries the hardware memory location of a uniform layout variable. */
		GLint getUniformLocation(const std::string& uniformName);

		/* 🚀 MODERN 3D UNIFORM MATRIX EXTENSIONS */
		void setUniform(const std::string& name, float value);
		void setUniform(const std::string& name, int value);
		void setUniform(const std::string& name, const glm::vec2& vector);
		void setUniform(const std::string& name, const glm::vec3& vector);
		void setUniform(const std::string& name, const glm::vec4& vector);
		void setUniform(const std::string& name, const glm::mat4& matrix); /**< Direct Camera3D Matrix Upload Hook */

		/* GETTERS */
		GLuint getProgramID() const { return m_programID; }

	private:
		/** @brief Low-level core compiler loop compiling independent hardware shader stages. */
		void compileShaders(const char* source, const std::string& name, GLuint& id);

		int m_numAttributtes = 0;      /**< Tracks registered active layout input locations */
		GLuint m_programID = 0;        /**< Unique OpenGL driver program tracking token */
		GLuint m_vertexShaderID = 0;   /**< Unique OpenGL vertex stage tracking token */
		GLuint m_fragmentShaderID = 0; /**< Unique OpenGL fragment stage tracking token */
	};
}
