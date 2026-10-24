#include "DebugRenderer.h"
#include <cmath>

namespace NS2
{
	constexpr float PI = 3.14159265359f;

	const char* FRAGMENT_SRC = R"(#version 130
		in vec4 fragmentColor;
		in vec2 fragmentPosition;
		out vec4 color;
		void main() {	
			color = fragmentColor;
		})";

	const char* VERTEX_SRC = R"(#version 130
		in vec2 vertexPosition;
		in vec4 vertexColor;
		out vec4 fragmentColor;
		out vec2 fragmentPosition;
		uniform mat4 cameraMatrix;
		void main() {
			// FIXED: Compute natural transformed clip-coordinates using the uniform projection matrix mapping
			gl_Position = cameraMatrix * vec4(vertexPosition, 0.0, 1.0);
			fragmentColor = vertexColor;
			fragmentPosition = vertexPosition;
		})";

	DebugRenderer::DebugRenderer() {}

	DebugRenderer::~DebugRenderer()
	{
		dispose();
	}

	void DebugRenderer::init()
	{
		m_program.compileShadersFromSource(VERTEX_SRC, FRAGMENT_SRC);
		m_program.addAttribute("vertexPosition");
		m_program.addAttribute("vertexColor");
		m_program.linkShaders();

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
		// PERFECT: ColorRGBA8 perfectly maps to normalized unsigned bytes
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

		glBindVertexArray(0);
	}

	void DebugRenderer::end()
	{
		if (m_vertices.empty()) return;

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(DebugVertex), m_vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_numElements = static_cast<int>(m_indices.size());
		m_indices.clear();
		m_vertices.clear();
	}

	glm::vec2 rotatePoint(glm::vec2 position, float angle)
	{
		float cosA = std::cos(angle);
		float sinA = std::sin(angle);
		return glm::vec2(
			position.x * cosA - position.y * sinA,
			position.x * sinA + position.y * cosA
		);
	}

	void DebugRenderer::drawLine(glm::vec2 a, glm::vec2 b, ColorRGBA8 color) {
		size_t i = m_vertices.size();
		m_vertices.resize(i + 2);

		m_vertices[i].position.x = a.x;
		m_vertices[i].position.y = a.y;
		m_vertices[i].color = color;

		m_vertices[i + 1].position.x = b.x;
		m_vertices[i + 1].position.y = b.y;
		m_vertices[i + 1].color = color;

		m_indices.push_back(static_cast<GLuint>(i));
		m_indices.push_back(static_cast<GLuint>(i + 1));
	}

	void DebugRenderer::drawPolygon(const std::vector<glm::vec2>& points, ColorRGBA8 color)
	{
		size_t start = m_vertices.size();
		size_t size = points.size();
		if (size < 2) return;

		m_vertices.resize(start + size);

		for (size_t i = 0; i < size; i++)
		{
			m_vertices[start + i].position.x = points[i].x;
			m_vertices[start + i].position.y = points[i].y;
			m_vertices[start + i].color = color;
		}

		m_indices.reserve(m_indices.size() + size * 2);
		for (size_t i = 0; i < size - 1; i++)
		{
			m_indices.push_back(static_cast<GLuint>(start + i));
			m_indices.push_back(static_cast<GLuint>(start + i + 1));
		}

		m_indices.push_back(static_cast<GLuint>(start + size - 1));
		m_indices.push_back(static_cast<GLuint>(start));
	}

	void DebugRenderer::drawBox(glm::vec4 destRect, ColorRGBA8 color)
	{
		size_t i = m_vertices.size();
		m_vertices.resize(i + 4);

		m_vertices[i].position.x = destRect.x;
		m_vertices[i].position.y = destRect.y + destRect.w;

		m_vertices[i + 1].position.x = destRect.x;
		m_vertices[i + 1].position.y = destRect.y;

		m_vertices[i + 2].position.x = destRect.x + destRect.z;
		m_vertices[i + 2].position.y = destRect.y;

		m_vertices[i + 3].position.x = destRect.x + destRect.z;
		m_vertices[i + 3].position.y = destRect.y + destRect.w;

		for (size_t j = i; j < i + 4; j++)
		{
			m_vertices[j].color = color;
		}

		m_indices.reserve(m_indices.size() + 8);

		GLuint idx = static_cast<GLuint>(i);
		m_indices.push_back(idx);     m_indices.push_back(idx + 1);
		m_indices.push_back(idx + 1); m_indices.push_back(idx + 2);
		m_indices.push_back(idx + 2); m_indices.push_back(idx + 3);
		m_indices.push_back(idx + 3); m_indices.push_back(idx);
	}

	void DebugRenderer::drawCircle(glm::vec2 center, ColorRGBA8 color, float radius)
	{
		// FIXED: Set safe floor bounds for vertices allocation limit layout
		int numVertices = static_cast<int>(radius * 4.0f);
		if (numVertices < 6) numVertices = 6;
		if (numVertices > 100) numVertices = 100;

		size_t start = m_vertices.size();
		m_vertices.resize(start + numVertices);

		for (int i = 0; i < numVertices; i++)
		{
			float angle = (static_cast<float>(i) / numVertices) * PI * 2.0f;
			m_vertices[start + i].position.x = std::cos(angle) * radius + center.x;
			m_vertices[start + i].position.y = std::sin(angle) * radius + center.y;
			m_vertices[start + i].color = color;
		}

		m_indices.reserve(m_indices.size() + numVertices * 2);
		for (int i = 0; i < numVertices - 1; i++)
		{
			m_indices.push_back(static_cast<GLuint>(start + i));
			m_indices.push_back(static_cast<GLuint>(start + i + 1));
		}

		m_indices.push_back(static_cast<GLuint>(start + numVertices - 1));
		m_indices.push_back(static_cast<GLuint>(start));
	}

	void DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth)
	{
		if (m_numElements == 0) return;

		m_program.use();

		GLint cmLocation = m_program.getUniformLocation("cameraMatrix");
		glUniformMatrix4fv(cmLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

		glLineWidth(lineWidth);
		glBindVertexArray(m_vao);
		glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		m_program.unuse();
	}

	void DebugRenderer::dispose()
	{
		if (m_vao != 0) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
		if (m_vbo != 0) { glDeleteBuffers(1, &m_vbo); m_vbo = 0; }
		if (m_ibo != 0) { glDeleteBuffers(1, &m_ibo); m_ibo = 0; }
		m_program.dispose();
	}
}
