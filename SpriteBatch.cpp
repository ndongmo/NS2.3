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
	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint textureID, 
		GLubyte depth, const ColorRGBA8& color)
	{
		this->textureID = textureID;
		this->depth = depth;

		this->topLeft.color = color;
		this->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		this->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		this->bottomLeft.color = color;
		this->bottomLeft.setPosition(destRect.x, destRect.y);
		this->bottomLeft.setUV(uvRect.x, uvRect.y);

		this->bottomRight.color = color;
		this->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		this->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		this->topRight.color = color;
		this->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		this->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	Glyph::Glyph(const std::vector<glm::vec2>& points, const glm::vec4& uvRect, GLuint textureID, 
		GLubyte depth, const ColorRGBA8& color)
	{
		this->textureID = textureID;
		this->depth = depth;

		this->topLeft.color = color;
		this->topLeft.position = points[TL];
		this->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		this->bottomLeft.color = color;
		this->bottomLeft.position = points[BL];
		this->bottomLeft.setUV(uvRect.x, uvRect.y);

		this->bottomRight.color = color;
		this->bottomRight.position = points[BR];
		this->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		this->topRight.color = color;
		this->topRight.position = points[TR];
		this->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	void SpriteBatch::init(int screenWidth, int screenHeight)
	{
		// Compile color shader
		m_textureProgram.compileShaders("../Sprite.vert", "../Sprite.frag");
		m_textureProgram.addAttribute("vertexPosition");
		m_textureProgram.addAttribute("vertexColor");
		m_textureProgram.addAttribute("vertexUV");
		m_textureProgram.linkShaders();

		glGenVertexArrays(1, &m_vao); // generate vertex arrays
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo); // generate buffer object
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// this is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// this is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// this is the uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);

		// Generate FBO
		glGenFramebuffers(1, &m_lightMap.fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightMap.fbo);

		// Generate Texture to hold the lightmap data
		glGenTextures(1, &m_lightMap.textureID);
		glBindTexture(GL_TEXTURE_2D, m_lightMap.textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightMap.textureID, 0);

		// Unbind FBO back to default screen buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void SpriteBatch::dispose()
	{
		m_textureProgram.dispose();

		if (m_lightMap.textureID != 0) {
			glDeleteTextures(1, &m_lightMap.textureID);
			m_lightMap.textureID = 0;
		}
		if (m_lightMap.fbo != 0) {
			glDeleteFramebuffers(1, &m_lightMap.fbo);
			m_lightMap.fbo = 0;
		}
		if (m_vao != 0) {
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
		if (m_vbo != 0) {
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}
	}

	void SpriteBatch::beginLightScene(const glm::vec3& ambient, const glm::mat4& cameraMatrix, GlyphSortType sortType)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightMap.fbo);
		glClearColor(ambient.r, ambient.g, ambient.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);

		// Add lights while preserving light-map alpha.
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE);

		begin(cameraMatrix, sortType);
	}

	void SpriteBatch::beginNormalScene(const glm::vec3& backColor, const glm::mat4& cameraMatrix, GlyphSortType sortType)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Use transparent clear if framebuffer alpha is needed.
		glClearColor(backColor.x, backColor.y, backColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		begin(cameraMatrix, sortType);
	}

	void SpriteBatch::mergeScenes(const glm::mat4& cameraMatrix, const glm::vec4 screenBox)
	{
		// Multiply RGB, preserve scene alpha.
		glBlendFuncSeparate(GL_DST_COLOR, GL_ZERO, GL_ZERO, GL_ONE);

		begin(cameraMatrix, GlyphSortType::NONE);
		draw(screenBox, m_lightMap.uvRect, m_lightMap.depth, m_lightMap.textureID, m_lightMap.color);
		endScene();

		// Restore default blending mode for normal scene rendering.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void SpriteBatch::draw(const std::vector<glm::vec2>& points, const NS2::TileSheet& tileSheet)
	{
		m_glyphs.emplace_back(points, tileSheet.getUvRect(), tileSheet.texture->id, tileSheet.depth, tileSheet.color);
	}

	void SpriteBatch::draw(glm::vec4 destRect, glm::vec4 uvRect, GLubyte depth, GLuint textureID, ColorRGBA8 color)
	{
		m_glyphs.emplace_back(destRect, uvRect, textureID, depth, color);
	}

	void SpriteBatch::draw(const std::vector<glm::vec2>& points, glm::vec4 uvRect, GLubyte depth, GLuint textureID, ColorRGBA8 color)
	{
		m_glyphs.emplace_back(points, uvRect, textureID, depth, color);
	}

	void SpriteBatch::begin(const glm::mat4& cameraMatrix, GlyphSortType sortType)
	{
		m_textureProgram.use();
		m_textureProgram.setUniform("ns2TextureSampler", 0);
		m_textureProgram.setUniform("cameraMatrix", cameraMatrix);

		m_sortType = sortType;
		m_renderBatches.clear();
		m_glyphs.clear();
	}

	void SpriteBatch::endScene()
	{
		end();
		render();
		m_textureProgram.unuse();
	}

	void SpriteBatch::end()
	{
		m_glyphsPointers.resize(m_glyphs.size());

		for (size_t i = 0; i < m_glyphs.size(); i++) {
			m_glyphsPointers[i] = &m_glyphs[i];
		}

		sortGlyph();
		createRenderBatches();
	}

	void SpriteBatch::render()
	{
		glBindVertexArray(m_vao);

		for (size_t i = 0; i < m_renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].textureID);
			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		if (m_glyphsPointers.empty()) {
			return;
		}

		std::vector<Vertex> vertices(m_glyphsPointers.size() * 6);
		int offset = 0;
		int cv = 0; // current vertex

		m_renderBatches.emplace_back(offset, 6, m_glyphsPointers[0]->textureID);
		vertices[cv++] = m_glyphsPointers[0]->topLeft;
		vertices[cv++] = m_glyphsPointers[0]->bottomLeft;
		vertices[cv++] = m_glyphsPointers[0]->bottomRight;
		vertices[cv++] = m_glyphsPointers[0]->bottomRight;
		vertices[cv++] = m_glyphsPointers[0]->topRight;
		vertices[cv++] = m_glyphsPointers[0]->topLeft;
		offset += 6;

		for (size_t i = 1; i < m_glyphsPointers.size(); i++)
		{
			if (m_glyphsPointers[i]->textureID != m_glyphsPointers[i - 1]->textureID) {
				m_renderBatches.emplace_back(offset, 6, m_glyphsPointers[i]->textureID);
			}
			else {
				m_renderBatches.back().numVertices += 6;
			}

			vertices[cv++] = m_glyphsPointers[i]->topLeft;
			vertices[cv++] = m_glyphsPointers[i]->bottomLeft;
			vertices[cv++] = m_glyphsPointers[i]->bottomRight;
			vertices[cv++] = m_glyphsPointers[i]->bottomRight;
			vertices[cv++] = m_glyphsPointers[i]->topRight;
			vertices[cv++] = m_glyphsPointers[i]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::sortGlyph() {
		switch (m_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_glyphsPointers.begin(), m_glyphsPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_glyphsPointers.begin(), m_glyphsPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(m_glyphsPointers.begin(), m_glyphsPointers.end(), compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->textureID < b->textureID);
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->textureID > b->textureID);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
}
