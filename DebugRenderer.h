/*!
* \file DebugRenderer.h
* \brief Debug renderer. It's used to render object shape.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include "CoreTypes.h"
#include "GLSLProgram.h"

#include <vector>
#include <glm\glm.hpp>

namespace NS2
{
	class DebugRenderer
	{
	public:
		DebugRenderer();
		~DebugRenderer();

		/*!
		* \brief Initialize renderer stuff.
		*/
		void init();

		/*!
		* \brief Draw line.
		* \param a: begin line point
		* \param b: end line point
		* \param color: line color
		*/
		void drawLine(glm::vec2 a, glm::vec2 b, ColorRGBA8 color);
		/*!
		* \brief Draw polygon (set of points).
		* \param points: set of points
		* \param color: line color
		*/
		void drawPolygon(const std::vector<glm::vec2>& points, ColorRGBA8 color);
		/*!
		* \brief Draw Box.
		* \param destRect: destination rectangle coordinates
		* \param color: line color
		* \param angle:
		*/
		void drawBox(glm::vec4 destRect, ColorRGBA8 color);
		/*!
		* \brief Draw Circle.
		* \param center: circle center coordinates
		* \param color: line color
		* \param radius:
		*/
		void drawCircle(glm::vec2 center, ColorRGBA8 color, float radius);
		/*!
		* \brief Render all shapes.
		* \param projectionMatrix: projection matrix
		* \param lineWith:
		*/
		void render(const glm::mat4& projectionMatrix, float lineWith);
		/*!
		* \brief Free all resources.
		*/
		void dispose();

		/*!
		 * \brief Debug vertex structure.
		 */
		struct DebugVertex
		{
			glm::vec2 position = glm::vec2(0.0f);
			ColorRGBA8 color = ColorRGBA8();
		};

	private:
		GLSLProgram m_program;						/* GLSL program */
		std::vector<DebugVertex> m_vertices;		/* Debug vertice */
		std::vector<GLuint> m_indices;				/* Indices of debug vertice */
		GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;		/* ID stuffs */
		int m_numElements = 0;						/* Number of elements to draw */
	};
}