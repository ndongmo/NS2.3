/**
 * @file ImageLoader.h
 * @brief Unified asset loader utilizing stb_image for multi-format image streaming.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated for NS2 Architecture (Universal Format Pass)
 */

#pragma once

#include "CoreTypes.h"

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace NS2
{
	/**
	 * @class ImageLoader
	 * @brief Static factory class that decodes image files (PNG, JPG, BMP, TGA) into OpenGL VRAM textures.
	 */
	class ImageLoader
	{
	public:
		ImageLoader() = delete; // Static factory layout; prevent instantiation

		/**
		 * @brief Parses an image file from disk (PNG, JPG, BMP, TGA) and populates a GLTexture container.
		 * @param filePath Local file path to the target image file asset.
		 * @param outTexture Reference to an existing GLTexture structure to receive the ID and dimensions.
		 * @return True if compilation passed successfully, false if asset streams broke.
		 */
		static bool load(const std::string& filePath, GLTexture& outTexture);

		/**
		 * @brief Procedurally allocates a tiny 1x1 solid pixel color texture to act as a shader fallback material.
		 * @param color Normalised rgba multiplier vector coefficients.
		 * @return Raw hardware OpenGL texture tracking identifier.
		 */
		static GLuint create(const glm::vec4& color);
	};
}
