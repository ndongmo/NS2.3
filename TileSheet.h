/*!
* \file TileSheet.h
* \brief Simple Tile Sheet class.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include "CoreTypes.h"

#include <glm\glm.hpp>

namespace NS2
{
	class TileSheet
	{
	public:
		/*!
		* \brief Initialize tile stuffs.
		* \param texture: tile texture
		* \parm dims: tile dimensions
		* \parm invX: inverse texture X axe or not
		* \parm invY: inverse texture Y axe or not
		*/
		void init(GLTexture* texture, ColorRGBA8 color, glm::ivec2 dims, bool invX, bool invY, GLubyte depth)
		{
			this->texture = texture;
			this->color = color;
			this->dims = dims;
			this->depth = depth;
			this->index = 0;

			if (invX) {
				this->invX = 1.0f;
				this->invZ = -1.0f;
			}
			else {
				this->invX = 0.0f;
				this->invZ = 1.0f;
			}
			if (invY) {
				this->invY = 1.0f;
				this->invW = -1.0f;
			}
			else {
				this->invY = 0.0f;
				this->invW = 1.0f;
			}
			updateUV();
		}
		/*!
		* \brief Updates vector of texture coordinates.
		* \param index: index of desired texture
		* \return vector of texture coordinates
		*/
		void updateUV()
		{
			if (dims.x * dims.y > 1) {
				int xTile = index % dims.x;
				int yTile = index / dims.x;

				uvRect.x = (xTile + invX) / (float)dims.x;
				uvRect.y = (yTile + invY) / (float)dims.y;
				uvRect.z = invZ / dims.x;
				uvRect.w = invW / dims.y;
			}
			else {
				uvRect.x = invX;
				uvRect.y = invY;
				uvRect.z *= invZ;
				uvRect.w *= invW;
			}
		}

		void inverseX(bool inv)
		{
			uvRect.z /= invZ;

			if (inv) {
				this->invX = 1.0f;
				this->invZ = -1.0f;
			}
			else {
				this->invX = 0.0f;
				this->invZ = 1.0f;
			}
			updateUV();
		}
		void inverseY(bool inv)
		{
			uvRect.w /= invW;

			if (inv) {
				this->invY = 1.0f;
				this->invW = -1.0f;
			}
			else {
				this->invY = 0.0f;
				this->invW = 1.0f;
			}
			updateUV();
		}
		void setIndex(unsigned int index)
		{
			this->index = index;
			updateUV();
		}
		void setDims(int x, int y)
		{
			dims.x = x;
			dims.y = y;
			index = 0;
			uvRect.z = uvRect.w = 1;
			updateUV();
		}
		void setUvRect(float w, float h) 
		{
			if (w != 0 && h != 0) {
				uvRect.z = w; uvRect.w = h;
				updateUV();
			}
		}
		void setUvRectW(float w)
		{
			if (w != 0) {
				uvRect.z = w;
				updateUV();
			}
		}
		void setUvRectH(float h)
		{
			if (h != 0) {
				uvRect.w = h;
				updateUV();
			}
		}
		bool getInvX() const { return (this->invX == 1.0f && this->invZ == -1.0f); }
		bool getInvY() const { return (this->invY == 1.0f && this->invW == -1.0f); }
		int getDimsX() const { return dims.x; }
		int getDimsY() const { return dims.y; }
		int getDim() const { return dims.x * dims.y; }
		glm::ivec2 getDims() const { return dims; }
		unsigned int getIndex() const { return index; }
		const glm::vec4& getUvRect() const { return uvRect; }

		GLTexture* texture = nullptr;	/* Tile texture */
		ColorRGBA8 color;				/* Part color */
		GLubyte depth = 0;				/* Texture depth */

	private:
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);	/* UV data */
		glm::ivec2 dims = glm::ivec2(1, 1);						/* Tile dimensions */
		unsigned int index = 0;									/* Current tile index */
		float invX = 0, invY = 0, invZ = 1, invW = 1;			/* Use to inverse the texture */
	};
}

