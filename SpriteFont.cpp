/**
 * @file SpriteFont.cpp
 * @brief High-performance sprite font atlas builder and instanced batch text generator.
 * @author Ndongmo Silatsa Fabrice
 */

#include "SpriteFont.h"
#include "SpriteBatch.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>

 // Fast inline bit-shift power-of-two rounding calculation utility
inline int closestPow2(int i) {
	i--;
	int pi = 1;
	while (i > 0) {
		i >>= 1;
		pi <<= 1;
	}
	return pi;
}

#define MAX_TEXTURE_RES 4096 // must be always a power of two

namespace NS2
{
	SpriteFont::SpriteFont(SpriteFont&& other) noexcept
		: m_regStart(other.m_regStart), m_regLength(other.m_regLength),
		m_glyphs(std::move(other.m_glyphs)), m_fontHeight(other.m_fontHeight), m_texID(other.m_texID)
	{
		other.m_texID = 0;
		other.m_fontHeight = 0;
	}

	SpriteFont& SpriteFont::operator=(SpriteFont&& other) noexcept
	{
		if (this != &other) {
			dispose();
			m_regStart = other.m_regStart;
			m_regLength = other.m_regLength;
			m_glyphs = std::move(other.m_glyphs);
			m_fontHeight = other.m_fontHeight;
			m_texID = other.m_texID;
			other.m_texID = 0;
			other.m_fontHeight = 0;
		}
		return *this;
	}

