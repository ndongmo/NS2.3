/**
 * @file SpriteBatch.cpp
 * @brief High-performance hardware instanced 2D sprite batching pipeline implementation.
 * @author Ndongmo Silatsa Fabrice
 */

#include "SpriteBatch.h"
#include <algorithm>
#include <cstddef>

namespace NS2
{
	void SpriteBatch::init()
	{
		createStaticQuadVAO();

		// Generate the streaming instance buffer for instanced attributes
		glGenBuffers(1, &m_instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

		size_t initialCapacity = 1000 * sizeof(SpriteInstanceData);
		glBufferData(GL_ARRAY_BUFFER, initialCapacity, nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Optimize memory allocations to prevent mid-frame vector reallocation stalls
		m_submissionQueue.reserve(1000);
		m_instancedData.reserve(1000);
		m_drawCalls.reserve(64);
	}

	void SpriteBatch::dispose()
	{
		if (m_instanceVBO != 0) { glDeleteBuffers(1, &m_instanceVBO); m_instanceVBO = 0; }
		if (m_quadVBO != 0) { glDeleteBuffers(1, &m_quadVBO);    m_quadVBO = 0; }
		if (m_quadIBO != 0) { glDeleteBuffers(1, &m_quadIBO);    m_quadIBO = 0; }
		if (m_quadVAO != 0) { glDeleteVertexArrays(1, &m_quadVAO); m_quadVAO = 0; }

		m_submissionQueue.clear();
		m_instancedData.clear();
		m_drawCalls.clear();
	}

	void SpriteBatch::begin(GlyphSortType sortType)
	{
		m_sortType = sortType;
		m_submissionQueue.clear();
		m_drawCalls.clear();
		m_instancedData.clear();
	}

	void SpriteBatch::draw(const std::vector<glm::vec2>& points, const NS2::TileSheet& tileSheet)
	{
		SpriteInstanceData data;
		data.destRect = glm::vec4(points[BL].x, points[BL].y, points[BR].x - points[BL].x, points[TL].y - points[BL].y);
		data.uvRect = tileSheet.getUvRect();
		data.color = tileSheet.color;

		m_submissionQueue.push_back({ tileSheet.texture->id, tileSheet.depth, data });
	}

	void SpriteBatch::draw(glm::vec4 destRect, glm::vec4 uvRect, GLubyte depth, GLuint textureID, ColorRGBA8 color)
	{
		SpriteInstanceData data;
		data.destRect = destRect;
		data.uvRect = uvRect;
		data.color = color;

		m_submissionQueue.push_back({ textureID, depth, data });
	}

	void SpriteBatch::end()
	{
		if (m_submissionQueue.empty()) return;

		if (m_sortType == GlyphSortType::TEXTURE) {
			std::sort(m_submissionQueue.begin(), m_submissionQueue.end(),
				[](const RenderElement& a, const RenderElement& b) {
					if (a.depth != b.depth) {
						return a.depth < b.depth;
					}
					return a.textureID < b.textureID;
				});
		}

		GLuint currentTextureID = m_submissionQueue[0].textureID;
		GLuint baseIndex = 0;
		GLuint currentCount = 0;

		for (const auto& element : m_submissionQueue)
		{
			if (element.textureID == currentTextureID) {
				currentCount++;
			}
			else {
				m_drawCalls.push_back({ currentTextureID, baseIndex, currentCount });
				currentTextureID = element.textureID;
				baseIndex = static_cast<GLuint>(m_instancedData.size());
				currentCount = 1;
			}
			m_instancedData.push_back(element.instanceData);
		}
		m_drawCalls.push_back({ currentTextureID, baseIndex, currentCount });

		glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, m_instancedData.size() * sizeof(SpriteInstanceData), nullptr, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_instancedData.size() * sizeof(SpriteInstanceData), m_instancedData.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::render()
	{
		if (m_drawCalls.empty()) return;

		glBindVertexArray(m_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

		GLsizei stride = sizeof(SpriteInstanceData);

		// Layout Attribute Bindings
		// Slot 1: destRect (vec4) -> Per-instance configurations
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(SpriteInstanceData, destRect));
		glVertexAttribDivisor(1, 1);

		// Slot 2: uvRect (vec4) -> Per-instance configurations
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(SpriteInstanceData, uvRect));
		glVertexAttribDivisor(2, 1);

		// Slot 3: color (RGBAubyte) -> Normalized to floating-point representation vectors
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)offsetof(SpriteInstanceData, color));
		glVertexAttribDivisor(3, 1);

		GLuint lastBoundTextureID = 0xFFFFFFFF;
		glActiveTexture(GL_TEXTURE0);

		for (const auto& call : m_drawCalls)
		{
			if (call.textureID != lastBoundTextureID) {
				glBindTexture(GL_TEXTURE_2D, call.textureID);
				lastBoundTextureID = call.textureID;
			}

			// High-performance instanced base instance call matching your MeshRenderer architecture
			glDrawElementsInstancedBaseInstance(
				GL_TRIANGLES,
				6, // Shared static template quad has exactly 6 indices
				GL_UNSIGNED_INT,
				nullptr,
				call.instanceCount,
				call.baseInstanceIndex
			);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createStaticQuadVAO()
	{
		// Simple normalized reference quad vertices (Position.xy, UV.uv)
		float quadVertices[] = {
			0.0f, 1.0f,  0.0f, 1.0f, // Top Left
			0.0f, 0.0f,  0.0f, 0.0f, // Bottom Left
			1.0f, 0.0f,  1.0f, 0.0f, // Bottom Right
			1.0f, 1.0f,  1.0f, 1.0f  // Top Right
		};

		unsigned int quadIndices[] = { 0, 1, 2, 2, 3, 0 };

		glGenVertexArrays(1, &m_quadVAO);
		glGenBuffers(1, &m_quadVBO);
		glGenBuffers(1, &m_quadIBO);

		glBindVertexArray(m_quadVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

		// Shared static layout parameters (Evaluated per-vertex: divisor = 0)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

		glBindVertexArray(0);
	}
}
