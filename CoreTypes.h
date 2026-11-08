#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <algorithm>
#include <limits>

namespace NS2
{
	/*!
	 * \brief Positions of corner points.
	 */
	enum Corner
	{
		BL = 0,		// Bottom left
		BR = 1,		// Bottom right
		TR = 2,		// Top right
		TL = 3		// Top left
	};

	/**
	 * @struct GLTexture
	 * @brief Plain-old-data structure wrapping an OpenGL hardware texture handle and its dimensions.
	 */
	struct GLTexture
	{
		std::string filePath; /**< Local file path tracking origin location disk points */
		GLuint id = 0;        /**< Raw OpenGL hardware texture handle identifier (0 indicates uninitialized) */
		int width = 0;        /**< Width dimension of the image in pixels */
		int height = 0;       /**< Height dimension of the image in pixels */
	};

	/**
	 * @struct CharGlyph
	 * @brief Stores structural size and UV mapping coordinates for a font character.
	 */
	struct CharGlyph {
		char character = 0;					/**< The exact ASCII char identifier this glyph tracks */
		glm::vec4 uvRect = glm::vec4(0.0f);	/**< Texture clipping coordinates (U, V, Width, Height) */
		glm::vec2 size = glm::vec2(0.0f);   /**< True width and height dimensional constraints of the letter */
	};

	/*!
	* \brief Color structure.
	*/
	struct  ColorRGBA8 {
		ColorRGBA8() : r(255), g(255), b(255), a(255) {}
		ColorRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : r(r), g(g), b(b), a(a) {}
		void setColor(GLubyte rr, GLubyte gg, GLubyte bb, GLubyte aa) {
			r = rr;
			g = gg;
			b = bb;
			a = aa;
		}

		void incrColor(int rr, int gg, int bb, int aa) {
			r = static_cast<GLubyte>(std::clamp(static_cast<int>(r) + rr, 0, 255));
			g = static_cast<GLubyte>(std::clamp(static_cast<int>(g) + gg, 0, 255));
			b = static_cast<GLubyte>(std::clamp(static_cast<int>(b) + bb, 0, 255));
			a = static_cast<GLubyte>(std::clamp(static_cast<int>(a) + aa, 0, 255));
		}

		GLubyte r = 0;
		GLubyte g = 0;
		GLubyte b = 0;
		GLubyte a = 0;
	};

	/*!
	* \brief Simple 2D Vertex structure.
	*/
	struct Vertex
	{
		glm::vec2 position;	// Vertex position
		glm::vec2 uv;		// UV texture coordinates
		ColorRGBA8 color;	// Vertex color

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v) {
			uv.x = u;
			uv.y = v;
		}
	};
}