	void SpriteFont::init(const char* fontPath, float size) {
		init(fontPath, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
	}

	void SpriteFont::init(const char* fontPath, float size, char cs, char ce) {
		if (!TTF_WasInit()) {
			TTF_Init();
		}

		TTF_Font* f = TTF_OpenFont(fontPath, size);
		if (f == nullptr) {
			fprintf(stderr, "Failed to open TTF font %s\n", fontPath);
			fflush(stderr);
			throw std::runtime_error("Font file missing or corrupted");
		}

		m_fontHeight = TTF_GetFontHeight(f);
		m_regStart = cs;
		m_regLength = ce - cs + 1;
		int padding = static_cast<int>(size / 8.0f);

		std::vector<glm::ivec4> glyphRects(m_regLength);
		int i = 0, advance;
		for (char c = cs; c <= ce; c++) {
			TTF_GetGlyphMetrics(f, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
			glyphRects[i].z -= glyphRects[i].x;
			glyphRects[i].x = 0;
			glyphRects[i].w -= glyphRects[i].y;
			glyphRects[i].y = 0;
			i++;
		}

		int rows = 1, w, h, bestWidth = 0, bestHeight = 0, bestRows = 0;
		int area = MAX_TEXTURE_RES * MAX_TEXTURE_RES;
		std::vector<std::vector<int>> bestPartition;
		bool partitionFound = false;

		while (rows <= m_regLength) {
			h = rows * (padding + m_fontHeight) + padding;
			std::vector<std::vector<int>> gr = createRows(glyphRects.data(), m_regLength, rows, padding, w);

			w = closestPow2(w);
			h = closestPow2(h);

			if (w > MAX_TEXTURE_RES || h > MAX_TEXTURE_RES) {
				rows++;
				continue;
			}

			if (area >= w * h) {
				bestPartition = std::move(gr);
				bestWidth = w;
				bestHeight = h;
				bestRows = rows;
				area = bestWidth * bestHeight;
				partitionFound = true;
				rows++;
			}
			else {
				break;
			}
		}

		if (!partitionFound) {
			fprintf(stderr, "Failed to Map TTF font %s to texture map coordinate blocks.\n", fontPath);
			fflush(stderr);
			TTF_CloseFont(f);
			throw std::runtime_error("Texture sheet limit size breached");
		}

		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		SDL_Color fg = { 255, 255, 255, 255 };
		int ly = padding;
		for (int ri = 0; ri < bestRows; ri++) {
			int lx = padding;
			for (size_t ci = 0; ci < bestPartition[ri].size(); ci++) {
				int gi = bestPartition[ri][ci];

				SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(f, static_cast<char>(cs + gi), fg);

				// Fast local pixel channel alpha pre-multiplication passes
				unsigned char* sp = static_cast<unsigned char*>(glyphSurface->pixels);
				int cp = glyphSurface->w * glyphSurface->h * 4;
				for (int pIdx = 0; pIdx < cp; pIdx += 4) {
					float alphaMult = sp[pIdx + 3] / 255.0f;
					sp[pIdx] = static_cast<unsigned char>(sp[pIdx] * alphaMult);
					sp[pIdx + 1] = sp[pIdx];
					sp[pIdx + 2] = sp[pIdx];
				}

				glPixelStorei(GL_UNPACK_ROW_LENGTH, glyphSurface->pitch / 4);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

				glTexSubImage2D(GL_TEXTURE_2D, 0, lx, bestHeight - ly - 1 - glyphSurface->h,
					glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);

				glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

				glyphRects[gi].x = lx;
				glyphRects[gi].y = ly;
				glyphRects[gi].z = glyphSurface->w;
				glyphRects[gi].w = glyphSurface->h;

				SDL_DestroySurface(glyphSurface);
				lx += glyphRects[gi].z + padding;
			}
			ly += m_fontHeight + padding;
		}

		int rs = padding - 1;
		if (rs < 1) rs = 1;
		std::vector<int> pureWhiteSquare(rs * rs, 0xffffffff);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Safe layout vector population without memory tracking overheads
		m_glyphs.resize(m_regLength + 1);
		for (int idx = 0; idx < m_regLength; idx++) {
			m_glyphs[idx].character = static_cast<char>(cs + idx);
			m_glyphs[idx].size = glm::vec2(glyphRects[idx].z, glyphRects[idx].w);
			m_glyphs[idx].uvRect = glm::vec4(
				static_cast<float>(glyphRects[idx].x) / bestWidth,
				static_cast<float>(glyphRects[idx].y) / bestHeight,
				static_cast<float>(glyphRects[idx].z) / bestWidth,
				static_cast<float>(glyphRects[idx].w) / bestHeight
			);
		}

		m_glyphs[m_regLength].character = ' ';
		m_glyphs[m_regLength].size = m_glyphs[0].size;
		m_glyphs[m_regLength].uvRect = glm::vec4(0.0f, 0.0f, static_cast<float>(rs) / bestWidth, static_cast<float>(rs) / bestHeight);

		glBindTexture(GL_TEXTURE_2D, 0);
		TTF_CloseFont(f);
	}

	void SpriteFont::dispose() {
		if (m_texID != 0) {
			glDeleteTextures(1, &m_texID);
			m_texID = 0;
		}
		m_glyphs.clear();
	}

	std::vector<std::vector<int>> SpriteFont::createRows(const glm::ivec4* rects, int rectsLength, int r, int padding, int& w) {
		std::vector<std::vector<int>> l(r);
		std::vector<int> cw(r, padding);

		for (int idx = 0; idx < rectsLength; idx++) {
			int ri = 0;
			for (int rii = 1; rii < r; rii++) {
				if (cw[rii] < cw[ri]) ri = rii;
			}
			cw[ri] += rects[idx].z + padding;
			l[ri].push_back(idx);
		}

		w = 0;
		for (int idx = 0; idx < r; idx++) {
			if (cw[idx] > w) w = cw[idx];
		}
		return l;
	}

	glm::vec2 SpriteFont::measure(std::string_view s) const {
		glm::vec2 size(0.0f, static_cast<float>(m_fontHeight));
		float cw = 0.0f;

		for (size_t si = 0; si < s.length(); si++) {
			char c = s[si];
			if (c == '\n') {
				size.y += m_fontHeight;
				if (size.x < cw) size.x = cw;
				cw = 0.0f;
			}
			else {
				int gi = c - m_regStart;
				if (gi < 0 || gi >= m_regLength) gi = m_regLength;
				cw += m_glyphs[gi].size.x;
			}
		}
		if (size.x < cw) size.x = cw;
		return size;
	}

	void SpriteFont::justify(std::string_view s, glm::vec2& position, glm::vec2 scaling, Justification just) const
	{
		glm::vec2 m = measure(s);
		switch (just) {
		case Justification::MIDDLE:
			position.x -= (m.x * scaling.x) / 2.0f;
			position.y -= (m.y * scaling.y) / 2.0f;
			break;
		case Justification::RIGHT:
			position.x -= (m.x * scaling.x);
			position.y -= (m.y * scaling.y);
			break;
		case Justification::MIDDLEX:
			position.x -= (m.x * scaling.x) / 2.0f;
			break;
		case Justification::RIGHTX:
			position.x -= (m.x * scaling.x);
			break;
		case Justification::MIDDLEY:
			position.y -= (m.y * scaling.y) / 2.0f;
			break;
		case Justification::RIGHTY:
			position.y -= (m.y * scaling.y);
			break;
		default: break;
		}
	}

	void SpriteFont::justify(std::string_view s, glm::vec4& destRect, Justification just) const
	{
		glm::vec2 m = measure(s);
		switch (just) {
		case Justification::MIDDLE:
			destRect.x -= (m.x * destRect.z) / 2.0f;
			destRect.y -= (m.y * destRect.w) / 2.0f;
			break;
		case Justification::RIGHT:
			destRect.x -= (m.x * destRect.z);
			destRect.y -= (m.y * destRect.w);
			break;
		case Justification::MIDDLEX:
			destRect.x -= (m.x * destRect.z) / 2.0f;
			break;
		case Justification::RIGHTX:
			destRect.x -= (m.x * destRect.z);
			break;
		case Justification::MIDDLEY:
			destRect.y -= (m.y * destRect.w) / 2.0f;
			break;
		case Justification::RIGHTY:
			destRect.y -= (m.y * destRect.w);
			break;
		default: break;
		}
	}

	void SpriteFont::draw(SpriteBatch& batch, std::string_view s, glm::vec2 position, glm::vec2 scaling,
		GLubyte depth, ColorRGBA8 tint, Justification just)
	{
		justify(s, position, scaling, just);

		float startX = position.x;
		for (size_t si = 0; si < s.length(); si++) {
			char c = s[si];
			if (c == '\n') {
				position.x = startX;
				position.y -= m_fontHeight * scaling.y;
			}
			else {
				int gi = c - m_regStart;
				if (gi < 0 || gi >= m_regLength) gi = m_regLength;

				const auto& glyph = m_glyphs[gi];
				glm::vec4 destRect(position.x, position.y, glyph.size.x * scaling.x, glyph.size.y * scaling.y);

				batch.draw(destRect, glyph.uvRect, depth, m_texID, tint);

				position.x += glyph.size.x * scaling.x;
			}
		}
	}

	void SpriteFont::draw(SpriteBatch& batch, std::string_view s, const glm::vec4& destRect,
		GLubyte depth, ColorRGBA8 tint, Justification just)
	{
		// Map structural vec4 bounds envelope dimensions mapping cleanly
		glm::vec4 adjustedRect = destRect;
		justify(s, adjustedRect, just);

		glm::vec2 position(adjustedRect.x, adjustedRect.y);
		glm::vec2 scaling(adjustedRect.z, adjustedRect.w);

		float startX = position.x;
		for (size_t si = 0; si < s.length(); si++) {
			char c = s[si];
			if (c == '\n') { 
				position.x = startX; 
				position.y -= m_fontHeight * scaling.y; 
			}
			else { 
				int gi = c - m_regStart; 
				if (gi < 0 || gi >= m_regLength) gi = m_regLength; 

				const auto& glyph = m_glyphs[gi]; 
				glm::vec4 glyphDest(position.x, position.y, glyph.size.x * scaling.x, glyph.size.y * scaling.y); 

				batch.draw(glyphDest, glyph.uvRect, depth, m_texID, tint); 

				position.x += glyph.size.x * scaling.x; 
			}
		}
	}
}