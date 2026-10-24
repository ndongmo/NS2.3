/**
 * @file SpriteBatch.h
 * @brief High-performance hardware-instanced batch renderer for 2D sprites.
 *        Utilizes CPU-side depth sorting with a compressed 36-byte GPU streaming layout.
 * @author Ndongmo Silatsa Fabrice
 */

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "CoreTypes.h"
# include "TileSheet.h"

namespace NS2
{
	enum class GlyphSortType { NONE, TEXTURE };

	/**
	 * @struct SpriteInstanceData
	 * @brief Highly compressed per-instance array payload streamed directly to the GPU.
	 * Total size: Exactly 36 bytes (perfectly aligned to 4-byte boundaries).
	 */
	struct SpriteInstanceData {
		glm::vec4 destRect = glm::vec4(0.0f);			// 16 bytes (x, y, width, height)
		glm::vec4 uvRect = glm::vec4(0.0f);				// 16 bytes (u, v, uWidth, vHeight)
		ColorRGBA8 color = ColorRGBA8();				// 4 bytes  (r, g, b, a)
	};

	class SpriteBatch
	{
	public:
		SpriteBatch() = default;
		~SpriteBatch() { dispose(); }

		// Enforce unique system resource handling rules by banning copy operations
		SpriteBatch(const SpriteBatch&) = delete;
		SpriteBatch& operator=(const SpriteBatch&) = delete;

		/** @brief Allocates the hardware drawing buffers and static quad templates. */
		void init();

		/** @brief Cleans up internal GPU handles and tracking vector pools. */
		void dispose();

		/** @brief Prepares structural tracking contexts for a fresh recording sweep. */
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

		/**
		 * @brief Pushes a sprite instance into the submission sorting queue.
		 */
		void draw(const std::vector<glm::vec2>& points, const NS2::TileSheet& tileSheet);
		void draw(glm::vec4 destRect, glm::vec4 uvRect, GLubyte depth, GLuint textureID, ColorRGBA8 color);

		/** @brief Processes multi-tier sorting algorithms and transfers instanced streams to VRAM. */
		void end();

		/** @brief Iterates over draw calls to execute fast instanced multi-pass drawing sweeps. */
		void render();

	private:
		struct RenderElement {
			GLuint textureID;
			GLubyte depth; // Preserved purely on CPU for Painter's Algorithm sorting layers
			SpriteInstanceData instanceData;
		};

		struct InstancedDrawCall {
			GLuint textureID;
			GLuint baseInstanceIndex;
			GLuint instanceCount;
		};

		void createStaticQuadVAO();

		GLuint m_quadVAO = 0;
		GLuint m_quadVBO = 0;
		GLuint m_quadIBO = 0;
		GLuint m_instanceVBO = 0;

		GlyphSortType m_sortType = GlyphSortType::TEXTURE;

		std::vector<RenderElement> m_submissionQueue;
		std::vector<SpriteInstanceData> m_instancedData;
		std::vector<InstancedDrawCall> m_drawCalls;
	};
}
