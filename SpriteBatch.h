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
#include "TileSheet.h"
#include "GLSLProgram.h"

namespace NS2
{
	enum class GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE };

	/*!
	* \brief Glypg : Single quad.
	*/
	class Glyph
	{
	public:
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
			GLuint textureID, GLubyte depth, const ColorRGBA8& color);

		Glyph(const std::vector<glm::vec2>& points, const glm::vec4& uvRect,
			GLuint textureID, GLubyte depth, const ColorRGBA8& color);

		GLuint textureID;	/* Glyph texture ID */
		GLubyte depth;		/* Glyph deth */
		Vertex topLeft;		/* Glyph top left corner */
		Vertex bottomLeft;	/* Glyph bottom left corner */
		Vertex bottomRight;	/* Glyph bottom right corner */
		Vertex topRight;	/* Glyph top right corner */
	};

	/*!
	* \brief Used to draw a texture. Each render batch is uded for a single draw call.
	*/
	class RenderBatch
	{
	public:
		RenderBatch(GLuint offset, GLuint numVertices, GLuint textureID) :
			offset(offset),
			numVertices(numVertices),
			textureID(textureID)
		{};
		GLuint offset;			/* offset value */
		GLuint numVertices;		/* nomber of vertices */
		GLuint textureID;		/* texture ID */
	};

	/*!
	 * \brief LightMap : Used to store light map data.
	 */
	struct LightMap
	{
		glm::vec4 uvRect{ 0.0f, 0.0f, 1.0f, -1.0f };	/* UV coordinates */
		ColorRGBA8 color{ 255, 255, 255, 255 };			/* color value */
		GLuint depth = 0;								/* depth value */
		GLuint fbo;										/* frame buffer object */
		GLuint textureID;								/* texture ID */
	};

	class SpriteBatch
	{
	public:
		SpriteBatch() = default;
		~SpriteBatch() { dispose(); }

		SpriteBatch(const SpriteBatch&) = delete;
		SpriteBatch& operator=(const SpriteBatch&) = delete;

		/** 
		 * @brief Allocates the hardware drawing buffers and static quad templates. 
		 */
		void init(int screenWidth, int screenHeight);

		/** 
		 * @brief Cleans up internal GPU handles and tracking vector pools. 
		 */
		void dispose();

		/** 
		 * @brief Begins rendering a light scene.
		 */
		void beginLightScene(const glm::vec3& ambient, const glm::mat4& cameraMatrix, 
			GlyphSortType sortType = GlyphSortType::TEXTURE);

		/**
		 * @brief Begins rendering a normal scene.
		 */
		void beginNormalScene(const glm::vec3& backColor, const glm::mat4& cameraMatrix, 
			GlyphSortType sortType = GlyphSortType::TEXTURE);

		/**
		 * @brief Merges light scene with normal scene into a single rendering pass.
		 */
		void mergeScenes(const glm::mat4& cameraMatrix, const glm::vec4 screenBox);

		/**
		 * @brief Ends the current scene and flushes all draw calls to the GPU.
		 */
		void endScene();

		/**
		 * @brief Pushes a sprite instance into the submission sorting queue.
		 */
		void draw(const std::vector<glm::vec2>& points, const NS2::TileSheet& tileSheet);
		void draw(glm::vec4 destRect, glm::vec4 uvRect, GLubyte depth, GLuint textureID, ColorRGBA8 color);
		void draw(const std::vector<glm::vec2>& points, glm::vec4 uvRect, GLubyte depth, GLuint textureID, ColorRGBA8 color);

	private:
		/** 
		 * @brief Prepares structural tracking contexts for a fresh recording sweep. 
		 */
		void begin(const glm::mat4& cameraMatrix, GlyphSortType sortType);

		/** 
		 * @brief Processes multi-tier sorting algorithms and transfers instanced streams to VRAM. 
		 */
		void end();

		/** 
		 * @brief Iterates over draw calls to execute fast instanced multi-pass drawing sweeps. 
		 */
		void render();

		/*!
		* \brief Create all needed render batches.
		*/
		void createRenderBatches();

		/*!
		* \brief Sort Glyph array.
		*/
		void sortGlyph();

		/* COMPARATOR */
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint m_vbo = 0;								/* vertex buffer object */
		GLuint m_vao = 0;								/* vertex array object */
		GlyphSortType m_sortType = GlyphSortType::NONE;	/* Current sort mode */
		std::vector<Glyph*> m_glyphsPointers;			/* Pointers of glyphs for sorting */
		std::vector<Glyph> m_glyphs;					/* Array of glyphs */
		std::vector<RenderBatch> m_renderBatches;		/* Array of render batches */

		LightMap m_lightMap;							/* Light map */
		GLSLProgram m_textureProgram;					/* The texture shader program */
	};
}
