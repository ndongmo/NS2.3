/**
 * @file ImageLoader.cpp
 * @brief Implements multi-format texture generation routines using stb_image.
 * @author Ndongmo Silatsa Fabrice
 */

#include "ImageLoader.h"
#include "Logger.h"
#include <stb_image.h> 

namespace NS2
{
	bool ImageLoader::load(const std::string& filePath, GLTexture& outTexture)
	{
		int width = 0;
		int height = 0;
		int channels = 0;

		// 🚀 UNIVERSAL STREAMING: stb_image reads the file header and automatically 
		// handles PNG, JPG, BMP, or TGA parsing under the hood.
		unsigned char* pixelData = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

		if (pixelData == nullptr)
		{
			logE << "IMAGE_LOADER_ERROR: Failed to load or decode texture path: " << filePath
				<< "\nReason: " << stbi_failure_reason() << std::endl;

			outTexture.id = 0;
			outTexture.width = 0;
			outTexture.height = 0;
			return false;
		}

		// Allocate an open slot cache matrix handle on your graphics hardware
		glGenTextures(1, &outTexture.id);
		glBindTexture(GL_TEXTURE_2D, outTexture.id);

		// Upload the pixel memory grid linearly up into active video memory (VRAM)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Configure filtering layout constraints to optimize asset scaling behavior
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbind the resource and free the temporary CPU ram allocations
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(pixelData);

		outTexture.width = width;
		outTexture.height = height;

		return true;
	}

	GLuint ImageLoader::create(const glm::vec4& color)
	{
		GLuint textureID = 0;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		unsigned char rawBytes[4] = {
			static_cast<unsigned char>(color.r * 255.0f),
			static_cast<unsigned char>(color.g * 255.0f),
			static_cast<unsigned char>(color.b * 255.0f),
			static_cast<unsigned char>(color.a * 255.0f)
		};

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawBytes);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
		return textureID;
	}
}
