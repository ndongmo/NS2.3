/*
This is a modified version of the SpriteFont class from the
Seed Of Andromeda source code.
Use it for any of your projects, commercial or otherwise,
free of charge, but do not remove this disclaimer.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS
ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
OF THIS SOFTWARE.

November 28 2014
Original Author: Cristian Zaloj
Modified By: Benjamin Arnold

October 27 2016
Modified By: Fabrice Ndongmo
*/

#pragma once

#include "CoreTypes.h"
#include <vector>
#include <string_view>
#include <glm/glm.hpp>

namespace NS2
{
	class SpriteBatch;

	constexpr char FIRST_PRINTABLE_CHAR = 32;
	constexpr char LAST_PRINTABLE_CHAR = 126;

	/**
	 * @enum Justification
	 * @brief Text justification configurations applied to screen text boundaries.
	 */
	enum Justification
	{
		LEFT, MIDDLE, RIGHT, MIDDLEX, RIGHTX, MIDDLEY, RIGHTY
	};

	class SpriteFont {
	public:
		SpriteFont() = default;
		~SpriteFont() { dispose(); }

		// Rule of 5: Prevent accidental shallow copy replication of internal GPU texture handles
		SpriteFont(const SpriteFont&) = delete;
		SpriteFont& operator=(const SpriteFont&) = delete;

		// Robust move semantics to seamlessly transfer font assets between resources
		SpriteFont(SpriteFont&& other) noexcept;
		SpriteFont& operator=(SpriteFont&& other) noexcept;

		/* INITIALIZERS */
		void init(const char* fontPath, float size);
		void init(const char* fontPath, float size, char cs, char ce);

		/**
		 * @brief Safely releases the hardware font texture asset and internal allocations.
		 */
		void dispose();

		int getFontHeight() const { return m_fontHeight; }

		/**
		 * @brief Measures the total visual pixel width and height boundaries of a given text view string.
		 */
		glm::vec2 measure(std::string_view s) const;

		/**
		 * @brief Adjusts position anchors relative to text metrics before layout generation.
		 */
		void justify(std::string_view s, glm::vec2& position, glm::vec2 scaling, Justification just) const;
		void justify(std::string_view s, glm::vec4& destRect, Justification just) const;

		/**
		 * @brief Batches text characters down to the highly compressed 36-byte instanced graphics pipeline.
		 */
		void draw(SpriteBatch& batch, std::string_view s, glm::vec2 position, glm::vec2 scaling,
			GLubyte depth, ColorRGBA8 tint, Justification just = Justification::LEFT);

		void draw(SpriteBatch& batch, std::string_view s, const glm::vec4& destRect,
			GLubyte depth, ColorRGBA8 tint, Justification just = Justification::LEFT);

	private:
		/**
		 * @brief Replaces the old heap-allocated vector pointer to completely protect against leaks.
		 */
		static std::vector<std::vector<int>> createRows(const glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

		int m_regStart = 0;
		int m_regLength = 0;
		int m_fontHeight = 0;
		unsigned int m_texID = 0;
		std::vector<CharGlyph> m_glyphs;
	};
}
