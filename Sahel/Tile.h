/*!
* \file Tile.h
* \brief Tile data structure.
*	It represents a ground square area.
* \author Ndongmo Silatsa Fabrice
* \date 15-06-2018
* \version 2.0
*/

#pragma once

#include "Types.h"
#include "IDraw.h"
#include "IFileManager.h"

#include <CoreTypes.h>

class ObjectLoader;
class ObjectSaver;
class TileEditor;

const float TILESIZE = 64.0f;

class Tile : public IDraw, public IFileManager
{
	friend ObjectLoader;
	friend ObjectSaver;
	friend TileEditor;

public:
	Tile();
	virtual void draw(NS2::SpriteBatch& spriteBatch) const override;
	virtual void drawDebug(NS2::DebugRenderer& debugRender, const NS2::ColorRGBA8& color) const override;

	/* SETTERS */
	void setTexWidth(float width) { 
		m_uvRect.z = width; 
		m_destRect.z = width * TILESIZE;
	}
	void setTexHeight(float height) { 
		m_uvRect.w = height; 
		m_destRect.w = height * TILESIZE;
	}
	void setMaterial(MaterialType material) { m_material = material; }
	void setColor(NS2::ColorRGBA8& color) { m_color = color; }
	void setTexture(NS2::GLTexture* texture) { m_texture = texture; }
	void setPosX(int x) { m_destRect.x = x * TILESIZE; }
	void setPosY(int y) { m_destRect.y = y * TILESIZE; }
	void setLastFrameDrawn(unsigned int frame) const { m_lastFrameDrawn = frame; }

	/* GETTERS */
	float getTexWidth() const { return m_uvRect.z; }
	float getTexHeight() const { return m_uvRect.w; }
	int getPosX() const { return (int)(m_destRect.x / TILESIZE); }
	int getPosY() const { return (int)(m_destRect.y / TILESIZE); }
	float getX() const { return m_destRect.x; }
	float getY() const { return m_destRect.y; }
	float getWidth() const { return m_destRect.z; }
	float getHeight() const { return m_destRect.w; }
	int getLastFrameDrawn() const { return m_lastFrameDrawn; }
	MaterialType getMaterial() const { return m_material; }
	const NS2::ColorRGBA8& getColor()const  { return m_color; }
	const NS2::GLTexture* getTexture()const { return m_texture; }
protected:
	mutable unsigned int m_lastFrameDrawn = 0;

	glm::vec4 m_uvRect;
	glm::vec4 m_destRect;
	MaterialType m_material;
	NS2::ColorRGBA8 m_color;
	NS2::GLTexture* m_texture = nullptr;
};

